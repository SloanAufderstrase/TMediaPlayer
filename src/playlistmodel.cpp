#include "playlistmodel.h"

#include "playlistmodel.h"

#include <QFileInfo>
#include <QUrl>
#include <VLCQtCore/MediaList.h>
#include <VLCQtCore/Media.h>


PlaylistModel::PlaylistModel(QObject *parent)
    : QAbstractItemModel(parent)
    , mMediaList(0)
{
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    return mMediaList && !parent.isValid() ? mMediaList->count() : 0;
}

int PlaylistModel::columnCount(const QModelIndex &parent) const
{
    return !parent.isValid() ? ColumnCount : 0;
}

QModelIndex PlaylistModel::index(int row, int column, const QModelIndex &parent) const
{
    return mMediaList && !parent.isValid()
            && row >= 0 && row < mMediaList->count()
            && column >= 0 && column < ColumnCount
        ? createIndex(row, column)
        : QModelIndex();
}

QModelIndex PlaylistModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);

    return QModelIndex();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole) {
        QVariant value = m_data[index];
        if (!value.isValid() && index.column() == Title) {
           QFileInfo fileInfo( mMediaList->at(index.row())->currentLocation() );
           return  fileInfo.fileName();
        }
        return value;
    }
    return QVariant();
}

VlcMediaList *PlaylistModel::getMediaList() const
{
    return mMediaList;
}

void PlaylistModel::setMediaList(VlcMediaList *mediaList)
{
    if (mMediaList) {
        disconnect(mMediaList, SIGNAL(willAddItem(libvlc_media_t*,int)), this, SLOT(updateModel()));
        disconnect(mMediaList, SIGNAL(itemAdded(libvlc_media_t*,int)), this, SLOT(updateModel()));
        disconnect(mMediaList, SIGNAL(willDeleteItem(libvlc_media_t*,int)), this, SLOT(updateModel()));
        disconnect(mMediaList, SIGNAL(itemDeleted(libvlc_media_t*,int)), this, SLOT(updateModel()));
    }
    beginResetModel();
    mMediaList = mediaList;

    if (mMediaList) {
        connect(mMediaList, SIGNAL(willAddItem(libvlc_media_t*,int)), this, SLOT(updateModel()));
        connect(mMediaList, SIGNAL(itemAdded(libvlc_media_t*,int)), this, SLOT(updateModel()));
        connect(mMediaList, SIGNAL(willDeleteItem(libvlc_media_t*,int)), this, SLOT(updateModel()));
        connect(mMediaList, SIGNAL(itemDeleted(libvlc_media_t*,int)), this, SLOT(updateModel()));
    }
    endResetModel();
}

bool PlaylistModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role);
    m_data[index] = value;
    emit dataChanged(index, index);
    return true;
}

void PlaylistModel::changeItems(int index)
{
    m_data.clear();
    emit dataChanged(this->index(index,0), this->index(index,ColumnCount));
}

void PlaylistModel::updateModel()
{
    beginResetModel();
    endResetModel();
}
