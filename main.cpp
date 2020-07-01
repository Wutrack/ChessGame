#include "ChessGame.h"
#include <QtWidgets/QApplication>
#include <qlabel.h>
#include "ChessBoard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChessGame w;

    ChessBoard Board(w);

    w.show();
    return a.exec();
}
