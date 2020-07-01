
#include <QIcon>

#pragma once
class ChessFigureBase
{
protected:

	bool			bIsWhiteTeam;
	int				StepCount		= 0;
	QString			IconPath		= "Icons/ChessIcons/";
	QIcon *			Icon			= nullptr;
public:

	ChessFigureBase(bool);

	virtual bool	IfCanMoveTo(int &fromX, int &fromY, int &toX, int &toY) = 0;

	virtual void	Step();
	int				GetStep()			{ return StepCount; };
	bool			IsWhiteTeam()		{ return bIsWhiteTeam; };

	QIcon *			InitiateIcon();
	QIcon *			GetIcon()			{ return Icon; };
};

