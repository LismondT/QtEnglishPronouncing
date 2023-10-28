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
	QString getTime();
	QString getUserName();

	QNetworkAccessManager* _manager;
	const QString API = "https://api.telegram.org/bot";
	const QString TOKEN = "6450516858:AAH2u1JJu8wsiqe7oVwnp_z6MD9UMyx6JXM";
	const QString CHAT_ID = "chat_id=-1001576353074";
};

