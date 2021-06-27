#include "glyricshowview.h"
#include "gstyledef.h"
#include <QPainter>
#include <QTimer>
#include <QDebug>

static const double refreshFreq = 10.0;
static const double refreshTime = 0.5;
static const int offsetItemNum = 5;
static const int centerOnOffset = -30;

GLyricItem::GLyricItem(const QString& lyricStr, const QColor& textColor,
                       const QFont& textFont, int textPixelSize, int width, int height)
    : m_color(textColor)
    , m_font(textFont)
    , m_textSize(textPixelSize)
    , m_text(lyricStr)
    , m_width(width)
    , m_height(height)
{

}

GLyricItem::~GLyricItem()
{

}

void GLyricItem::setTextColor(const QColor& textColor)
{
    m_color = textColor;
    update();
}

void GLyricItem::setTextFont(const QFont& textFont)
{
    m_font = textFont;
    update();
}

void GLyricItem::setTextPixelSize(int textSize)
{
    m_textSize = textSize;
    update();
}

void GLyricItem::setWidth(int width)
{
    m_width = width;
    update();
}

void GLyricItem::setHeight(int height)
{
    m_height = height;
    update();
}

int GLyricItem::width()
{
    return m_width;
}

int GLyricItem::height()
{
    return m_height;
}

QRectF GLyricItem::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void GLyricItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    m_font.setPixelSize(m_textSize);
    QFontMetrics ftMetrics(m_font);
    QRect rect = ftMetrics.boundingRect(m_text);
    int beginX = (m_width - rect.width()) / 2;
    int beginY = (m_height - rect.height()) / 2 + rect.height();

    painter->setFont(m_font);
    painter->setPen(m_color);
    painter->drawText(beginX, beginY, m_text);

//    painter->setPen(QPen(Qt::red));
//    painter->drawRect(boundingRect());
}

////////////////////////////////////////////////////////////////////////
GLyricScene::GLyricScene(QObject *parent)
    : QGraphicsScene(parent)
{

}

GLyricScene::~GLyricScene()
{

}

///////////////////////////////////////////////////////////////////////
//该类实现了歌词的滚动显示，主要通过QGraphicsView体系实现，通过定时器刷centerOn()
GLyricShowView::GLyricShowView(const QMap<int, QString>& parsedLyric, QWidget *parent)
    : QGraphicsView(parent)
    , m_lyricItems(QVector<GLyricItem *>())
    , m_curCenterItem(nullptr)
    , m_lyricScene(nullptr)
    , m_parsedLyric(parsedLyric)
    , m_timer(nullptr)
    , m_curCenterOnHeight(0)
    , m_lstCenterOnHeight(0)
    , m_moveDistance(0)
    , m_isLoadedLyric(false)
{
    init();
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &GLyricShowView::onTime);

}

GLyricShowView::~GLyricShowView()
{

}

void GLyricShowView::startAnimation()
{
    if (!m_timer->isActive())
    {
        m_timer->start(refreshTime / refreshFreq * 1000);
    }
}

void GLyricShowView::stopAnimation()
{
    m_timer->stop();
}

void GLyricShowView::init()
{
    setFixedSize(MUSIC_LYRICVIEW_WIDTH, MUSIC_LYRICVIEW_WIDTH);
    setStyleSheet("background: transparent;border:0px");
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_lyricScene = new GLyricScene(this);
    setScene(m_lyricScene);
    QMap<int, QString> initMap;
	initMap.insert(0, "Loading ... ...");
    setLyric(initMap);
}

void GLyricShowView::onTime()
{ 
    if (qAbs(m_moveDistance) <= MUSIC_LYRICVIEWITEM_HEIGHT)
    {
        m_curCenterOnHeight += m_moveDistance / refreshFreq;
        centerOn(width() / 2, m_curCenterOnHeight + centerOnOffset);
        update();

        if (qAbs(m_curCenterOnHeight - m_lstCenterOnHeight) > qAbs(m_moveDistance))
        {
            qDebug() << m_moveDistance << m_lstCenterOnHeight << m_curCenterOnHeight;
            m_timer->stop();
            m_lstCenterOnHeight = m_curCenterOnHeight;
        }
    }
    else
    {
        m_curCenterOnHeight += m_moveDistance;
        centerOn(width() / 2, m_curCenterOnHeight + centerOnOffset);
        update();

        qDebug() << m_moveDistance << m_lstCenterOnHeight << m_curCenterOnHeight;
        m_timer->stop();
        m_lstCenterOnHeight = m_curCenterOnHeight;
    }
}

