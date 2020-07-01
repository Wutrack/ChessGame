#include "ChessGrid.h"
#include "ChessFigureBase.h"
#include "ChessMoveManager.h"
#include <QFileDialog>


void ChessGrid::SetGridCollor()
{
	if (bIsWhiteCollor) {
		setStyleSheet("background-color:white;");
	}
	else {
		setStyleSheet("background-color:orange;");
	}
}

void ChessGrid::mousePressEvent(QMouseEvent *e)
{
	QPushButton::mousePressEvent(e);
	MoveManager->OnGridClicked(this);
}

ChessGrid::ChessGrid(ChessMoveManager *moveManager, const int &inX, const int &inY, bool isWhiteCollor)
	: MoveManager(moveManager), x(inX), y(inY), bIsWhiteCollor(isWhiteCollor)
{
	int size = 40;
	setFixedSize(QSize(size, size));
	setIconSize(QSize(size, size));
	SetGridCollor();
}


void ChessGrid::SetFigurePtr(ChessFigureBase *ptr)
{
	Figure = ptr;
	if (ptr != nullptr) {
		setIcon(*ptr->GetIcon());
	}
}

//Move a figure to this grid
void ChessGrid::MoveFigureHereFromGrid(ChessGrid *fromGrid)
{
	SetFigurePtr(fromGrid->GetFigure());
	fromGrid->SetFigurePtr(nullptr);

	fromGrid->setIcon(QIcon());
}

void ChessGrid::ToggleActiveState()
{
	bActiveState = !bActiveState;
	if (bActiveState) {
		setStyleSheet("background-color:lime;");
	}
	else {
		SetGridCollor();
	}
}

void ChessGrid::DeleteFigure()
{
	if (Figure != nullptr) {
		delete Figure;
		Figure = nullptr;
	}
	setIcon(QIcon());
}

bool ChessGrid::IsFree()const 
{
	return Figure == nullptr;
}

void ChessGrid::GetGridPosition(int &outX, int &outY)const
{
	outX = x;
	outY = y;
}
