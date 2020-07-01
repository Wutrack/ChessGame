#pragma once

#include <qpushbutton.h>

class ChessGrid : public QPushButton
{
private:

	bool							bIsWhiteCollor	= true;
	bool							bActiveState	= false;
	class ChessFigureBase *			Figure			= nullptr;
	class ChessMoveManager *		MoveManager		= nullptr;

	int			x		= 0,
				y		= 0;
	
	void			SetGridCollor();

	virtual void	mousePressEvent		(QMouseEvent *e) override;

public:
	ChessGrid(class ChessMoveManager*, const int &inX, const int &inY, bool isWhiteCollor);
	void					SetFigurePtr	(ChessFigureBase *ptr);
	void					MoveFigureHereFromGrid	(ChessGrid *fromGrid);

	void					ToggleActiveState();
	void					DeleteFigure();

	bool					IsFree() const;
	ChessFigureBase *		GetFigure() const	{ return Figure; };

	void					GetGridPosition		(int &outX, int &outY)const;
};

