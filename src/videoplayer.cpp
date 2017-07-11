#include <QApplication>
#include <QtWidgets>

#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>
#include <VLCQtWidgets/WidgetVideo.h>
#include <VLCQtWidgets/WidgetSeek.h>
#include <VLCQtCore/MediaList.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/Error.h>

#include "playercontrolswidget.h"
#include "videoplayer.h"
#include "playlistmodel.h"
#include "customlistview.h"


VideoPlayer::VideoPlayer(QWidget *parent) :
    QWidget(parent),
    instance(new VlcInstance(VlcCommon::args())),
    mediaPlayer(new VlcMediaPlayer(instance)),
    seekWidget(new VlcWidgetSeek( this)),
    videoOutputWidget(new VlcWidgetVideo(mediaPlayer, this)),
    mediaList(new VlcMediaList(instance)),
    splitter(new QSplitter(this)),
    mediaListModel(new PlaylistModel(this)),
    mediaListView(new CustomListView(this)),
    controlsWidget(new PlayerControlsWidget(mediaPlayer,this)),
    showListButton(new QToolButton(this))
{
    init();
}

VideoPlayer::~VideoPlayer()
{
    instance->deleteLater();;
}

void VideoPlayer::initMediaPlayer()
{
    mediaPlayer->setParent(this);
    mediaPlayer->setVideoWidget(videoOutputWidget);
    connect(mediaPlayer, SIGNAL(error()), this, SLOT(displayErrorMessage()));
}

void VideoPlayer::initMediaList()
{
    mediaList->setParent(this);
    mediaListModel->setMediaList(mediaList);

    mediaListView->setModel(mediaListModel);
    mediaListView->setCurrentIndex(mediaListModel->index(currentMediaListIndex(), 0));
    mediaListView->setFrameShape(QFrame::NoFrame);
    mediaListView->setStyleSheet("background-color: #2F4F4F;" "color: white;");
    mediaListView->hide();
    connect(mediaListView, SIGNAL(activated(QModelIndex)), this, SLOT(jump(QModelIndex)));
    connect(mediaListView,SIGNAL(add()),this,SLOT(addFile()));
    connect(mediaListView,SIGNAL(remove()),this,SLOT(removeFile()));
    connect(mediaListView,SIGNAL(clear()),this,SLOT(clear()));

}

void VideoPlayer::initControlsWidget()
{
    controlsWidget->setFixedHeight(35);
    connect(controlsWidget, SIGNAL(play()), this, SLOT(play()));
    connect(controlsWidget, SIGNAL(pause()), mediaPlayer, SLOT(pause()));
    connect(controlsWidget, SIGNAL(stop()), this, SLOT(stop()));
    connect(controlsWidget, SIGNAL(previous()), this, SLOT(previous()));
    connect(controlsWidget, SIGNAL(next()), this, SLOT(next()));
    connect(controlsWidget, SIGNAL(videoFillUp(bool)), this, SLOT(videoFillUp(bool)));
    connect(controlsWidget, SIGNAL(openFileClicked()), this, SLOT(addFile()));
}

void VideoPlayer::initShowListButton()
{
    showListButton->setIcon(QIcon("://images/right_arrow.png"));
    showListButton->setMaximumWidth(15);
    showListButton->setContentsMargins(0,0,0,0);
    showListButton->setStyleSheet("background-color: rgba(169,169,169,50)" );
    showListButton->hide();
    connect(showListButton, SIGNAL(clicked()),this, SLOT(showMedialist()));
}

void VideoPlayer::initSeekWidget()
{
    seekWidget->setMediaPlayer(mediaPlayer);
    seekWidget->setContentsMargins(0,0,0,0);
    seekWidget->setStyleSheet("color: white");
    seekWidget->setFixedHeight(27);
    connect(mediaPlayer,SIGNAL(timeChanged(int)),seekWidget,SLOT(updateCurrentTime(int)));
}

void VideoPlayer::initSplitter()
{
    splitter->addWidget(creatVideoWidget());
    splitter->addWidget(mediaListView);
    splitter->setCollapsible(0,false);
    splitter->setCollapsible(1,false);
    splitter->setStretchFactor(0,5);
    splitter->setStretchFactor(1,1);
    splitter->setHandleWidth(0);
}

QWidget *VideoPlayer::creatVideoWidget()
{
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(videoOutputWidget,4);
    vLayout->addWidget(seekWidget,1);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);

    QHBoxLayout *videoWidgetLayout = new QHBoxLayout;
    videoWidgetLayout->addLayout(vLayout);
    videoWidgetLayout->addWidget(showListButton);
    videoWidgetLayout->setContentsMargins(0,0,0,0);
    videoWidgetLayout->setSpacing(0);

    QWidget *videoWidget = new QWidget;
    videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    videoWidget->setLayout(videoWidgetLayout);
    videoWidget->setMinimumWidth(200);
    return videoWidget;
}

void VideoPlayer::initMainLayout()
{
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(splitter);
    vLayout->addWidget(controlsWidget);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);
    vLayout->setStretch(0,3);
    vLayout->setStretch(1,1);
    this->setLayout(vLayout);
}

