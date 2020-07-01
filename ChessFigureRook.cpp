#include "ChessFigureRook.h"

ChessFigureRook::ChessFigureRook(bool isWhiteTeam)
	:ChessFigureBase(isWhiteTeam)
{

	if (isWhiteTeam)IconPath += "wRook.png";
	else IconPath += "bRook.png";
}

bool ChessFigureRook::IfCanMoveTo(int &fromX, int &fromY, int &toX, int &toY)
{
	if(fromX == toX || fromY == toY)
		return true;
	return false;
}
