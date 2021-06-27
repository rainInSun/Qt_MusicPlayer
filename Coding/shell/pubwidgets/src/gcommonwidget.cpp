#include "gcommonwidget.h"
#include "gpushbutton.h"
#include "gstyledef.h"
#include <QMouseEvent>

GTitleBar::GTitleBar(QWidget *parent)
    : QFrame(parent)
    , m_closeBtn(nullptr)
{
    setFixedSize(MUSIC_TITLEBAR_WIDTH, MUSIC_TITLEBAR_HEIGHT);
    setStyleSheet("GTitleBar {background-color:rgb(255,255,255,0)}"
                    "GTitleBar:hover {background-color:rgb(255,255,255,50)}");

    m_closeBtn = new GCloseButton(this);
    m_closeBtn->move(width() - m_closeBtn->width(), 0);
    connect(m_closeBtn, &GCloseButton::mouseRelease, this, &GTitleBar::sigCloseApp);
}

GTitleBar::~GTitleBar()
{

}

/////////////////////////////////////////////////////////////////////////////////

GMusicProgressLabel::GMusicProgressLabel(Type labelType, QWidget *parent)
    : QLabel(parent)
    , m_labelType(labelType)
{
    setDefaulText();
    setStyleSheet("QLabel {color: rgb(255, 255, 255)}");
}

GMusicProgressLabel::~GMusicProgressLabel()
{

}

void GMusicProgressLabel::ShowProgress(qint64 curTime, qint64 totalTime)
{
    if (progressType == m_labelType)
    {
        int minNum = curTime / 60000;
        int secNum = (curTime - minNum * 60000) / 1000;
        setText(QString("%1:%2").arg(minNum).arg(secNum));
    }
    else if (totalTimeType == m_labelType)
    {
        int minNum = totalTime / 60000;
        int secNum = (totalTime - minNum * 60000) / 1000;
        setText(QString("%1:%2").arg(minNum).arg(secNum));
    }
}

void GMusicProgressLabel::setDefaulText()
{
    setText("--:--");
}