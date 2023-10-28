#include "FileDownloader.h"

#include <qfile.h>

FileDownloader::FileDownloader(const QString& audioPath, QObject* parent) :
	QObject(parent)
{
	_menager = new QNetworkAccessManager();
	_parser = new WordUrlParser(_menager);

	_AUDIO_PATH = audioPath;

	connect(_menager, &QNetworkAccessManager::finished, this, &FileDownloader::onResult);

	connect(this, &FileDownloader::setConvertUrl, _parser, &WordUrlParser::getData);
	connect(_parser, &WordUrlParser::onReady, this, &FileDownloader::saveData);
}

FileDownloader::~FileDownloader() {
	delete _parser;
	delete _menager;
}


void FileDownloader::saveData(const QString& url)
{
	QNetworkRequest request;
	request.setUrl(QUrl(url));
	_menager->get(request);

	qDebug() << "Url in getData(FileDown): " << QUrl(url);
}

void FileDownloader::onResult(QNetworkReply* reply)
{	
	if (reply->error()) {
		qDebug() << "ERROR";
		qDebug() << reply->errorString();
		qDebug() << "Reply";
		qDebug() << reply->readAll();
		emit NetError();
		return;
	}

	if (_fileName == nullptr) {
		qDebug() << "FileName is not exist";
		return;
	}

	QFile* file = new QFile(_AUDIO_PATH + _fileName + ".mp3");

	if (!file->open(QIODevice::WriteOnly)) {
		qDebug() << "File is not open";
		return;
	}
	
	file->write(reply->readAll());
	file->close();

	_fileName = nullptr;

	qDebug() << "File is Downloaded";

	emit onReady();
}

void FileDownloader::setFileName(const QString& word)
{
	_fileName = word;
	emit setConvertUrl(word);
}
