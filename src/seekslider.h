#ifndef SEEKSLIDER_H
#define SEEKSLIDER_H

#include <QSlider>

class SeekSlider : public QSlider
{
    Q_OBJECT
public:
    explicit SeekSlider(Qt::Orientation orientation, QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *ev);
signals:
    void seekSliderClicked(int positon);

};

#endif // SEEKSLIDER_H
