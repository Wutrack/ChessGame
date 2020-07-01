#pragma once

#include <QtWidgets/QMainWindow>
#include <qpushbutton.h>
#include "ui_ChessGame.h"

class ChessGame : public QMainWindow
{
    Q_OBJECT

public:
    ChessGame(QWidget *parent = Q_NULLPTR);


    void	SendNotification    (const QString &);
    void    RecordMove          (class ChessGrid*, class ChessGrid*, bool);
    void    RecordMove          (const QString &, bool);
    void    FinishGame          (bool);
    void    ChangePawn();
private:
    Ui::ChessGameClass ui;

    int RecordCounter;
    class QTimer *NotificationTimer = nullptr;

signals:

    void StartNewGameSignal();
    void ChangePawnOnSelectedFigureSignal(const QString&);

private slots:

    void StartNewGame();
};