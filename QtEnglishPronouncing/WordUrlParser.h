#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class WordUrlParser : public QObject
{
	Q_OBJECT
public:
	explicit WordUrlParser(QNetworkAccessManager* menager, QObject* parent = 0);
	virtual ~WordUrlParser();

	QString getWordUrl(const QString& word);

signals:
	void onReady(const QString& url);

public slots:
	void getData(const QString& word);
	void onResult(QNetworkReply* reply);

private:
	QNetworkAccessManager* _menager;
	const QString _URL_PATH = "https://dictionary.cambridge.org/ru/%D1%81%D0%BB%D0%BE%D0%B2%D0%B0%D1%80%D1%8C/%D0%B0%D0%BD%D0%B3%D0%BB%D0%BE-%D1%80%D1%83%D1%81%D1%81%D0%BA%D0%B8%D0%B9/";
	const QString _URL_OUT_PATH = "https://dictionary.cambridge.org/";
};

