#include "../include/gpushbutton.h"
#include <QPainter>

GPushButton::GPushButton(QWidget *parent)
    : QPushButton(parent)
{

}

GPushButton::~GPushButton()
{

}

//////////////////////////////////////////////////////////////

GCloseButton::GCloseButton(QWidget *parent)
    : GPushButton(parent)
    , m_transparency(0)
{
    setFixedSize(25, 25);
    // setStyleSheet("GCloseButton:hover {background-color:rgba(255, 0, 0, 0)}"
    //             "GCloseButton:pressed {background-color:rgba(0, 255, 0, 0)}"
    //             "GCloseButton {background-color:rgba(0, 0, 255, 0)}");
}

GCloseButton::~GCloseButton()
{

}

void GCloseButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    painter.setPen(QPen(Qt::white));
    painter.drawLine(5, 5, width() - 5, height() - 5);
    painter.drawLine(width() - 5, 5, 5, height() - 5);

    painter.setPen(QPen(Qt::NoPen));
    painter.setBrush(QBrush(QColor(255, 255, 255, m_transparency)));
    painter.drawRect(0, 0, width(), height());
}

void GCloseButton::enterEvent(QEvent *event)
{
    m_transparency = 100;
    update();
}

void GCloseButton::leaveEvent(QEvent *event)
{
    m_transparency = 0;
    update();
}

void GCloseButton::mousePressEvent(QMouseEvent *event)
{
    m_transparency = 150;
    update();
}

void GCloseButton::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseRelease();
}

//////////////////////////////////////////////////////////////////////

GStartPauseButton::GStartPauseButton(QWidget *parent)
    : GPushButton(parent)
    , m_musicState(begin)
    , m_mouseState(normal)
    , m_playImg(QVector<QPixmap> ())
    , m_pauseImg(QVector<QPixmap> ())
{
    //这里和枚举要对应起来
    QPixmap playImg = QPixmap(":/button/play.svg");
    m_playImg.push_back(playImg);
    QPixmap playHoverImg = QPixmap(":/button/play_hover.svg");
    m_playImg.push_back(playHoverImg);
    QPixmap playPressedImg = QPixmap(":/button/play_pressed.svg");
    m_playImg.push_back(playPressedImg);

    QPixmap pauseImg = QPixmap(":/button/pause.svg");
    m_pauseImg.push_back(pauseImg);
    QPixmap pauseHoverImg = QPixmap(":/button/pause_hover.svg");
    m_pauseImg.push_back(pauseHoverImg);
    QPixmap pausePressedImg = QPixmap(":/button/pause_pressed.svg");
    m_pauseImg.push_back(pausePressedImg);
}

GStartPauseButton::~GStartPauseButton()
{

}

void GStartPauseButton::setMusicState(GStartPauseButton::MusicState state)
{
    m_musicState = state;
    update();
}

GStartPauseButton::MusicState GStartPauseButton::getMusicState()
{
    return m_musicState;
}

void GStartPauseButton::paintEvent(QPaintEvent *event)
{
    if (m_pauseImg.size() < 3 || m_playImg.size() < 3)
        return;

    QPainter painter(this);
    if (play == m_musicState)
        painter.drawPixmap(rect(), m_pauseImg[m_mouseState]);
    else if (pause == m_musicState || begin == m_musicState)
        painter.drawPixmap(rect(), m_playImg[m_mouseState]);
}

void GStartPauseButton::enterEvent(QEvent *event)
{
    m_mouseState = hover;
    update();
}

void GStartPauseButton::leaveEvent(QEvent *event)
{
    m_mouseState = normal;
    update();
}

void GStartPauseButton::mousePressEvent(QMouseEvent *event)
{
    m_mouseState = pressed;
    update();
    GPushButton::mousePressEvent(event);
}

void GStartPauseButton::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouseState = hover;
    update();
    GPushButton::mouseReleaseEvent(event);
}