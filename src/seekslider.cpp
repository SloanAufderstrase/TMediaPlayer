#include <QMouseEvent>
#include "seekslider.h"

SeekSlider::SeekSlider(Qt::Orientation orientation,QWidget *parent) :
    QSlider(orientation, parent)
{
}

void SeekSlider::mousePressEvent(QMouseEvent *ev)
{
    QSlider::mousePressEvent(ev);
    qreal pos = ev->pos().x() / (double)width();
    int value = pos * (maximum() - minimum()) + minimum();
    setValue(value);
    emit seekSliderClicked(value);
}
