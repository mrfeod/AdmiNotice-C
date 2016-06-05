#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTimer>
#include <QNetworkAccessManager>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void start();

private slots:
	void checkNotice();

private:
	void closeEvent(QCloseEvent *event);
	void moveToCenter();

private:
	Ui::MainWindow *ui;
	QTimer m_timer;
	QNetworkAccessManager m_manager;
};

#endif // MAINWINDOW_H
