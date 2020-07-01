#include "ChessFigureBishop.h"
#include <cmath>

ChessFigureBishop::ChessFigureBishop(bool isWhiteTeam)
	:ChessFigureBase(isWhiteTeam) {

	if (isWhiteTeam)IconPath += "wBishop.png";
	else IconPath += "bBishop.png";
}


bool ChessFigureBishop::IfCanMoveTo(int &fromX, int &fromY, int &toX, int &toY)
{
	if (abs(fromX - toX) == abs(fromY - toY)) {
		return true;
	}
	if (fromX + fromY == toX + toY) {
		return true;
	}
	return false;
}