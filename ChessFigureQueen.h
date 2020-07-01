#pragma once
#include "ChessFigureBase.h"
class ChessFigureQueen : public ChessFigureBase
{
private:

public:
	ChessFigureQueen(bool);

	virtual bool IfCanMoveTo(int &fromX, int &fromY, int &toX, int &toY) override;
};

