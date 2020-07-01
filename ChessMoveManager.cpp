#include "ChessMoveManager.h"
#include "ChessFigureBase.h"
#include "ChessFigureList.h"
#include "ChessGrid.h"
#include "ChessBoard.h"
#include <QTimer>
#include <QLabel>
#include <cmath>
#include "ChessGame.h"


ChessMoveManager::ChessMoveManager(ChessBoard* board, ChessGame& chessGame)
	:Board(board), Game(chessGame)
{
	connect(&Game, SIGNAL(ChangePawnOnSelectedFigureSignal(const QString &)),
		this, SLOT(ChangePawn(const QString &)));
}

void ChessMoveManager::ChangePawn(const QString &str)
{
	ChessFigureBase *newFigure;

	//Spawn new figure
	if (str == "Queen") {
		newFigure = new ChessFigureQueen(!bIsWhiteTurn);
	}
	else if (str == "Rook") {
		newFigure = new ChessFigureRook(!bIsWhiteTurn);
	}
	else if (str == "Bishop") {
		newFigure = new ChessFigureBishop(!bIsWhiteTurn);
	}
	else if (str == "Knight") {
		newFigure = new ChessFigureKnight(!bIsWhiteTurn);
	}
	else {
		newFigure = new ChessFigurePawn(!bIsWhiteTurn);
	}

	//If create new figure was succeed - change figure
	if (newFigure != nullptr) {
		newFigure->InitiateIcon();
		ChessGridPawn->DeleteFigure();
		ChessGridPawn->SetFigurePtr(newFigure);
	}

	bIsGameFreeze = false;
	Game.RecordMove("Pawn changed!", bIsWhiteTurn);

	//Check on Check to the another team King after move
	if (CheckOnCheckToTheKing()) {

		//Save figure grid, that check to the king
		ChechToTheKingGrid = ChessGridPawn;

		//Check if its Checkmate
		if (CheckOnCheckmate()) {
			bIsGameFreeze = true;
			Game.FinishGame(bIsWhiteTurn);
		}
		else {
			Game.SendNotification("Great Move! Check to the King!");
		}
	}

}

