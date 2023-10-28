#include "InfoLogger.h"

#include <QDir>
#include <QTime>

InfoLogger::InfoLogger(QObject* parent)
	: QObject(parent)
{
	_manager = new QNetworkAccessManager();
}

InfoLogger::~InfoLogger()
{
	delete _manager;
}

void InfoLogger::sendMessage(QString action)
{
	const QString COMMAND = "/sendMessage?";

	QNetworkRequest request;
	QString message;
	QString time = getTime();
	QString userName = getUserName();

	QString userTimeMessage = QString::fromUtf8(u8"Время: ") + time + '\n';
	QString userNameMessage = QString::fromUtf8(u8"Пользователь: ") + userName + '\n';
	QString userActionMessage = QString::fromUtf8(u8"Действие: ") + action + '\n';

	message.append(userTimeMessage);
	message.append(userNameMessage);
	message.append(userActionMessage);

	QUrl url = API + TOKEN + COMMAND + CHAT_ID + "&text=" + message;
	request.setUrl(url);
	_manager->get(request);

	connect(_manager, &QNetworkAccessManager::finished,
		[=](QNetworkReply* reply) {
			if (reply->error()) {
				qDebug() << reply->error();
				return;
			}
		});

	qDebug() << url;
}

QString InfoLogger::getTime()
{
	return QTime().toString();
}

QString InfoLogger::getUserName()
{
	return QString::fromUtf8(QDir::home().dirName().toUtf8());
}
