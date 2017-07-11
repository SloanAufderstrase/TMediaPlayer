#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractItemModel>
class VlcMediaList;
struct libvlc_media_t;

class PlaylistModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Column
    {
        Title = 0,
        ColumnCount
    };
    explicit PlaylistModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    VlcMediaList *getMediaList() const;
    void setMediaList(VlcMediaList *mediaList);

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole);

private slots:
    void changeItems(int index);
    void updateModel();

private:
    VlcMediaList *mMediaList;
    QMap<QModelIndex, QVariant> m_data;

};

#endif // PLAYLISTMODEL_H