void ChessMoveManager::OnGridClicked(ChessGrid* secondClickedGrid)
{
	ChessGrid *SaveKindGrid = nullptr;

	ChessFigureBase *SaveKilledFigure = nullptr;

	bool bKillFigure = false;

	bool bCanMove = false;

	//Check if game has finished
	if (bIsGameFreeze) return;

	//If selected grid is already active
	if (ChessGridActive == secondClickedGrid) {
		ChessGridActive->ToggleActiveState();
		FigureActive = nullptr;
		ChessGridActive = nullptr;
		return;
	}

	//Castling logic
	if (ChessGridActive != nullptr && secondClickedGrid != nullptr) {
		if (ChessFigureKing *kingfigure = dynamic_cast<ChessFigureKing *>(FigureActive)) {
			if (kingfigure->GetStep() == 0 && !kingfigure->bCastling) {

				int fromX, fromY, toX, toY;
				ChessGridActive->GetGridPosition(fromX, fromY);
				secondClickedGrid->GetGridPosition(toX, toY);
				ChessFigureRook *rookfigure;

				int rookX;

				if (toX == 6 || toX == 2) {

					(toX == 6) ? rookX = 7 : rookX = 0;

					if (rookfigure = dynamic_cast<ChessFigureRook *>(Board->GetChessGrid(rookX, toY)->GetFigure())) {

						if (rookfigure->GetStep() == 0) {

							ChessGrid *saveKingGrid;

							(bIsWhiteTurn) ? saveKingGrid = WhiteKingGrid : saveKingGrid = BlackKingGrid;

							int tempX = fromX;


							//Loop all way and check on Check to the king
							for(int i = 0; i < 3; i++)
							{
								//If have Check to the king on way - undo all
								if (CheckOnCheckToTheKing()) {

									(bIsWhiteTurn) ? WhiteKingGrid = saveKingGrid : BlackKingGrid = saveKingGrid;

									Game.SendNotification("You cant make castling now!");
									return;
								}
								(toX == 6) ? tempX++ : tempX--; 
								(bIsWhiteTurn) ? WhiteKingGrid = Board->GetChessGrid(tempX, toY) 
									: BlackKingGrid = Board->GetChessGrid(tempX, toY);

							}
							(toX == 6) ? tempX = 5 : tempX = 3;

							secondClickedGrid->MoveFigureHereFromGrid(saveKingGrid);
							Board->GetChessGrid(tempX, toY)->MoveFigureHereFromGrid(Board->GetChessGrid(rookX, toY));
							kingfigure->bCastling = true;

							bIsWhiteTurn = !bIsWhiteTurn;

							//Check on Check to the another team King after move
							if (CheckOnCheckToTheKing()) {

								//Save figure grid, that check to the king
								ChechToTheKingGrid = Board->GetChessGrid(tempX, toY);

								//Check if its Checkmate
								if (CheckOnCheckmate()) {
									bIsGameFreeze = true;
									Game.FinishGame(bIsWhiteTurn);
								}
								else {
									Game.SendNotification("Great Move! Check to the King!");
								}
							}

							ChessGridActive->ToggleActiveState();
							FigureActive->Step();
							rookfigure->Step();
							FigureActive = nullptr;
							ChessGridActive = nullptr;
							Game.RecordMove("Castling", bIsWhiteTurn);
							Game.SendNotification("Great castling!");
							return;
						}
					}
				}

			}
		}
	}

	//If no selected grid
	if (FigureActive == nullptr) {

		//If selected grid have a figure
		if (secondClickedGrid->GetFigure()) {

			//If figure is from current team turn
			if (secondClickedGrid->GetFigure()->IsWhiteTeam() == bIsWhiteTurn) {
				FigureActive = secondClickedGrid->GetFigure();
				ChessGridActive = secondClickedGrid;
				ChessGridActive->ToggleActiveState(); 
			}
			else {
				Game.SendNotification("Now is another team turn!");
			}
		}
	}
	//Check if figure can move in selected grid
	else {
		if (bCanMove = IfFigureCanMoveHere(ChessGridActive, secondClickedGrid)) {
			Game.SendNotification("Nice move!");
		}
		else {
			Game.SendNotification("You cant move here!");
		}
	}

	//Start move logic if true
	if (bCanMove == true) {

		//If selected grid have enemy figure - move with kill
		if (!secondClickedGrid->IsFree() || PawnKillGridSave) {
			bKillFigure = true;
		}
		//Save killed figure for undo
		if (bKillFigure) {

			//If pawn special kill
			if (PawnKillGridSave) {
				SaveKilledFigure = PawnKillGridSave->GetFigure();
				PawnKillGridSave->SetFigurePtr(nullptr);
				PawnKillGridSave->setIcon(QIcon());
			}
			else{
				SaveKilledFigure = secondClickedGrid->GetFigure();		
			}
		}

		//Move selected figure to selected grid
		secondClickedGrid->MoveFigureHereFromGrid(ChessGridActive);

		//If selected figure is a King - save its new position. Save variable for undo
		if (dynamic_cast<ChessFigureKing *>(FigureActive)) {

			if (bIsWhiteTurn) {
				SaveKindGrid = WhiteKingGrid;
				WhiteKingGrid = secondClickedGrid;
			}
			else {
				SaveKindGrid = BlackKingGrid;
				BlackKingGrid = secondClickedGrid;
			}
		}

		//Check on Check to the self King after move. If true - return figures back
		if (CheckOnCheckToTheKing()) {

			//Undo King position
			if (dynamic_cast<ChessFigureKing *>(FigureActive)) {

				(bIsWhiteTurn) ? WhiteKingGrid = SaveKindGrid : BlackKingGrid = SaveKindGrid;
			}

			//Move figure back
			ChessGridActive->MoveFigureHereFromGrid(secondClickedGrid);

			//If move was with kill
			if (bKillFigure) {
				if (PawnKillGridSave) {
					PawnKillGridSave->SetFigurePtr(SaveKilledFigure);
				}
				else {
					secondClickedGrid->SetFigurePtr(SaveKilledFigure);
				}
			}
			Game.SendNotification("Look out! Check to the King!");
		}
		else {
			//Clear temp variable
			if (bKillFigure) {
				SaveKilledFigure = nullptr;
			}

			ChessGridActive->ToggleActiveState();

			//Change team turn
			bIsWhiteTurn = !bIsWhiteTurn;

			//Check on Check to the another team King after move
			if (CheckOnCheckToTheKing()) {

				//Save figure grid, that check to the king
				ChechToTheKingGrid = secondClickedGrid;

				//Check if its Checkmate
				if (CheckOnCheckmate()) {
					bIsGameFreeze = true;
					Game.FinishGame(bIsWhiteTurn);
				}
				else {
					Game.SendNotification("Great Move! Check to the King!");
				}
			}
			Game.RecordMove(ChessGridActive, secondClickedGrid, bIsWhiteTurn);

			FigureActive->Step();

			if (ChessFigurePawn *pawn = dynamic_cast<ChessFigurePawn *>(FigureActive)) {

				int fromX, fromY, toX, toY;
				ChessGridActive->GetGridPosition(fromX, fromY);
				secondClickedGrid->GetGridPosition(toX, toY);

				if (FigureActive->GetStep() == 1) {
					if (fromX == toX && abs(fromY - toY) == 2) {
						pawn->bForvardStep = true;
					}
				}
				if (toY == 0 || toY == 7) {
					bIsGameFreeze = true;
					ChessGridPawn = secondClickedGrid;
					Game.ChangePawn();
				}
			}


			//Clear pointers for new move
			FigureActive = nullptr;
			ChessGridActive = nullptr;
			PawnKillGridSave = nullptr;
		}
	}
}


