#pragma once

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "WordUrlParser.h"

class FileDownloader : public QObject
{
	Q_OBJECT
public:
	explicit FileDownloader(const QString& audioPath, QObject* parent = 0);
	virtual ~FileDownloader();

signals:
	void onReady();
	void NetError();
	void setConvertUrl(const QString& word);

public slots:
	void saveData(const QString& url);
	void onResult(QNetworkReply* reply);
	void setFileName(const QString& word);

private:
	QNetworkAccessManager* _menager;
	WordUrlParser* _parser;

	QString _fileName;
	QString _AUDIO_PATH;
};

