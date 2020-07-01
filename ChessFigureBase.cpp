#include "ChessFigureBase.h"

ChessFigureBase::ChessFigureBase(bool isWhiteTeam)
	:bIsWhiteTeam(isWhiteTeam)
{
	Icon = new QIcon();
}

void ChessFigureBase::Step()
{
	StepCount++;
}

QIcon *ChessFigureBase::InitiateIcon()
{
	Icon->addPixmap(QPixmap(IconPath));
	return Icon;
}
