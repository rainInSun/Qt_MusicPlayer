#include "gfilelistwidget.h"
#include <QPushButton>
#include <QPainter>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDir>
#include <QFileInfoList>
#include <QMouseEvent>

#include "gfilelistview.h"
#include "gfilelistmodel.h"

GFileListWidget::GFileListWidget(QWidget *parent)
    : QWidget(parent)
    , m_titleBar(nullptr)
    , m_fileListView(nullptr)
    , m_defaultPath("")
    , m_fileListModel(nullptr)
    , m_fileListDelegate(nullptr)
	, m_basePoint(QPoint())
	, m_bDrag(false)
{
    initUI();
    m_defaultPath = "C:/Users/Administrator/Desktop/music";
    connect(m_titleBar, &GFileListTitleBar::closeBtnClicked, this, &GFileListWidget::onCloseBtnClicked);
    connect(m_titleBar, &GFileListTitleBar::refreshBtnCicked, this, &GFileListWidget::onRefreshBtnClicked);
    connect(m_fileListView, &GFileListView::doubleClicked, this, &GFileListWidget::onMusicListDblClicked);
    updateModel();
}

GFileListWidget::~GFileListWidget()
{
}

void GFileListWidget::updateModel()
{
    if (m_defaultPath.isEmpty())
        return;

    m_fileListModel->clear();
    QDir dir(m_defaultPath);
    QStringList nameFilters;
    nameFilters << "*.mp3";
    QFileInfoList mp3Files = dir.entryInfoList(nameFilters);

    for (int i = 0; i < mp3Files.size(); i++)
    {
        ItemShowData showData;
        showData.iconPath = ":/button/music.svg";
        showData.songAbsPath = mp3Files[i].absoluteFilePath();
        QStringList splitFilePath = mp3Files[i].absoluteFilePath().split("/");
        showData.songName = splitFilePath[splitFilePath.size() - 1];
        QStandardItem *item = new QStandardItem();
        item->setData(QVariant::fromValue(showData), SHOW_DATA_ROLE);
        m_fileListModel->appendRow(item);
    }

    QModelIndex defaultIndex = m_fileListModel->index(0, 0);
    m_fileListView->setCurrentIndex(defaultIndex);
}

void GFileListWidget::onCloseBtnClicked()
{
    if (isVisible())
        hide();
}

void GFileListWidget::onRefreshBtnClicked()
{
    updateModel();
}

void GFileListWidget::initUI()
{
    setFixedSize(400, 600);
    setWindowFlags(Qt::FramelessWindowHint);

    m_titleBar = new GFileListTitleBar(this);
    m_fileListView = new GFileListView(this);

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addWidget(m_titleBar);
    vLayout->addWidget(m_fileListView);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(vLayout);

    m_fileListModel = new GFileListModel(this);
    m_fileListView->setModel(m_fileListModel);
    m_fileListDelegate = new GFileListViewDelegate(this);
    m_fileListView->setItemDelegate(m_fileListDelegate);
    m_fileListView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

QString GFileListWidget::getCurrentPlaySong()
{
    QModelIndex curIndex = m_fileListView->currentIndex();
    QVariant var = curIndex.data(SHOW_DATA_ROLE);
    ItemShowData itemData = var.value<ItemShowData>();
    return itemData.songAbsPath;

}

QString GFileListWidget::getNextPlaySong()
{
    QModelIndex curIndex = m_fileListView->currentIndex();
    QModelIndex nextIndex;
    if (curIndex.row() > m_fileListModel->rowCount() - 2)
        nextIndex = m_fileListModel->index(0, 0);
    else
        nextIndex = m_fileListModel->index(curIndex.row() + 1, 0);

    QVariant var = nextIndex.data(SHOW_DATA_ROLE);
    ItemShowData itemData = var.value<ItemShowData>();
    return itemData.songAbsPath;

}

QString GFileListWidget::getPrePlaySong()
{
    QModelIndex curIndex = m_fileListView->currentIndex();
    QModelIndex preIndex;
    if (curIndex.row() < 1)
        preIndex = m_fileListModel->index(m_fileListModel->rowCount() - 1, 0);
    else
        preIndex = m_fileListModel->index(curIndex.row() - 1, 0);

    QVariant var = preIndex.data(SHOW_DATA_ROLE);
    ItemShowData itemData = var.value<ItemShowData>();
    return itemData.songAbsPath;
}

void GFileListWidget::onMusicListDblClicked(const QModelIndex& index)
{
    QVariant var = index.data(SHOW_DATA_ROLE);
    ItemShowData itemData = var.value<ItemShowData>();
    emit sigMusicListDblClicked(itemData.songAbsPath);
}

void GFileListWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (m_bDrag)
	{
		move(event->globalPos() - m_basePoint);
	}
}

void GFileListWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (m_titleBar->frameGeometry().contains(event->pos()))
		{
			m_basePoint = event->globalPos() - frameGeometry().topLeft();
			m_bDrag = true;
		}
	}
}

void GFileListWidget::mouseReleaseEvent(QMouseEvent *event)
{
	m_bDrag = false;
}

void GFileListWidget::onCurrentSongChanged(int offset)
{
    QModelIndex curIndex = m_fileListView->currentIndex();
    QModelIndex newIndex;
	if (curIndex.row() + offset < 0)
		newIndex = m_fileListModel->index(m_fileListModel->rowCount() - 1, 0);
	else if (curIndex.row() + offset > m_fileListModel->rowCount() - 1)
		newIndex = m_fileListModel->index(0, 0);
	else
		newIndex = m_fileListModel->index(curIndex.row() + offset, 0);

    m_fileListView->setCurrentIndex(newIndex);
}

///////////////////////////////////////////////////////
GFileListTitleBar::GFileListTitleBar(QWidget *parent)
    : QWidget(parent)
    , m_refreshBtn(nullptr)
    , m_closeBtn(nullptr)
    , m_titleLabel(nullptr)
{
    initUI();
    connect(m_closeBtn, &QPushButton::clicked, this, &GFileListTitleBar::closeBtnClicked);
    connect(m_refreshBtn, &QPushButton::clicked, this, &GFileListTitleBar::refreshBtnCicked);
}

GFileListTitleBar::~GFileListTitleBar()
{

}

void GFileListTitleBar::initUI()
{
    setFixedSize(400, 40);
    m_titleLabel = new QLabel(this);
    m_titleLabel->setStyleSheet("QLabel {font-size: 20px; color: rgb(255, 255, 255)}");
    m_titleLabel->setText("歌曲列表");
    m_titleLabel->setFixedSize(100, 30);

    m_refreshBtn = new QPushButton(this);
    m_refreshBtn->setFixedSize(30, 30);
    m_refreshBtn->setStyleSheet("QPushButton {border-image: url(:/button/refresh.svg)}"
                                "QPushButton:hover {border-image: url(:/button/refresh_hover.svg)}"
                                "QPushButton:pressed {border-image: url(:/button/refresh_pressed.svg)}");

    m_closeBtn = new QPushButton(this);
    m_closeBtn->setFixedSize(30, 30);
    m_closeBtn->setStyleSheet("QPushButton {border-image: url(:/button/close.svg)}"
                                "QPushButton:hover {border-image: url(:/button/close_hover.svg)}"
                                "QPushButton:pressed {border-image: url(:/button/close_pressed.svg)}");
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addWidget(m_titleLabel);
    hLayout->addStretch();
    hLayout->addWidget(m_refreshBtn);
    hLayout->addWidget(m_closeBtn);
    hLayout->setContentsMargins(5, 5, 5, 5);

    setLayout(hLayout);
}

void GFileListTitleBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(rect());
}