void ChessMoveManager::SetKing(ChessGrid *grid, bool bIsWhite)
{
	(bIsWhite) ? WhiteKingGrid = grid : BlackKingGrid = grid;

}

void ChessMoveManager::NewGame()
{
	bIsWhiteTurn = true;
	bIsGameFreeze = false;

	if (ChessGridActive) {
		ChessGridActive->ToggleActiveState();
		ChessGridActive = nullptr;
		FigureActive = nullptr;
	}
}


bool ChessMoveManager::IfFigureCanMoveHere(const ChessGrid *fromGrid, const ChessGrid *toGrid)
{
	int fromX, fromY, toX, toY;
	fromGrid->GetGridPosition(fromX, fromY);
	toGrid->GetGridPosition(toX, toY);

	//Check if figure can move to that grid
	if (fromGrid->GetFigure()->IfCanMoveTo(fromX, fromY, toX, toY)) {

		//if that grid have the same figure color - return false
		if (ChessFigureBase *f = toGrid->GetFigure()) {
			if (f->IsWhiteTeam() == fromGrid->GetFigure()->IsWhiteTeam()) {
				return false;
			}
		}

		//Additional logic for Pawn
		if (dynamic_cast<ChessFigurePawn *>(fromGrid->GetFigure())) {

			if (fromX != toX && toGrid->IsFree()) {
				if (Board->GetChessGrid(toX, fromY)->GetFigure()) {

					if (ChessFigurePawn *tempPawn = dynamic_cast<ChessFigurePawn *>(Board->GetChessGrid(toX, fromY)->GetFigure())) {

						if (tempPawn->IsWhiteTeam() != bIsWhiteTurn && abs(fromY - toY) == 1) {

							if (tempPawn->bForvardStep) {
								PawnKillGridSave = Board->GetChessGrid(toX, fromY);
								return true;
							}

						}
					}
				}
				return false;
			}
			if (fromX == toX && !toGrid->IsFree()) {
				return false;
			}
		}

		//another logic for Knight
		if (dynamic_cast<ChessFigureKnight *>(fromGrid->GetFigure())) {
			return true;
		}

		//Check if any other figure is situated on the way, exept the last grid
		if (Board->IsLineinFree(fromX, fromY, toX, toY)) {
			return true;
		}		
	}
	return false;
}

bool ChessMoveManager::CheckOnCheckToTheKing()
{
	//Variable for chosen grid
	ChessGrid *TempGrid = nullptr;

	//Variable for chosen figure
	ChessFigureBase *TempFigure = nullptr;

	//Loop all grids
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t k = 0; k < 8; k++)
		{
			//Get a grid
			TempGrid = Board->GetChessGrid(k, i);

			//if the grid has a figure
			if (TempFigure = TempGrid->GetFigure()) {

				//If the figure has another color
				if (TempFigure->IsWhiteTeam() != bIsWhiteTurn) {

					//Check if selected figure can Check to the another team king
					if (TempFigure->IsWhiteTeam()) {
						if (IfFigureCanMoveHere(TempGrid, BlackKingGrid))
							return true;
					}
					else {
						if (IfFigureCanMoveHere(TempGrid, WhiteKingGrid))
							return true;
					}
				}
			}
		}
	}
	return false;
}

