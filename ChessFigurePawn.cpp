#include "ChessFigurePawn.h"
#include <cmath>

ChessFigurePawn::ChessFigurePawn(bool isWhiteTeam)
	:ChessFigureBase(isWhiteTeam) {

	if (isWhiteTeam)IconPath += "wPawn.png";
	else IconPath += "bPawn.png";
}


bool ChessFigurePawn::IfCanMoveTo(int &fromX, int &fromY, int &toX, int &toY)
{
	if (bIsWhiteTeam) {
		if (toY > fromY) {
			if (toY - fromY == 1) {
				if (abs(fromX - toX) <= 1) {
					return true;
				}
			}
			else if (fromY == 1 && toY - fromY == 2 && fromX == toX) {
				return true;
			}
		}
	}
	if (!bIsWhiteTeam) {
		if (toY < fromY) {
			if (fromY - toY == 1) {
				if (abs(fromX - toX) <= 1) {
					return true;
				}
			}
			else if (fromY == 6 && fromY - toY == 2 && fromX == toX) {
				return true;
			}
		}
	}
	return false;
}

void ChessFigurePawn::Step()
{
	ChessFigureBase::Step();
	
	if (StepCount > 1) {
		bForvardStep = false;
	}
}
