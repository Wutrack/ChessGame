#include "ChessFigureKnight.h"
#include <cmath>

ChessFigureKnight::ChessFigureKnight(bool isWhiteTeam)
	:ChessFigureBase(isWhiteTeam){

	if (isWhiteTeam)IconPath += "wKnight.png";
	else IconPath += "bKnight.png";
}


bool ChessFigureKnight::IfCanMoveTo(int &fromX, int &fromY, int &toX, int &toY)
{
	if (abs(fromX - toX) < 3 && abs(fromY - toY) < 3) {
		if (abs(fromX - toX) + abs(fromY - toY) == 3) {
			return true;
		}
	}	
	return false;
}
