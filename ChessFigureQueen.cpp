#include "ChessFigureQueen.h"
#include <cmath>

ChessFigureQueen::ChessFigureQueen(bool isWhiteTeam)
	:ChessFigureBase(isWhiteTeam) {

	if (isWhiteTeam)IconPath += "wQueen.png";
	else IconPath += "bQueen.png";
}


bool ChessFigureQueen::IfCanMoveTo(int &fromX, int &fromY, int &toX, int &toY)
{
	if (fromX == toX || fromY == toY || abs(fromX - toX) == abs(fromY - toY)
		|| fromX + fromY == toX + toY) {
		return true;
	}
	return false;
}