#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtEnglishPronouncing.h"

#include <qbuffer.h>
#include <qmediaplayer.h>
#include <qmediaplaylist.h>
#include <qfile.h>
#include <QKeyEvent>

#include "FileDownloader.h"
#include "InfoLogger.h"

class QtEnglishPronouncing : public QMainWindow
{
    Q_OBJECT

public:
    QtEnglishPronouncing(QWidget *parent = nullptr);
    ~QtEnglishPronouncing();

    void WordsListUpdate();
    bool WordsListIsChoosed();
    
    void FolderDepend();

signals:
    //set name that was saved (FileDownloader::getData)
    void setFileName(const QString& name);

public slots:
    //Audios
    void playAudioPushButton();

    //Net
    void getAudioPushButton(); //Download *.mp3 file
    void AudioLoaded();
    void NetErrorHandler();

    //WordsList
    void WordsListDeleteAll();
    void WordsListDeleteOne();

private:
    Ui::QtEnglishPronouncingClass ui;

    QMediaPlayer* _mediaPLayer;
    QMediaPlaylist* _mediaPlaylist;

    FileDownloader* _downloader;
    InfoLogger* _logger;

    
    const QString AUDIO_PATH = "./audios/";
    bool _left_smile;
};
