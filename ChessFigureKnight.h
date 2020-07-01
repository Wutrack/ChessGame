#pragma once
#include "ChessFigureBase.h"
class ChessFigureKnight : public ChessFigureBase
{

public:
	ChessFigureKnight(bool);

	virtual bool IfCanMoveTo(int &fromX, int &fromY, int &toX, int &toY) override;
};

