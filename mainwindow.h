/*
 The current running version is built on top of Qt 6.4.0
 The comments have expicitly referred to code that belong in other versions of Qt (Qt 5.15.2)
 Comment/uncomment relative code before testing and deployment

 @Authors:  Yuelin Xin, Ahmad Najmi bin Mohamad Shahir
 @file:     mainwindow.h
*/



#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QFileDialog>
#include <QPushButton>
#include <QAudioOutput>
#include <QMediaMetaData>
#include <QMediaPlaylist>

#include "range_slider.h"

//#include <QtAV>
//#include <QtAVWidgets>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_file_triggered();

    void on_actionPlay_triggered();

    void on_actionPause_triggered();

    void on_actionStop_triggered();

    void on_actionVolumn_triggered();

    void on_actionVolumn_Up_triggered();

    void on_actionVolumn_Down_triggered();

    void on_actionMute_triggered();

    void setTimeLabel();

    void on_actionForward_10s_triggered();

    void on_actionBackward_10s_triggered();

    void changeEvent(QEvent *event) override;

    void on_actionNext_triggered();

    void on_actionPrevious_triggered();

    void on_actionPlaylist_triggered();

    void setTitle();

    void switchPlayingMedia();

    void switchListSelection();

    void on_actionClear_Playlist_triggered();

    void on_actionLoop_triggered();

    void on_actionShuffle_triggered();

    void on_actionEdit_triggered();

    void on_actionTrim_Video_triggered();

    void on_actionSave_Trim_triggered();

private:
    Ui::MainWindow *ui;
    QMediaPlayer* player;
    QMediaPlaylist *play_list;
    RangeSlider *doubleSlider;
//    QAudioOutput* audio; // -- Qt6 --
//    QMediaMetaData* metadata;
};

#endif // MAINWINDOW_H
