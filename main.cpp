#include "mainwindow.h"
#include "RunGuard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	RunGuard guard( "{070E1BFB-9E1F-45D5-B9A4-D90F059ACAD9}" );
	if ( !guard.tryToRun() )
		return 0;

	QApplication a(argc, argv);
	a.setQuitOnLastWindowClosed(false);
	MainWindow w;
	w.start();

	return a.exec();
}
