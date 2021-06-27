#include "gfilelistview.h"
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>
#include <QPainter>

#define ITEM_HEIGHT 30

GFileListView::GFileListView(QWidget *parent)
    : QListView(parent)
{
    setFixedSize(400, 560);
}

GFileListView::~GFileListView()
{

}

//////////////////////////////////////////////////////////////
GFileListViewDelegate::GFileListViewDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

GFileListViewDelegate::~GFileListViewDelegate()
{

}

QSize GFileListViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(), ITEM_HEIGHT);
}

void GFileListViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid())
    {
        painter->save();
        QVariant var = index.data(SHOW_DATA_ROLE);
        ItemShowData itemData = var.value<ItemShowData>();

        QRect itemRect(option.rect.x(), option.rect.y(), option.rect.width() - 1, option.rect.height() - 1);
        if (option.state.testFlag(QStyle::State_MouseOver))
        {
            painter->setPen(QPen(QColor("#ebeced")));
            painter->setBrush(QColor("#ebeced"));
            painter->drawRect(itemRect);
        }
        else if (option.state.testFlag(QStyle::State_Selected))
        {
            painter->setPen(QPen(QColor("#e3e3e5")));
            painter->setBrush(QColor("#e3e3e5"));
            painter->drawRect(itemRect);
        }

        QRect iconRect(itemRect.left() + 5, itemRect.top() + 5, 20, 20);
        QRect songNameRect(iconRect.right() + 10, iconRect.bottom() - 20, itemRect.width() - iconRect.width() - 10, 20);

        painter->drawPixmap(iconRect, QPixmap(itemData.iconPath));
        painter->setPen(QPen(Qt::black));
        painter->drawText(songNameRect, itemData.songName);
        painter->restore();
    }
}

QWidget *GFileListViewDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return nullptr;
}
