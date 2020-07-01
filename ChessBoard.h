#pragma once
#include <QObject>


class ChessBoard : public QObject
{ 
	Q_OBJECT

private:
	class ChessGrid *			ChessGridArray[8][8];
	class ChessMoveManager*		MoveManager				= nullptr;

public:
	ChessBoard(class ChessGame&);

	bool IsLineinFree(const int &fromX, const int &fromY, const int &toX, const int &toY);

	class ChessGrid* GetChessGrid(const int &x, const int &y) { return ChessGridArray[x][y]; };

private slots:

	void StartNewGame();
};

