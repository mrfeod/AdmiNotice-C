#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setQuitOnLastWindowClosed(false);
	MainWindow w;
	w.start();

	return a.exec();
}
