#ifndef GFILELISTVIEW_H
#define GFILELISTVIEW_H

#include <QListView>
#include <QStyledItemDelegate>

#define SHOW_DATA_ROLE Qt::UserRole + 1

typedef struct
{
    QString iconPath;
    QString songName;
    QString songAbsPath;
} ItemShowData;
Q_DECLARE_METATYPE(ItemShowData)

class GFileListView : public QListView
{
    Q_OBJECT
public:
    GFileListView(QWidget *parent = nullptr);
    virtual ~GFileListView();
};

class GFileListViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    GFileListViewDelegate(QObject *parent = nullptr);
    virtual ~GFileListViewDelegate();

private:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // GFILELISTVIEW_H
