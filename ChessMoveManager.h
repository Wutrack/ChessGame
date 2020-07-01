#pragma once
#include <QObject>
#include <vector>

using std::vector;

class ChessMoveManager : public QObject
{
private:
	Q_OBJECT

	class ChessBoard*			Board					= nullptr;
	class QLabel*				NotifLabel				= nullptr;

	class ChessFigureBase*		FigureActive			= nullptr;
	class ChessGrid*			ChessGridActive			= nullptr;
	class ChessGrid*			ChessGridPawn		= nullptr;

	class ChessGrid*			WhiteKingGrid			= nullptr;
	class ChessGrid*			BlackKingGrid			= nullptr;
	class ChessGrid*			ChechToTheKingGrid		= nullptr;

	class ChessGrid*			PawnKillGridSave			= nullptr;

	class ChessGame&			Game;

	bool						bIsGameFreeze			= false;
	bool						bIsWhiteTurn			= true;

	bool	IfFigureCanMoveHere		(const ChessGrid *fromGrid, const ChessGrid *toGrid);
	bool	CheckOnCheckToTheKing();
	bool	CheckOnCheckmate();

public:
	ChessMoveManager(class ChessBoard*, class ChessGame&);

	void	OnGridClicked			(class ChessGrid*);

	void	SetKing					(ChessGrid *, bool);

	void	NewGame();

private slots:
	void	ChangePawn(const QString &);

};

