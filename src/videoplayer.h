#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H
class QListView;
class QSlider;
class QSplitter;
class QToolButton;

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;
class VlcWidgetSeek;
class VlcWidgetVideo;
class VlcMediaList;

class PlayerControlsWidget;
class PlaylistModel;
class CustomListView;

#include <QWidget>

class VideoPlayer : public QWidget
{
    Q_OBJECT
public:
    explicit VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();
private slots:
    void addFile();
    void removeFile();
    void clear();
    void jump(const QModelIndex &index);
    void stop();
    void play();
    void previous();
    void next();
    void displayErrorMessage();
    void bufferingProgress(int progress);
    void showMedialist();//the widget that contains play list and button
    void videoFillUp(bool toggled);

private:
    void init();
    void initMediaPlayer();
    void initMediaList();
    void initControlsWidget();
    void initShowListButton();
    void initSeekWidget();
    void initSplitter();
    QWidget *creatVideoWidget();
    void initMainLayout();
    void initStyle();

//    void addToPlaylist(const QStringList &fileNames);
    void setStatusInfo(const QString &info);
    int currentMediaListIndex() const;

    VlcInstance *instance;
    VlcMediaPlayer *mediaPlayer;
    VlcWidgetSeek *seekWidget;
    VlcWidgetVideo *videoOutputWidget;
    VlcMediaList *mediaList;

    QSplitter *splitter;
    PlaylistModel *mediaListModel;
    CustomListView *mediaListView;
    PlayerControlsWidget *controlsWidget;
    QToolButton *showListButton;

    QString trackInfo;
    QString statusInfo;



};

#endif // VIDEOPLAYER_H
