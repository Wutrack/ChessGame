
#include <QGridLayout>
#include <qlabel.h>
#include "ChessBoard.h"
#include "MyLable.h"
#include "ChessGrid.h"
#include "ChessMoveManager.h"
#include "ChessFigureList.h"
#include "ChessGame.h"

ChessBoard::ChessBoard(ChessGame &ChessGame)
{
	MoveManager = new ChessMoveManager(this, ChessGame);
	QGridLayout *grid = ChessGame.findChild<QGridLayout*>("ChessBoard");

	int x;
	int y = 8;
	bool bSetWhiteGrid = false;
	char letters[] = "aABCDEFGH";

	for (size_t i = 0; i < 9; i++)
	{
		y--;
		x = -1;
		bSetWhiteGrid = !bSetWhiteGrid;
		for (size_t k = 0; k < 9; k++)
		{
			//Add chars around the board
			if (i != 8 && k == 0 ) {
				MyLable *label = new MyLable(QString::number(y+1));
				grid->addWidget(label, i, k);
			}
			else if (i == 8 && k != 0) {
				MyLable *label = new MyLable(QString(letters[k]));
				grid->addWidget(label, i, k);
			}
			else if (i == 8 && k == 0) {
				continue;
			}
			else
			{
				ChessGrid *chessGrid = new ChessGrid(MoveManager, x, y, bSetWhiteGrid);
				ChessGridArray[x][y] = chessGrid;
				grid->addWidget(chessGrid, i, k);

				bSetWhiteGrid = !bSetWhiteGrid;
			}
			x++;
		}
	}

	connect(&ChessGame, SIGNAL(StartNewGameSignal()), this, SLOT(StartNewGame()));
}

void ChessBoard::StartNewGame()
{
	MoveManager->NewGame();
	bool bAddToWhiteTeam = true;
	ChessGrid *chessGrid = nullptr;

	for (size_t y = 0; y < 8; y++)
	{
		for (size_t x = 0; x < 8; x++)
		{
			chessGrid = ChessGridArray[x][y];
			chessGrid->DeleteFigure();

			if (y <= 1 || y >= 6) {
				//Set figure team
				if (y >= 6) bAddToWhiteTeam = false;

				if (y == 1 || y == 6) {
					chessGrid->SetFigurePtr(new ChessFigurePawn(bAddToWhiteTeam));
				}
				else {
					if (x == 0 || x == 7) {
						chessGrid->SetFigurePtr(new ChessFigureRook(bAddToWhiteTeam));
					}
					else if (x == 1 || x == 6) {
						chessGrid->SetFigurePtr(new ChessFigureKnight(bAddToWhiteTeam));
					}
					else if (x == 2 || x == 5) {
						chessGrid->SetFigurePtr(new ChessFigureBishop(bAddToWhiteTeam));
					}
					else if (x == 3) {
						chessGrid->SetFigurePtr(new ChessFigureQueen(bAddToWhiteTeam));
					}
					else if (x == 4) {
						chessGrid->SetFigurePtr(new ChessFigureKing(bAddToWhiteTeam));
						MoveManager->SetKing(chessGrid, bAddToWhiteTeam);
					}
				}

				chessGrid->setIcon(*chessGrid->GetFigure()->InitiateIcon());
			}
		}

	}
}

bool ChessBoard::IsLineinFree(const int &fromX, const int &fromY, const int &toX, const int &toY)
{
	int x = fromX, y = fromY;
	
	//Loop each grid of the line
	while (x != toX || y != toY)
	{
		if (x < toX) x++;
		else if (x > toX) x--;
		if (y < toY) y++;
		else if (y > toY) y--;

		if (!ChessGridArray[x][y]->IsFree()) {
			if (x == toX && y == toY) {
				return true;
			}
			return false;
		}	
	}
	return true;
}
