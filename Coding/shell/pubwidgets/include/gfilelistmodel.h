#ifndef GFILELISTMODEL_H
#define GFILELISTMODEL_H

#include <QStandardItemModel>

class GFileListModel : public QStandardItemModel
{
    Q_OBJECT
public:
    GFileListModel(QObject *parent = nullptr);
    virtual ~GFileListModel();
};

#endif // GFILELISTMODEL_H
