#include "ChessGame.h"
#include <Qtimer>
#include <Qtime>
#include "ChessBoard.h"
#include "ChessGrid.h"
#include "ChessFigureList.h"
#include <algorithm>

ChessGame::ChessGame(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	//Timer for notification
	NotificationTimer = new QTimer(this);
	NotificationTimer->setSingleShot(true);

	ui.MoveRecord->setReadOnly(true);
	ui.ComfirmNewGameWidget->setVisible(false);
	ui.ChangePawnWidget->setVisible(false);

	//Exit from game button (Main Menu)
	connect(ui.ExitButton, &QPushButton::released, this, []() {exit(0); });
	//Start new game button (Main Menu)
	connect(ui.StartGameButton, SIGNAL(released()), this, SLOT(StartNewGame()));
	//Back to main menu button (Game)
	connect(ui.BackToMenuButton, &QPushButton::released, this, [&]() {
		ui.MainMenu->setVisible(true);
		});
	//Open new game comfirm widget (Game)
	connect(ui.NewGameButton, &QPushButton::released, this, [&]() {
		ui.ComfirmNewGameWidget->setVisible(true);
		});
	//Cancel start new game (Game)
	connect(ui.ComfirmNewGameButtons, &QDialogButtonBox::rejected, this, [&]() {
		ui.ComfirmNewGameWidget->setVisible(false);
		});
	//Comfirm start new game (Game)
	connect(ui.ComfirmNewGameButtons, &QDialogButtonBox::accepted, this, [&]() {
		ui.ComfirmNewGameWidget->setVisible(false);
		StartNewGame();
		});
	//Choose new figure instead pawn (Game)
	connect(ui.ChangePawnButton, &QPushButton::released, this, [&]() {
		QString str;

		if (ui.radioButton->isChecked()) {
			str = ui.radioButton->text();
		}
		else if (ui.radioButton_2->isChecked()) {
			str = ui.radioButton_2->text();
		}
		else if (ui.radioButton_3->isChecked()) {
			str = ui.radioButton_3->text();
		}
		else if (ui.radioButton_4->isChecked()) {
			str = ui.radioButton_4->text();
		}
		else if (ui.radioButton_5->isChecked()) {
			str = ui.radioButton_5->text();
		}

		ui.ChangePawnWidget->setVisible(false);
		emit ChangePawnOnSelectedFigureSignal(str);
		});

}


void ChessGame::StartNewGame() {

	RecordCounter = 1;
	ui.MoveRecord->setText("");
	ui.NotificationLabel->setText("");
	ui.MenuLabel->setText("The best chess game ever!");
	ui.MainMenu->setVisible(false);
	emit StartNewGameSignal();
}

void ChessGame::SendNotification(const QString &text)
{
	ui.NotificationLabel->setText(text);

	NotificationTimer->stop();
	NotificationTimer->setInterval(5000);
	NotificationTimer->start();

	connect(NotificationTimer, &QTimer::timeout, this, [&]() { ui.NotificationLabel->setText(""); });
}

void ChessGame::RecordMove(class ChessGrid *fromGrid, class ChessGrid *toGrid, bool isWhiteTeamTurn)
{
	QString str, text;
	text = QString::number(RecordCounter) + ": ";

	if (dynamic_cast<ChessFigurePawn *>(toGrid->GetFigure())) {
		text += "Pawn";
	}
	else if (dynamic_cast<ChessFigureRook *>(toGrid->GetFigure())) {
		text += "Rook";
	}
	else if (dynamic_cast<ChessFigureKnight *>(toGrid->GetFigure())) {
		text += "Knight";
	}
	else if (dynamic_cast<ChessFigureBishop *>(toGrid->GetFigure())) {
		text += "Bishop";
	}
	else if (dynamic_cast<ChessFigureQueen *>(toGrid->GetFigure())) {
		text += "Queen";
	}
	else if (dynamic_cast<ChessFigureKing *>(toGrid->GetFigure())) {
		text += "King";
	}

	//Generate grid names for recording
	int fromX, fromY, toX, toY;
	fromGrid->GetGridPosition(fromX, fromY);
	toGrid->GetGridPosition(toX, toY);
	char letters[] = "ABCDEFGH";

	text += QString(" ") + QString(letters[fromX]) + QString::number(fromY+1);
	text += QString("->") + QString(letters[toX]) + QString::number(toY+1);

	if (!isWhiteTeamTurn) {
		str = "<font color='#873600'>" + text + "</font>\n";
	}
	else
	{
		str = "<font color='#000000'>" + text + "</font>\n";
	}	

	RecordCounter++;
	ui.MoveRecord->append(str);
}

void ChessGame::RecordMove(const QString &text, bool isWhiteTeamTurn)
{
	QString str;
	if (!isWhiteTeamTurn) {
		str = "<font color='#873600'>"+ text + "</font>\n";
	}
	else
	{
		str = "<font color='#000000'>" + text + "</font>\n";
	}

	RecordCounter++;
	ui.MoveRecord->append(str);
}

void ChessGame::FinishGame(bool isWhiteWon)
{
	//Stop timer for permanent label
	NotificationTimer->stop();
	QString text = "Great Move! CHECKMATE!\n";

	if (!isWhiteWon) {
		text += "White";
	}
	else {
		text += "Black";
	}
	text += " won! Want to play again?";

	ui.MenuLabel->setText(text);
	ui.NotificationLabel->setText(text);
}

void ChessGame::ChangePawn()
{
	ui.ChangePawnWidget->setVisible(true);
}
