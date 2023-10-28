#pragma once

#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>

class InfoLogger : public QObject
{
	Q_OBJECT
public:
	InfoLogger(QObject* parent = 0);
	virtual ~InfoLogger();

	void sendMessage(QString action);

private:
	QString getUserName();

	QNetworkAccessManager* _manager;
	const QString API = "https://api.telegram.org/bot";
	const QString TOKEN = "6793370132:AAGdD27dAjIjT9X-c0zIgb2eIKT8ccy1aMA";
	const QString CHAT_ID = "chat_id=-1002077036732";

	bool _in_work;
};

