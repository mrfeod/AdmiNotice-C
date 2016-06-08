#pragma once

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
	QString m_server  = QString();
	unsigned int m_id = 0;
};
