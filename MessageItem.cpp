#include "MessageItem.h"
#include "ui_MessageItem.h"

#include <QCloseEvent>

MessageItem::MessageItem(const Message& msg, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MessageItem)
{
	ui->setupUi(this);

	ui->headerBrowser->setText(msg.date.toString() + " #" + QString::number(msg.id));
	ui->messageBrowser->setText(msg.msg);
	connect(ui->closeButton, &QPushButton::clicked, this, &QWidget::close);
}

MessageItem::~MessageItem()
{
	delete ui;
}

void MessageItem::closeEvent(QCloseEvent *event)
{
	event->accept();
	emit closed();
}
