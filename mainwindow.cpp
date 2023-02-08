/*
 The current running version is built on top of Qt 6.4.0
 The comments have expicitly referred to code that belong in other versions of Qt (Qt 5.15.2)
 Comment/uncomment relative code before testing and deployment

 @Authors:  Yuelin Xin, Ahmad Najmi bin Mohamad Shahir
 @file:     mainwindow.cpp
*/



#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // new Qt media player used to play/control the media stream
    player = new QMediaPlayer(this);
    play_list = new QMediaPlaylist(this);
//    audio = new QAudioOutput(this); // -- Qt6 --
//    metadata = new QMediaMetaData(); // -- Qt6 --

    doubleSlider = new RangeSlider(Qt::Horizontal);
    ui->horizontalLayout_13->addWidget(doubleSlider);
//    QFile file(":/qss/slider_style.qss");
//    file.open(QFile::ReadOnly);
//    QString styleSheet = QLatin1String(file.readAll());
//    doubleSlider->setStyleSheet(styleSheet);

    // set video and video outputs
    player->setVideoOutput(ui->vw);
//    player->setAudioOutput(audio); // -- Qt6 --

    // connect signals and slots
    connect(player, &QMediaPlayer::durationChanged, ui->slider, &QSlider::setMaximum);
    connect(player, &QMediaPlayer::positionChanged, ui->slider, &QSlider::setValue);
    connect(ui->slider, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);
    connect(player, &QMediaPlayer::volumeChanged, ui->slider_vol, &QSlider::setValue);
    connect(ui->slider_vol, &QSlider::sliderMoved, player, &QMediaPlayer::setVolume);
    connect(ui->play, &QPushButton::clicked, this, &MainWindow::on_actionPlay_triggered);
    connect(ui->stop, &QPushButton::clicked, this, &MainWindow::on_actionStop_triggered);
    connect(ui->next, &QPushButton::clicked, this, &MainWindow::on_actionNext_triggered);
    connect(ui->previous, &QPushButton::clicked, this, &MainWindow::on_actionPrevious_triggered);
    connect(ui->openfile, &QPushButton::clicked, this, &MainWindow::on_actionOpen_file_triggered);
    connect(ui->logo, &QPushButton::clicked, this, &MainWindow::on_actionOpen_file_triggered);
    connect(ui->volume, &QPushButton::clicked, this, &MainWindow::on_actionMute_triggered);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::setTimeLabel);
    connect(ui->list, &QPushButton::clicked, this, &MainWindow::on_actionPlaylist_triggered);
    connect(play_list, &QMediaPlaylist::currentMediaChanged, this, &MainWindow::setTitle);
    // connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::switchPlayingMedia);
    connect(ui->listWidget, &QListWidget::currentRowChanged, play_list, &QMediaPlaylist::setCurrentIndex);
    connect(play_list, &QMediaPlaylist::currentIndexChanged, this, &MainWindow::switchListSelection);
    connect(ui->clear_playlist, &QPushButton::clicked, this, &MainWindow::on_actionClear_Playlist_triggered);
    connect(ui->loop, &QPushButton::clicked, this, &MainWindow::on_actionLoop_triggered);
    connect(ui->shuffle, &QPushButton::clicked, this, &MainWindow::on_actionShuffle_triggered);
    connect(ui->edit, &QPushButton::clicked, this, &MainWindow::on_actionEdit_triggered);
    // connect(ui->vw, &QVideoWidget::dropEvent, this, &MainWindow::on_actionOpen_file_triggered);

    // trim connect
    // connect(player, &QMediaPlayer::positionChanged, doubleSlider, &RangeSlider::setValue);
    // connect(doubleSlider, &RangeSlider::sliderMoved, player, &QMediaPlayer::setPosition);
    connect(ui->cancel_button, &QPushButton::clicked, this, &MainWindow::on_actionTrim_Video_triggered);
    connect(ui->trim_button, &QPushButton::clicked, this, &MainWindow::on_actionSave_Trim_triggered);

    // additional ui settings
    ui->stackedWidget->setCurrentIndex(1); // set the stacked widget to home page
    this->setAcceptDrops(true);
    ui->playlist->hide();
    ui->slider->setEnabled(false);
    play_list->setPlaybackMode(QMediaPlaylist::Sequential);
    ui->frame_edit->hide();
    // ui->stop->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete player;
    delete play_list;
    delete doubleSlider;
    //    delete audio; // -- Qt6 --
    //    delete metadata; // -- Qt6 --
}





