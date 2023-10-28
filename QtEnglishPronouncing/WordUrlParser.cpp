#include "WordUrlParser.h"

#include <qDebug>
#include <QXmlStreamReader>

WordUrlParser::WordUrlParser(QNetworkAccessManager* menager, QObject* parent)
	: QObject(parent)
{
	_menager = new QNetworkAccessManager();//ToDo menager;

	connect(_menager, &QNetworkAccessManager::finished, this, &WordUrlParser::onResult);
}

WordUrlParser::~WordUrlParser()
{
	delete _menager;
}

QString WordUrlParser::getWordUrl(const QString& html)
{
	//ToDo какого хкй€ бл€ть он не работает и не работает
	QString outUrl = _URL_OUT_PATH;

	int sposAudio2 = html.indexOf("src=\"/ru/media/%D0%B0%D0%BD%D0%B3%D0%BB%D0%BE-%D1%80%D1%83%D1%81%D1%81%D0%BA%D0%B8%D0%B9/us_pron/") + 5;
	int eposAudio2 = html.indexOf("<source type=\"audio/ogg\" src=\"/ru/media/%D0%B0%D0%BD%D0%B3%D0%BB%D0%BE-%D1%80%D1%83%D1%81%D1%81%D0%BA%D0%B8%D0%B9/us_pron_ogg/") - 8;

	outUrl += html.mid(sposAudio2, eposAudio2 - sposAudio2);

	if (outUrl == _URL_OUT_PATH) {
		return NULL;
	}

	qDebug() << "OUT_URL: " << outUrl;
	return outUrl;
}

void WordUrlParser::getData(const QString& word)
{
	QUrl url = _URL_PATH + word;
	QNetworkRequest request;
	request.setUrl(url);
	_menager->get(request);

	qDebug() << "Url in WordUrlParser: " << url;
}

void WordUrlParser::onResult(QNetworkReply* reply)
{
	if (reply->error()) {
		//ToDO 
		return;
	}

	QString page = reply->readAll();
	QString url(getWordUrl(page));
	emit onReady(url);


	qDebug() << "Page in onResult:" << url;
}

