#pragma once
#include "ChessFigureBase.h"
class ChessFigurePawn :	public ChessFigureBase
{
private:

public:

	bool bForvardStep = false;
	ChessFigurePawn(bool);

	virtual bool IfCanMoveTo(int &fromX, int &fromY, int &toX, int &toY) override;
	virtual void Step() override;
};

