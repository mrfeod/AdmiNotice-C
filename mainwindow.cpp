#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "MessageItem.h"

#include <QNetworkReply>
#include <QNetworkRequest>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QCloseEvent>

#include <QDesktopWidget>

namespace {
	const int ONE_MINUTE = 1000 * 60;
}

namespace {
	const QString SETTING_ID     = "id";
	const QString SETTING_SERVER = "server";

	QVariant GetSetting(QString name)
	{
		QVariant res;
		QFile setting(name);
		if (setting.open(QIODevice::ReadOnly))
			res = QVariant(setting.readAll());

		qDebug() << "GetSetting" << res;
		return res;
	}

	template<typename T>
	void SetSetting(QString name, T val)
	{
		QFile setting(name);
		if (setting.open(QIODevice::WriteOnly))
			setting.write(QVariant(val).toString().toLatin1());
	}
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_timer(this)
{
	ui->setupUi(this);

	setWindowFlags(windowFlags() & ~Qt::WindowMinimizeButtonHint & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowCloseButtonHint);
	setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
#ifndef Q_OS_WIN
	setWindowFlags(windowFlags() | Qt::X11BypassWindowManagerHint);
#endif

	setWindowIcon(QIcon(":/icon.png"));

	m_timer.setSingleShot(true);
	m_timer.setInterval(ONE_MINUTE);

	connect(&m_timer, &QTimer::timeout, this, &MainWindow::checkNotice);
	connect(&m_manager, &QNetworkAccessManager::finished, [this](QNetworkReply *reply)
	{
		if(reply->error() == QNetworkReply::NoError)
		{
			QString replyString = reply->readAll();
			replyString = replyString.replace("response: ", "");
			QJsonDocument doc = QJsonDocument::fromJson(replyString.toUtf8());
			QJsonArray items = doc.array();

			unsigned int id = 0;
			Q_FOREACH(auto item, items)
			{
				QJsonObject obj = item.toObject();
				Message msg;
				msg.id   = obj.value("_id").toInt();
				msg.msg  = obj.value("message").toString();
				msg.date = QDate::fromString(obj.value("date").toString(), Qt::ISODate);

				auto* msgItem = new MessageItem(msg, this);
				ui->centralWidget->layout()->addWidget(msgItem);
				connect(msgItem, &MessageItem::closed, this, [this]()
				{
					auto* w = qobject_cast<QWidget*>(sender());
					ui->centralWidget->layout()->removeWidget(w);
					delete w;
					moveToCenter();
					this->close();
				});

				id = qMax(id, msg.id);
			}
			SetSetting(SETTING_ID, QString::number(id));
			m_id = id;
			show();
			moveToCenter();
			raise();
			activateWindow();
		}
		else
		{
			start();
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
	static bool first = true;
	if(first)
	{
		first = false;
		checkNotice();
		return;
	}

	m_timer.start();
}

void MainWindow::checkNotice()
{
	if(m_server.isEmpty() || !QUrl(m_server).isValid())
		m_server = GetSetting(SETTING_SERVER).toString();

	if(m_id == 0)
	{
		QVariant res = GetSetting(SETTING_ID);
		m_id = res.isValid() ? res.toInt() : m_id;
	}


	m_manager.get(QNetworkRequest(QUrl(m_server + QString::number(m_id))));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	event->ignore();

	if(ui->centralWidget->layout()->count() == 1)
	{
		start();
		hide();
	}
}

void MainWindow::moveToCenter()
{
	auto fixSize = [](QWidget* w){ w->resize(w->sizeHint()); w->adjustSize(); };
	QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
	fixSize(ui->centralWidget);
	fixSize(this);
	QApplication::processEvents(QEventLoop::AllEvents);

	setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
									size(), QApplication::desktop()->availableGeometry()));
}
