#pragma once

#include <QWidget>
#include <QDate>

struct Message {
	QString msg;
	QDate date;
	unsigned int id = 0;
};

namespace Ui {
class MessageItem;
}

class MessageItem : public QWidget
{
	Q_OBJECT

public:
	explicit MessageItem(const Message& msg, QWidget *parent = 0);
	~MessageItem();

signals:
	void closed();

private:
	void closeEvent(QCloseEvent *event);

private:
	Ui::MessageItem *ui;
};
