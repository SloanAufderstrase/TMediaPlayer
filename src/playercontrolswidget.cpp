#include <QToolButton>
#include <QHBoxLayout>

#include <VLCQtCore/MediaPlayer.h>
#include <VLCQtCore/Audio.h>
#include <QStyle>

#include "seekslider.h"
#include "playercontrolswidget.h"

const int defaultVolume = 50;
PlayerControlsWidget::PlayerControlsWidget(VlcMediaPlayer *player, QWidget *parent)
    : QFrame(parent),
      playerState(Vlc::Stopped),
      playButton(new QToolButton(this)),
      stopButton(new QToolButton(this)),
      nextButton(new QToolButton(this)),
      previousButton(new QToolButton(this)),
      muteButton(new QToolButton(this)),
      playListButton(new QToolButton(this)),
      openFileButton(new QToolButton(this)),
      toolboxButton(new QToolButton(this)),
      volumeSlider(new SeekSlider(Qt::Horizontal, this)),
      audio(new VlcAudio(player))
{
    init();
}

void PlayerControlsWidget::initAttributes()
{
    this->setFrameShadow(QFrame::Raised);
    this->setFrameShape(QFrame::Panel);
}


void PlayerControlsWidget::initStyle()
{
    this->setStyleSheet("QFrame { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,"
                             "stop:0 rgba(63, 46, 122, 255),"
                             "stop:1 rgba(171, 157, 219, 255)); }"
                            );

}

void PlayerControlsWidget::initAudio()
{
    if(audio){
        audio->setVolume(defaultVolume);
        audio->setParent(this);
    }
}

void PlayerControlsWidget::initButtons()
{
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    playButton->setStyleSheet("background-color::transparent");
    connect(playButton, SIGNAL(clicked()), this, SLOT(playClicked()));

    stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    stopButton->setStyleSheet("background-color::rgba(125,125,125,0)");
    stopButton->setEnabled(false);
    connect(stopButton, SIGNAL(clicked()), this, SIGNAL(stop()));

    nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    nextButton->setStyleSheet("background-color::transparent");
    connect(nextButton, SIGNAL(clicked()), this, SIGNAL(next()));

    previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    previousButton->setStyleSheet("background-color::transparent");
    connect(previousButton, SIGNAL(clicked()), this, SIGNAL(previous()));

    muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    muteButton->setStyleSheet("background-color::transparent");
    connect(muteButton, SIGNAL(clicked()), this, SLOT(muteClicked()));
}

void PlayerControlsWidget::initVolumeSlider()
{
    volumeSlider->setOrientation(Qt::Horizontal);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(defaultVolume);
    volumeSlider->setStyleSheet("background-color::transparent");
    connect(volumeSlider,SIGNAL(sliderMoved(int)),audio,SLOT(setVolume(int)));
    connect(volumeSlider,SIGNAL(seekSliderClicked(int)),audio,SLOT(setVolume(int)));
    connect(audio,SIGNAL(volumeChanged(int)),volumeSlider,SLOT(setValue(int)));
}


QHBoxLayout *PlayerControlsWidget::creatManageLayout()
{
    QHBoxLayout *manageLayout = new QHBoxLayout;
    playListButton->setIcon(QIcon("://images/playlist.png"));
    playListButton->setStyleSheet("background-color::transparent");
    playListButton->setCheckable(true);
    connect(playListButton, SIGNAL(toggled(bool)), this, SIGNAL(videoFillUp(bool)));

    openFileButton->setIcon(QIcon("://images/add.png"));
    openFileButton->setStyleSheet("background-color::transparent");
    connect(openFileButton, SIGNAL(clicked()), this, SIGNAL(openFileClicked()));

    toolboxButton->setIcon(QIcon("://images/toolbox.ico"));
    toolboxButton->setStyleSheet("background-color::transparent");

    manageLayout->addWidget(toolboxButton);
    manageLayout->addWidget(openFileButton);
    manageLayout->addWidget(playListButton);

    manageLayout->setSpacing(0);
    manageLayout->setContentsMargins(0,0,0,0);
    return manageLayout;
}

void PlayerControlsWidget::initLayout()
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->addStretch(1);
    layout->addWidget(stopButton);
    layout->addWidget(previousButton);
    layout->addWidget(playButton);
    layout->addWidget(nextButton);
    layout->addWidget(muteButton);
    layout->addWidget(volumeSlider);
    layout->addStretch(1);
    layout->addLayout(creatManageLayout());
    layout->setContentsMargins(6,6,6,6);
    layout->setSpacing(12);
    setLayout(layout);
}

void PlayerControlsWidget::init()
{
    initAttributes();
    initStyle();
    initAudio();
    initButtons();
    initVolumeSlider();
    initLayout();
}



void PlayerControlsWidget::playClicked()
{
    switch (playerState) {
    case Vlc::Idle:
    case Vlc::Opening:
    case Vlc::Buffering:
    case Vlc::Stopped:
    case Vlc::Ended:
    case Vlc::Error:
    case Vlc::Paused:
        emit play();
        break;
    case Vlc::Playing:
        emit pause();
        break;
    }
}

void PlayerControlsWidget::muteClicked()
{
    muteButton->setIcon(style()->standardIcon(!audio->getMute()
            ? QStyle::SP_MediaVolumeMuted
            : QStyle::SP_MediaVolume));
    audio->toggleMute();
}

Vlc::State PlayerControlsWidget::state() const
{
    return playerState;
}

void PlayerControlsWidget::setState(Vlc::State state)
{
    if (state != playerState) {
        playerState = state;

        switch (state) {
        case Vlc::Idle:
            break;
        case Vlc::Opening:
            break;
        case Vlc::Buffering:
            break;
        case Vlc::Stopped:
            stopButton->setEnabled(false);
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        case Vlc::Playing:
            stopButton->setEnabled(true);
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        case Vlc::Paused:
            stopButton->setEnabled(true);
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        case Vlc::Ended:
            break;
        case Vlc::Error:
            break;
        default:
            break;
        }
    }
}

int PlayerControlsWidget::volume() const
{
    return audio ? audio->volume() : 0;
}

void PlayerControlsWidget::setVolume(int volume)
{
    if (audio)
        audio->setVolume(volume);
}

bool PlayerControlsWidget::isMuted() const
{
    if(audio)
        return audio->getMute();
    return false;
}