/* Window Slots */
void MainWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::WindowStateChange) {
        if (this->windowState() == Qt::WindowFullScreen) {
            ui->frame->hide();
            if (!ui->playlist->isHidden()) {
                on_actionPlaylist_triggered();
            }
        } else {
            ui->frame->show();
        }
    }
}

void MainWindow::setTitle()
{
    qint64 index = play_list->currentIndex();
    if (index == -1) {
        on_actionStop_triggered();
        return;
    }
    QString fileName = ui->listWidget->item(index)->text();
    this->setWindowTitle(fileName);
}




/* File Slots */
void MainWindow::on_actionOpen_file_triggered()
{
    QStringList fileName = QFileDialog::getOpenFileNames(this, "Browse media", "",
                                                         "Video File (*.mov, *.avi, *.mp4)");
    if(fileName.length() == 0){return;}
    ui->stackedWidget->setCurrentIndex(0);
    ui->slider->setEnabled(true);
    for (int i = 0; i < fileName.length(); i++)
    {
        play_list->addMedia(QUrl::fromLocalFile(fileName.at(i)));
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(fileName.at(i).split("/").last());
        ui->listWidget->addItem(item);
    }
    player->setPlaylist(play_list);
//    player->setSource(QUrl::fromLocalFile(fileName)); // -- Qt6 --
    player->play();
    ui->play->setIcon(QIcon(":/icons/pause-fill.svg"));
//    QMediaMetaData a = player->metaData();
}





/* Playback Slots */
void MainWindow::on_actionPlay_triggered()
{
    if(player->state() == 1){ // -- Qt5 --
//    if(player->playbackState() == 1){ // -- Qt6 --
        // the media is playing
        player->pause();
        ui->play->setIcon(QIcon(":/icons/play-fill.svg"));
    }
    else if(player->state() == 2){ // -- Qt5 --
//    else if(player->playbackState() == 2){ // -- Qt6 --
        // the media is paused
        player->play();
        ui->play->setIcon(QIcon(":/icons/pause-fill.svg"));
    }
    else{
        // the media is stopped
        if(player->state() == 0 && play_list->isEmpty()){
            on_actionOpen_file_triggered();
        } else{
            ui->stackedWidget->setCurrentIndex(0);
            player->play();
            ui->play->setIcon(QIcon(":/icons/pause-fill.svg"));
        }
    }
}

void MainWindow::on_actionPause_triggered(){return;}

void MainWindow::on_actionStop_triggered()
{
    player->stop();
    ui->play->setIcon(QIcon(":/icons/play-fill.svg"));
    ui->slider->setSliderPosition(0);
    ui->stackedWidget->setCurrentIndex(1);
    ui->time->setText("00:00");
    ui->slider->setEnabled(false);
    this->setWindowTitle("CV Pro Player");
}

void MainWindow::on_actionForward_10s_triggered()
{
    qint64 cur_position = player->position();
    player->setPosition(cur_position + 10000);
}


void MainWindow::on_actionBackward_10s_triggered()
{
    qint64 cur_position = player->position();
    player->setPosition(cur_position - 10000);
}

void MainWindow::setTimeLabel()
{
    // set text in clock format
    int position = player->position()/1000;
    int sec = position % 60;
    int min = position / 60;
    QString sec_text = QString::number(sec).rightJustified(2, '0');
    QString min_text = QString::number(min).rightJustified(2, '0');
    ui->time->setText(min_text + ":" + sec_text);
}

void MainWindow::on_actionNext_triggered()
{
    if (play_list->isEmpty()) {
        return;
    }
    if (play_list->currentIndex()+1 == play_list->mediaCount() && play_list->playbackMode() == 2) {
        on_actionStop_triggered();
    }
    play_list->next();
}

