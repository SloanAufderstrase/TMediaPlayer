TEMPLATE = app
TARGET = TMediaPlayer
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_ICONS = $$PWD/Icons/TMediaPlayer.ico

#DESTDIR = $$PWD/bin/
QT += core gui
QT += network \
      xml \
      widgets

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/videoplayer.cpp \
    $$PWD/src/playlistmodel.cpp \
    $$PWD/src/playercontrolswidget.cpp \
    $$PWD/src/seekslider.cpp \
    $$PWD/src/customlistview.cpp

HEADERS += \
    $$PWD/src/videoplayer.h \
    $$PWD/src/playlistmodel.h \
    $$PWD/src/playercontrolswidget.h \
    $$PWD/src/seekslider.h \
    $$PWD/src/customlistview.h

RESOURCES += \
    mediaplayer.qrc



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/vlc/lib/ -llibVLCQtCore.dll -llibVLCQtWidgets.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/vlc/lib/ -llibVLCQtCored.dll -llibVLCQtWidgetsd.dll

INCLUDEPATH += $$PWD/vlc/include
DEPENDPATH += $$PWD/vlc/include