bool ChessMoveManager::CheckOnCheckmate()
{
	//Temp variable for chosen grid
	ChessGrid *TempGrid = nullptr;

	//For Grid Save
	ChessFigureBase *SaveFigure = nullptr;

	//Save start king position
	ChessGrid *KingGrid = nullptr;
	
	(bIsWhiteTurn) ? KingGrid = WhiteKingGrid : KingGrid = BlackKingGrid;


	int kingX, kingY;
	KingGrid->GetGridPosition(kingX, kingY);

	int checkX, checkY;
	ChechToTheKingGrid->GetGridPosition(checkX, checkY);

	int x = 0, y = 0;

	//Check if king can avoid the Check if it move to another grid
	for (int i = -1; i < 7; i++)
	{
		if (i < 2) {
			x = kingX + i;
			y = kingY + 1;
		}
		else if (i == 2) {
			x = kingX - 1;
			y = kingY;
		}
		else if (i == 3) {
			x = kingX + 1;
			y = kingY;
		}
		else {
			x = kingX + (i - 5);
			y = kingY - 1;
		}

		// x y values should be in right dimension
		if (x >= 0 && x < 8 && y >= 0 && y < 8) {
			TempGrid = Board->GetChessGrid(x, y);

			//If selected grid have a figure from the King's team
			if (!TempGrid->IsFree()) {
				if (TempGrid->GetFigure()->IsWhiteTeam() == bIsWhiteTurn) {
					continue;
				}
			}
			//If King kill ChechFigure or ChechFigure cant check to the king - check on Check to the king again
			if (ChechToTheKingGrid == TempGrid || !IfFigureCanMoveHere(ChechToTheKingGrid, TempGrid)) {

				if (TempGrid->GetFigure()) {
					SaveFigure = TempGrid->GetFigure();
				}

				//Move King to chosen grid and save figure 
				TempGrid->MoveFigureHereFromGrid(KingGrid);
				
				//Change King position for test
				if (bIsWhiteTurn) WhiteKingGrid = TempGrid;
				else BlackKingGrid = TempGrid;

				//If true - undo all moves
				if (!CheckOnCheckToTheKing()) {

					KingGrid->MoveFigureHereFromGrid(TempGrid);

					//Return King's position back

					(bIsWhiteTurn) ? WhiteKingGrid = KingGrid : BlackKingGrid = KingGrid;

					if (SaveFigure) {
						TempGrid->SetFigurePtr(SaveFigure);
						SaveFigure = nullptr;
					}
					return false;
				}

				KingGrid->MoveFigureHereFromGrid(TempGrid);

				if (SaveFigure) {
					TempGrid->SetFigurePtr(SaveFigure);
					SaveFigure = nullptr;
				}

			}
		}
	}

	//Return King's position back
	(bIsWhiteTurn) ? WhiteKingGrid = KingGrid : BlackKingGrid = KingGrid;

	int checkType = 0;

	//Select check type
	//If horisontal or vertical
	if (checkX == kingX || checkY == kingY) {
		checkType = 0;
	}
	//If diagonal
	else if (abs(checkX - kingX) == abs(checkY - kingY)){
		checkType = 1;
	}
	//If Knight is Chech to the king
	else if (dynamic_cast<ChessFigureKnight *>(ChechToTheKingGrid->GetFigure())) {
		checkType = 2;
	}



	for (size_t i = 0; i < 8; i++)
	{
		for (size_t k = 0; k < 8; k++)
		{
			TempGrid = Board->GetChessGrid(i, k);

			//If Grid has a figure
			if (!TempGrid->IsFree()) {
				if (TempGrid->GetFigure()->IsWhiteTeam() == bIsWhiteTurn 
					&& !dynamic_cast<ChessFigureKing *>(TempGrid->GetFigure())) {

					int tempX = checkX, tempY = checkY;

					bool bToTop;

					switch (checkType)
					{
					//If horisontal or vertical
					case 0:
						if (checkX == kingX) {
							bToTop = checkY < kingY;

							while (tempY != kingY) {
								if (IfFigureCanMoveHere(TempGrid, Board->GetChessGrid(tempX, tempY))) {
									return false;
								}
								if (bToTop)tempY++;
								else tempY--;
							}
						}
						else {
							bToTop = checkX < kingX;

							while (tempX != kingX) {
								if (IfFigureCanMoveHere(TempGrid, Board->GetChessGrid(tempX, tempY))) {
									return false;
								}
								if (bToTop)tempX++;
								else tempX--;
							}
						}
						break;
					//If diagonal
					case 1:
						if (checkX < kingX && checkY < kingY || checkX > kingX && checkY > kingY ) {
							bToTop = checkX > kingX;

							while (tempY != kingY) {
								if (IfFigureCanMoveHere(TempGrid, Board->GetChessGrid(tempX, tempY))) {
									return false;
								}
								if (bToTop) {
									tempX--;
									tempY--;
								}
								else {
									tempX++;
									tempY++;
								}
							}
						}
						else {
							bToTop = checkX < kingX;

							while (tempX != kingX) {
								if (IfFigureCanMoveHere(TempGrid, Board->GetChessGrid(tempX, tempY))) {
									return false;
								}
								if (bToTop) {
									tempX++;
									tempY--;
								}
								else {
									tempX--;
									tempY++;
								}
							}
						}
						break;
					//If Knight is Chech to the king
					case 2:
						if (IfFigureCanMoveHere(TempGrid, Board->GetChessGrid(checkX, checkY))) {
							return false;
						}
						break;
					}

				}
			}
		}
	}
	return true;
}

