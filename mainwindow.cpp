#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QNetworkReply>
#include <QNetworkRequest>

#include <QCloseEvent>

namespace {
	const int ONE_MINUTE = 1000 * 2;
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_timer(this)
{
	ui->setupUi(this);

	setWindowFlags(windowFlags() & ~Qt::WindowMinimizeButtonHint & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowCloseButtonHint);
	setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
#ifndef Q_OS_WIN
	setWindowFlags(windowFlags() | Qt::X11BypassWindowManagerHint);
#endif

	connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::close);

	m_timer.setSingleShot(true);
	m_timer.setInterval(ONE_MINUTE);

	connect(&m_timer, &QTimer::timeout, this, &MainWindow::checkNotice);
	connect(&m_manager, &QNetworkAccessManager::finished, [this](QNetworkReply *reply)
	{
		if(reply->error() == QNetworkReply::NoError)
		{
			ui->textField->setText(reply->readAll());
			show();
		}
		reply->deleteLater();
	});
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::start()
{
	m_timer.start();
}

void MainWindow::checkNotice()
{
	m_manager.get(QNetworkRequest(QUrl("http://feod.noip.me/pull")));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	hide();
	start();
	event->ignore();
}
