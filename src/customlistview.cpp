#include <QtWidgets>
#include "customlistview.h"

CustomListView::CustomListView(QWidget *parent) : QListView(parent)
{

}

void CustomListView::contextMenuEvent(QContextMenuEvent *event)
{
    QListView::contextMenuEvent(event);
    QMenu menu;
    menu.addAction(tr("Add..."),this, SIGNAL(add()),QKeySequence());
    menu.addAction(tr("Remove"),this, SIGNAL(remove()),QKeySequence());
    menu.addAction(tr("Clear"),this, SIGNAL(clear()),QKeySequence());
    menu.setStyleSheet("background-color: #2F4F4F;" "color: white;");

    menu.exec(QCursor::pos());
}
