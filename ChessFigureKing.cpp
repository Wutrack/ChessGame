#include "ChessFigureKing.h"
#include <cmath>

ChessFigureKing::ChessFigureKing(bool isWhiteTeam)
	:ChessFigureBase(isWhiteTeam) {

	if (isWhiteTeam)IconPath += "wKing.png";
	else IconPath += "bKing.png";
}


bool ChessFigureKing::IfCanMoveTo(int &fromX, int &fromY, int &toX, int &toY)
{
	if (abs(fromX - toX) <= 1 && abs(fromY - toY) <= 1) {
		return true;
	}
	return false;
}