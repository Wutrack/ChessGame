#pragma once
#include "ChessFigureBase.h"
class ChessFigureKing :	public ChessFigureBase
{
private:

public:
	bool bCastling = false;

	ChessFigureKing(bool);

	virtual bool IfCanMoveTo(int &fromX, int &fromY, int &toX, int &toY) override;
};

