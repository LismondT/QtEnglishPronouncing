#include "QtEnglishPronouncing.h"
#include <qDebug>
#include <QDir>
#include <QFileInfoList>

//ToDo delete this
#include <qstandardpaths.h>


QtEnglishPronouncing::QtEnglishPronouncing(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //SoundsInit
    _mediaPLayer = new QMediaPlayer(this);

    //Logger
    _logger = new InfoLogger();

    //FilesDownloader
    _downloader = new FileDownloader(AUDIO_PATH);
    connect(_downloader, &FileDownloader::onReady, this, &QtEnglishPronouncing::AudioLoaded);
    connect(this, &QtEnglishPronouncing::setFileName, _downloader, &FileDownloader::setFileName);
    connect(_downloader, &FileDownloader::NetError, this, &QtEnglishPronouncing::NetErrorHandler);

    //Buttons
    connect(ui.pronounceButton, &QPushButton::clicked, this, &QtEnglishPronouncing::playAudioPushButton);
    connect(ui.getAudioButton, &QPushButton::clicked, this, &QtEnglishPronouncing::getAudioPushButton);
    connect(ui.deleteAllButton, &QPushButton::clicked, this, &QtEnglishPronouncing::WordsListDeleteAll);
    connect(ui.deleteOneButton, &QPushButton::clicked, this, &QtEnglishPronouncing::WordsListDeleteOne);

    //UtilsF
    ui.wordsList->setSortingEnabled(false);
    this->WordsListUpdate();
    this->FolderDepend();

    _logger->sendMessage(QString::fromUtf8(u8"Зашел в приложение"));
    _left_smile = true;
}

QtEnglishPronouncing::~QtEnglishPronouncing()
{
    delete _logger;
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

void QtEnglishPronouncing::WordsListDeleteAll()
{
    qDebug("WordsListDeleteAll is clicked");

    QDir dir(AUDIO_PATH);
    dir.setFilter(QDir::Files);

    foreach(QString dirFile, dir.entryList())
    {
        dir.remove(dirFile);
    }

    this->WordsListUpdate();

    _logger->sendMessage(QString::fromUtf8(u8"Удалил все слова"));
}

void QtEnglishPronouncing::WordsListDeleteOne()
{
    qDebug("WordsListDeleteOne is clicked");

    if (!WordsListIsChoosed())
        return;

    QDir dir(AUDIO_PATH);
    QString item = ui.wordsList->currentItem()->text();

    dir.setFilter(QDir::Files);
    dir.remove(item + ".mp3");

    this->WordsListUpdate();

    _logger->sendMessage(QString::fromUtf8(u8"Удалил слово {") + item + "}");
}

void QtEnglishPronouncing::getAudioPushButton()
{
    QString word = ui.wordEdit->text();
    this->FolderDepend();
    ui.wordEdit->setText("");
    emit setFileName(word);

    _logger->sendMessage(QString::fromUtf8(u8"Получил слово {") + word + "}");
}


void QtEnglishPronouncing::AudioLoaded()
{
    this->WordsListUpdate();
    ui.info->setText((""));

    qDebug() << "Audio was loaded";
}

void QtEnglishPronouncing::NetErrorHandler()
{
    if (_left_smile) {
        ui.info->setText(QString::fromUtf8(u8"Слово не найдено (:"));
        _left_smile = false;
    }
    else {
        ui.info->setText(QString::fromUtf8(u8"Слово не найдено :)"));
        _left_smile = true;
    }

    _logger->sendMessage(QString::fromUtf8(u8"Получил ошибку"));
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
