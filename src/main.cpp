#include <QApplication>
#include "videoplayer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    VideoPlayer videoPlayer;
    videoPlayer.show();
    return app.exec();
}
