#ifndef PLAYERCONTROLSWIDGET_H
#define PLAYERCONTROLSWIDGET_H
#include <QFrame>
#include <VLCQtCore/Enums.h>
class QAbstractButton;
class QHBoxLayout;
class VlcMediaPlayer;
class VlcAudio;
class SeekSlider;

class PlayerControlsWidget : public QFrame
{
    Q_OBJECT
public:
    PlayerControlsWidget(VlcMediaPlayer *player, QWidget *parent = 0);
    bool isMuted() const;
    int volume() const;

    void initAttributes();

    void initVolumeSlider();

signals:
    void play();
    void pause();
    void stop();
    void next();
    void previous();
    void changeVolume(int volume);
    void changeMuting(bool muting);
    void videoFillUp(bool toggled);
    void openFileClicked();

public slots:
    void setState(Vlc::State state);
    void playClicked();
    void muteClicked();
    void setVolume(int volume);

private:
    void init();
    void initLayout();
    QHBoxLayout *creatManageLayout();
    void initStyle();
    Vlc::State state() const;

    Vlc::State playerState;
    QAbstractButton *playButton;
    QAbstractButton *stopButton;
    QAbstractButton *nextButton;
    QAbstractButton *previousButton;
    QAbstractButton *muteButton;
    QAbstractButton *playListButton;
    QAbstractButton *openFileButton;
    QAbstractButton *toolboxButton;
    SeekSlider *volumeSlider;
    VlcAudio *audio;

    void initButtons();
    void initAudio();
};

#endif // PLAYERCONTROLSWIDGET_H
