#pragma once
#include "ChessFigureBase.h"
class ChessFigureBishop :public ChessFigureBase
{
private:

public:
	ChessFigureBishop(bool);

	virtual bool IfCanMoveTo(int &fromX, int &fromY, int &toX, int &toY) override;
};