void MainWindow::on_actionPrevious_triggered()
{
    if (play_list->isEmpty()) {
        return;
    }
    if (play_list->currentIndex() == 0) {
        return;
    }
    play_list->previous();
}

void MainWindow::on_actionPlaylist_triggered()
{
    if (ui->playlist->isHidden()) {
        ui->playlist->show();
    }
    else {
        ui->playlist->hide();
    }
}

void MainWindow::switchPlayingMedia()
{
    play_list->setCurrentIndex(ui->listWidget->currentRow());
}

void MainWindow::switchListSelection()
{
    ui->listWidget->setCurrentRow(play_list->currentIndex());
}

void MainWindow::on_actionClear_Playlist_triggered()
{
    on_actionStop_triggered();
    ui->listWidget->clear();
    play_list->clear();
}

void MainWindow::on_actionLoop_triggered()
{
    play_list->setPlaybackMode(QMediaPlaylist::Loop);
    ui->loop->setStyleSheet("background: rgba(143,143,143,60); border-radius: 4px;");
    ui->shuffle->setStyleSheet("background: rgba(0,0,0,0); border-radius: 4px;");
}

void MainWindow::on_actionShuffle_triggered()
{
    play_list->setPlaybackMode(QMediaPlaylist::Random);
    ui->shuffle->setStyleSheet("background: rgba(143,143,143,60); border-radius: 4px;");
    ui->loop->setStyleSheet("background: rgba(0,0,0,0); border-radius: 4px;");
}







/* Audio Slots */
void MainWindow::on_actionVolumn_triggered(){return;}

void MainWindow::on_actionVolumn_Up_triggered()
{
    // -- Qt5 --
     int volumn = player->volume();
     if(volumn < 100){
         volumn += 10;
         player->setVolume(volumn);
     }

    // -- Qt6 --
//    float volumn = audio->volume();
//    if(volumn < 1){
//        volumn += 0.1;
//        audio->setVolume(volumn);
//    }
}

void MainWindow::on_actionVolumn_Down_triggered()
{
    // -- Qt5 --
     int volumn = player->volume();
     if(volumn > 0){
         volumn -= 10;
         player->setVolume(volumn);
     }

    // -- Qt6 --
//    float volumn = audio->volume();
//    if(volumn > 0){
//        volumn -= 0.1;
//        audio->setVolume(volumn);
//    }
}

void MainWindow::on_actionMute_triggered()
{
    // -- Qt6 --
//    if(audio->isMuted()){
//        audio->setMuted(false);
//    }
//    else{
//        audio->setMuted(true);
//    }

    // -- Qt5--
    if (player->isMuted()) {
        player->setMuted(false);
        ui->volume->setIcon(QIcon(":/icons/volume-up-fill.svg"));
        ui->slider_vol->setEnabled(true);
    }
    else {
        player->setMuted(true);
        ui->volume->setIcon(QIcon(":/icons/volume-mute-fill.svg"));
        ui->slider_vol->setEnabled(false);
    }
}






/* Editor Slots */
void MainWindow::on_actionEdit_triggered()
{
    ui->playlist->show();
    if (ui->frame_edit->isHidden()) {
        ui->frame_edit->show();
    }
    else {
        ui->frame_edit->hide();
    }
}

void MainWindow::on_actionTrim_Video_triggered()
{
    on_actionEdit_triggered();
    if (ui->stackedWidget->currentIndex() == 2) {
        player->setVideoOutput(ui->vw);
        ui->stackedWidget->setCurrentIndex(0);
        ui->frame->show();
    }
    else {
        player->setVideoOutput(ui->vw_edit);
        ui->stackedWidget->setCurrentIndex(2);
        ui->frame->hide();
    }
}

void MainWindow::on_actionSave_Trim_triggered()
{
    QString fileName = player->currentMedia().QMediaContent::request().url().toString().mid(8);
    QStringList fileNameShort = fileName.split(".mp4");
    QString name = fileNameShort.value(fileNameShort.length() - 2);
    QFile::copy(fileName, name + "_trimmed.mp4");
    on_actionTrim_Video_triggered();
}