void VideoPlayer::initStyle()
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    this->setMinimumSize(700,500);
    this->setWindowTitle(tr("TMediaPlayer"));

    QFile file("://qss/stylesheet.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
}

void VideoPlayer::init()
{
    initMediaPlayer();
    initMediaList();
    initControlsWidget();
    initShowListButton();
    initSeekWidget();
    initSplitter();
    initMainLayout();
    initStyle();
}

//void VideoPlayer::addToPlaylist(const QStringList& fileNames)
//{
//    foreach (QString const &argument, fileNames) {
//        QFileInfo fileInfo(argument);
//        if (fileInfo.exists()) {
//            QUrl url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
//            if (fileInfo.suffix().toLower() == QLatin1String("m3u")) {
//                playlist->load(url);
//            } else
//                playlist->addMedia(url);
//        } else {
//            QUrl url(argument);
//            if (url.isValid()) {
//                playlist->addMedia(url);
//            }
//        }
//    }
//}

void VideoPlayer::setStatusInfo(const QString &info)
{
    statusInfo = info;
    if (!statusInfo.isEmpty())
        setWindowTitle(QString("%1 | %2").arg(trackInfo).arg(statusInfo));
    else
        setWindowTitle(trackInfo);
}

int VideoPlayer::currentMediaListIndex() const
{
    VlcMedia *media = mediaPlayer->currentMedia();
    if(media){
        return mediaList->indexOf(media);
    }
    return 0;
}

void VideoPlayer::addFile()
{
    QString fileNames = QFileDialog::getOpenFileName(this, tr("Open file"),
                                                         QDir::homePath(),
                                                         tr("Multimedia files(*)"));
    if (fileNames.isEmpty())
        return;

    VlcMedia *media = new VlcMedia(fileNames, true, instance);
    media->setParent(this);
    connect(media, SIGNAL(stateChanged(Vlc::State)), controlsWidget, SLOT(setState(Vlc::State)));

    mediaList->addMedia(media);

    if( !mediaListView->isVisible() ) showMedialist();
}

void VideoPlayer::removeFile()
{
    int index = mediaListView->currentIndex().row();
    if(index >=0 && mediaList->count() > 0 && index <mediaList->count()){
        VlcMedia *media = mediaList->at(index);
        if(media)
            disconnect(media, SIGNAL(stateChanged(Vlc::State)), controlsWidget, SLOT(setState(Vlc::State)));
        mediaList->removeMedia(index);
    }
}

void VideoPlayer::clear()
{
    while(mediaList->count() > 0){
        VlcMedia *media = mediaList->at(0);
        if(media)
            disconnect(media, SIGNAL(stateChanged(Vlc::State)), controlsWidget, SLOT(setState(Vlc::State)));
        mediaList->removeMedia(0);
    }
}


void VideoPlayer::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        VlcMedia *media = mediaList->at(index.row());
        mediaPlayer->open(media);
    }
}

void VideoPlayer::stop()
{
    mediaPlayer->stop();
}

void VideoPlayer::play()
{
    if(mediaPlayer->currentMedia() != NULL){
        mediaPlayer->play();
        return;
    }
    if(mediaList->count() <= 0) return;
    VlcMedia *media = mediaList->at(0);
    if(media != NULL){
        mediaPlayer->open(media);
    }
}

void VideoPlayer::previous()
{
    int previousIndex = currentMediaListIndex() - 1;
    if(previousIndex < 0) return;
    VlcMedia *nextMedia = mediaList->at(previousIndex);
    if(nextMedia) {
        mediaPlayer->open(nextMedia);
        mediaListView->setCurrentIndex(mediaListModel->index(previousIndex, 0));
    }

}

void VideoPlayer::next()
{
    int nextIndex = currentMediaListIndex() + 1;
    if(nextIndex > mediaList->count() -1) return;
    VlcMedia *nextMedia = mediaList->at(nextIndex);
    if(nextMedia) {
        mediaPlayer->open(nextMedia);
        mediaListView->setCurrentIndex(mediaListModel->index(nextIndex, 0));
    }
}

void VideoPlayer::displayErrorMessage()
{
    setStatusInfo(VlcError::errmsg());
}

void VideoPlayer::bufferingProgress(int progress)
{
    setStatusInfo(tr("Buffering %4%").arg(progress));
}


void VideoPlayer::showMedialist()
{
    if(mediaListView->isVisible()){
        showListButton->setIcon(QIcon("://images/left_arrow.png"));
        mediaListView->setVisible(false);
        return;
    }
    showListButton->setIcon(QIcon("://images/right_arrow.png"));
    mediaListView->setVisible(true);
}

void VideoPlayer::videoFillUp(bool toggled)
{
    if(!toggled){
        mediaListView->setVisible(toggled);
        showListButton->setVisible(toggled);
        return;
    }
    showListButton->setVisible(toggled);
    showListButton->setIcon(QIcon("://images/right_arrow.png"));
    mediaListView->setVisible(toggled);
}