void GLyricShowView::setLyric(const QMap<int, QString>& parsedLyric)
{
    m_parsedLyric = parsedLyric;
    for (int i = 0; i < m_lyricItems.size(); i++)
    {
        m_lyricScene->removeItem(m_lyricItems[i]);
        delete m_lyricItems[i];
    }
    m_lyricItems.clear();

	int sceneWidth = 0;
	int sceneHeight = 0;
	int cnt = 0;

	for (; cnt < offsetItemNum; cnt++)
	{
		GLyricItem *item = new GLyricItem("");
		m_lyricItems.push_back(item);
		item->setWidth(MUSIC_LYRICVIEWITEM_WIDTH);
        item->setHeight(MUSIC_LYRICVIEWITEM_HEIGHT);
		item->setPos(0, cnt * item->height());
		sceneHeight += item->height();
		sceneWidth = item->width();
		m_lyricScene->addItem(item);
	}
	cnt--;
	auto iterator = m_parsedLyric.begin();
    for ( ; iterator != m_parsedLyric.end(); iterator++)
    {
		cnt++;
        GLyricItem *item = new GLyricItem(iterator.value());
        m_lyricItems.push_back(item);
        item->setWidth(MUSIC_LYRICVIEWITEM_WIDTH);
        item->setHeight(MUSIC_LYRICVIEWITEM_HEIGHT);
        item->setPos(0, cnt * item->height());
        sceneHeight += item->height();
        sceneWidth = item->width();
        m_lyricScene->addItem(item);
    }
    cnt++;
    if (m_parsedLyric.size() < 10)
    {
        for (; cnt < offsetItemNum * 2 + m_parsedLyric.size(); cnt++)
        {
            GLyricItem *item = new GLyricItem("");
            m_lyricItems.push_back(item);
            item->setWidth(MUSIC_LYRICVIEWITEM_WIDTH);
            item->setHeight(MUSIC_LYRICVIEWITEM_HEIGHT);
            item->setPos(0, cnt * item->height());
            sceneHeight += item->height();
            sceneWidth = item->width();
            m_lyricScene->addItem(item);
        }
    }

	m_lyricScene->setSceneRect(0, 0, sceneWidth, sceneHeight);
    if (offsetItemNum - 1 < m_lyricItems.size())
    {
        m_curCenterItem = m_lyricItems[offsetItemNum - 1];
        m_curCenterItem->setTextColor(Qt::white);
        m_curCenterOnHeight = m_curCenterItem->mapToScene(m_curCenterItem->width() / 2, m_curCenterItem->height() / 2).y();
        m_lstCenterOnHeight = m_curCenterOnHeight;
        centerOn(width() / 2, m_curCenterOnHeight + centerOnOffset);
        update();
    }
    m_isLoadedLyric = true;
}

void GLyricShowView::ShowProgress(qint64 curTime, qint64 totalTime)
{
    if (!m_isLoadedLyric)
        return;

    auto iterator = m_parsedLyric.begin();
    int cnt = 0;
    for ( ; iterator != m_parsedLyric.end(); iterator++)
    {
        cnt++;
        if (iterator.key() > curTime)
            break;
    }
    cnt--;
    cnt = cnt + offsetItemNum - 1;
    // if (cnt > m_lyricItems.size() - 1 || m_lyricItems[cnt] == m_curCenterItem)
     if (cnt > m_lyricItems.size() - 1)
        return;

    for (int i = 0; i < m_lyricItems.size(); i++)
    {
        if (m_lyricItems[i])
            m_lyricItems[i]->setTextColor(Qt::gray);
    }

    int beginHeight = m_curCenterItem->mapToScene(m_curCenterItem->width() / 2, m_curCenterItem->height() / 2).y();
    int endHeight = m_lyricItems[cnt]->mapToScene(m_lyricItems[cnt]->width() / 2, m_lyricItems[cnt]->height() / 2).y();
    m_moveDistance = endHeight - beginHeight;
	m_curCenterItem = m_lyricItems[cnt];
	m_curCenterItem->setTextColor(Qt::white);

    if (m_moveDistance == 0)
    {
        update();
        return;
    }
    startAnimation();
    //这里不能更新否则onTime里面的数据不对
    // m_curCenterOnHeight = m_curCenterItem->mapToScene(m_curCenterItem->width() / 2, m_curCenterItem->height() / 2).y();
    // m_lstCenterOnHeight = m_curCenterOnHeight;
}

//表明歌词是否加载，未加载前不会去相应歌词的滚动动画
void GLyricShowView::setIsLoadedLyric(bool isLoad)
{
    m_isLoadedLyric = isLoad;
}