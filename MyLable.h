#pragma once
#include <qlabel.h>
class MyLable :	public QLabel
{
private:

public:
	MyLable(QString &text) :
		QLabel(text) {
		setFixedSize(40, 40);
		setAlignment(Qt::AlignCenter);
		setStyleSheet("background-color:silver;");
	}
};

