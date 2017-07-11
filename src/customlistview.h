#ifndef CUSTOMLISTVIEW_H
#define CUSTOMLISTVIEW_H
#include <QListView>

class CustomListView : public QListView
{
    Q_OBJECT
public:
    CustomListView(QWidget *parent = 0);
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void add();
    void remove();
    void clear();
};

#endif // CUSTOMLISTVIEW_H
