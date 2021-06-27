#ifndef GFILELISTWIDGET_H
#define GFILELISTWIDGET_H

#include <QWidget>

class QPushButton;
class QLabel;
class GFileListView;
class GFileListModel;
class GFileListViewDelegate;

class GFileListTitleBar : public QWidget
{
    Q_OBJECT

public:
    GFileListTitleBar(QWidget *parent = nullptr);
    ~GFileListTitleBar();

signals:
    void refreshBtnCicked();
    void closeBtnClicked();

private:
    void initUI();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPushButton *m_refreshBtn;
    QPushButton *m_closeBtn;
    QLabel *m_titleLabel;
};

class GFileListWidget : public QWidget
{
    Q_OBJECT

public:
    GFileListWidget(QWidget *parent = nullptr);
    ~GFileListWidget();

    void updateModel();
    void onCloseBtnClicked();
    void onRefreshBtnClicked();

    QString getCurrentPlaySong();
    QString getNextPlaySong();
    QString getPrePlaySong();

public slots:
    void onMusicListDblClicked(const QModelIndex& index);
    void onCurrentSongChanged(int offset);

signals:
    void sigMusicListDblClicked(QString filePath);

protected:
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void initUI();

private:
    GFileListTitleBar *m_titleBar;
    GFileListView *m_fileListView;
    QString m_defaultPath;
    GFileListModel *m_fileListModel;
    GFileListViewDelegate *m_fileListDelegate;

    bool m_bDrag;
	QPoint m_basePoint;
};
#endif // GFILELISTWIDGET_H
