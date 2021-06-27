#include "../include/gprogressbar.h"
#include "gstyledef.h"
#include <QPainter>
#include <QBrush>
#include <QPaintEvent>
#include <QPen>

static const int lineWidth = MUSIC_PROGRESSMUSIC_WIDTH - 10;
static const int lineHeight = 4;

//////////////////////////////////////////////////////////
GProgressBtn::GProgressBtn(QWidget *parent)
	: QPushButton(parent)
	, m_bPressed(false)
{
	setFixedSize(16, 16);
}

GProgressBtn::~GProgressBtn()
{

}

void GProgressBtn::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	if (m_bPressed)
	{
		painter.setBrush(QBrush(Qt::gray));
		painter.setPen(QPen(Qt::gray));
	}
	else
	{
		painter.setBrush(QBrush(Qt::white));
		painter.setPen(QPen(Qt::white));
	}
	QRectF rect(1.0, 1.0, rect().width() - 2.0, rect().height() - 2.0);
	painter.drawEllipse(rect);
}

void GProgressBtn::mousePressEvent(QMouseEvent *event)
{
	m_bPressed = true;
	update();
}

void GProgressBtn::mouseReleaseEvent(QMouseEvent *event)
{
	m_bPressed = false;
	update();
	emit progressBtnRelease(event->globalPos());
}

bool GProgressBtn::isPressed()
{
	return m_bPressed;
}
/////////////////////////////////////////////////////////////////

GProgressBar::GProgressBar(QWidget *parent)
	: QWidget(parent)
	, m_progressBtn(nullptr)
	, m_progressBtnBeginX(0)
	, m_progressBtnEndX(0)
	, m_progressBtnY(0)
	, m_curProgress(0.0)
{
	initUI();
	m_progressBtnBeginX = (rect().width() - lineWidth) / 2 - 3;
	m_progressBtnEndX = (rect().width() - lineWidth) / 2 + lineWidth;
	m_progressBtnY = (rect().height() - m_progressBtn->rect().height()) / 2 - 3;

	m_progressBtn->move(m_progressBtnBeginX, m_progressBtnY);

	connect(m_progressBtn, &GProgressBtn::progressBtnRelease, this, &GProgressBar::onProgressBtnRelease);
}

GProgressBar::~GProgressBar()
{

}

void GProgressBar::initUI()
{
	setFixedSize(MUSIC_PROGRESSMUSIC_WIDTH, MUSIC_PROGRESSMUSIC_HEIGHT);
	m_progressBtn = new GProgressBtn(this);
}

void GProgressBar::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setPen(QPen(Qt::gray, lineHeight));
	painter.drawLine((rect().width() - lineWidth) / 2, 
					(rect().height() - lineHeight) / 2, 
					(rect().width() - lineWidth) / 2 + lineWidth, 
					(rect().height() - lineHeight) / 2);

	if (m_progressBtn->pos().x() > (rect().width() - lineWidth) / 2)
	{
		painter.setPen(QPen(Qt::white, lineHeight));
		painter.drawLine((rect().width() - lineWidth) / 2, 
						(rect().height() - lineHeight) / 2,
						m_progressBtn->pos().x(), 
						(rect().height() - lineHeight) / 2);
	}

}

void GProgressBar::mouseMoveEvent(QMouseEvent *event)
{
	if (m_progressBtn->isPressed())
	{
		int a = event->x();
		if (event->x() < m_progressBtnBeginX)
			m_progressBtn->move(m_progressBtnBeginX, m_progressBtnY);
		else if (event->x() > m_progressBtnEndX)
			m_progressBtn->move(m_progressBtnEndX, m_progressBtnY);
		else
			m_progressBtn->move(event->x(), m_progressBtnY);

		update();
	}
}

void GProgressBar::onProgressBtnRelease(QPoint globalPoint)
{
	QPoint localPoint = mapFromGlobal(globalPoint);
	int localX = localPoint.x();
	if (localX < m_progressBtnBeginX)
		localX = m_progressBtnBeginX;
	else if (localX > m_progressBtnEndX)
		localX = m_progressBtnEndX;

	m_curProgress = (double)(localX - m_progressBtnBeginX) / (double)(m_progressBtnEndX - m_progressBtnBeginX);
	update();

	emit progressChanged(m_curProgress);
}

void GProgressBar::setProgress(qint64 curPos, qint64 total)
{
	if (!m_progressBtn->isPressed())
	{
		m_curProgress = (double) curPos / (double) total;
		m_progressBtn->move(m_progressBtnBeginX + m_curProgress * (m_progressBtnEndX - m_progressBtnBeginX), m_progressBtnY);
		update();
	}
}

void GProgressBar::setProgress(double curProgress)
{
	if (!m_progressBtn->isPressed())
	{
		m_curProgress = curProgress;
		m_progressBtn->move(m_progressBtnBeginX + m_curProgress * (m_progressBtnEndX - m_progressBtnBeginX), m_progressBtnY);
		update();
	}
}

double GProgressBar::getCurProgress()
{
	return m_curProgress;
}

////////////////////////////////////////////////////////////////////////////////////////

GMusicProgressBar::GMusicProgressBar(QWidget *parent)
	: GProgressBar(parent)
{

}

GMusicProgressBar::~GMusicProgressBar()
{

}

void GMusicProgressBar::ShowProgress(qint64 curTime, qint64 totalTime)
{
	setProgress(curTime, totalTime);
}

///////////////////////////////////////////////////////////////////////////////

GVoiceProgressBar::GVoiceProgressBar(QWidget *parent)
	: GProgressBar(parent)
{

}

GVoiceProgressBar::~GVoiceProgressBar()
{
	
}
