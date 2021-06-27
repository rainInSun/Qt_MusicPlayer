#ifndef GCOMMONWIDGET_H
#define GCOMMONWIDGET_H

#include <QFrame>
#include <QLabel>
#include "gmusicplayservice.h"

class GCloseButton;

class GTitleBar : public QFrame
{
    Q_OBJECT
public:
    GTitleBar(QWidget *parent = nullptr);
    virtual ~GTitleBar();

signals:
    void sigCloseApp();

private:
    GCloseButton *m_closeBtn;
};

class GMusicProgressLabel : public QLabel, public IGProgressShower
{
    Q_OBJECT
public:
    enum Type
    {
        progressType = 0,
        totalTimeType
    };
public:
    GMusicProgressLabel(Type labelType, QWidget *parent = nullptr);
    virtual ~GMusicProgressLabel();

    virtual void ShowProgress(qint64 curTime, qint64 totalTime) override;
    void setDefaulText();

private:
    Type m_labelType;
};


#endif // GCOMMONWIDGET_H
