#pragma once
#include "ChessFigureBase.h"
class ChessFigureRook : public ChessFigureBase
{
private:

public:

	ChessFigureRook(bool);

	virtual bool IfCanMoveTo(int &fromX, int &fromY, int &toX, int &toY) override;
};

