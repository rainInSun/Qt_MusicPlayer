#ifndef GPROGRESSBAR_H
#define GPROGRESSBAR_H

#include <QWidget>
#include <QPushButton>
#include "gmusicplayservice.h"

class GProgressBtn : public QPushButton
{
	Q_OBJECT
public:
	GProgressBtn(QWidget *parent = nullptr);
	virtual ~GProgressBtn();

protected:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;

public:
	bool isPressed();

signals:
	void progressBtnRelease(QPoint globalPoint);

private:
	bool m_bPressed;
};

class GProgressBar : public QWidget
{
	Q_OBJECT
public:
	GProgressBar(QWidget *parent = nullptr);
	virtual ~GProgressBar();

	void setProgress(qint64 curPos, qint64 total);
	void setProgress(double curProgress);
	double getCurProgress();

protected:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;

protected slots:
	void onProgressBtnRelease(QPoint globalPoint);

private:
	void initUI();

signals:
	void progressChanged(double curProgress);

private:
	GProgressBtn *m_progressBtn;
	int m_progressBtnBeginX;
	int m_progressBtnEndX;
	int m_progressBtnY;
	double m_curProgress;
};

class GMusicProgressBar : public GProgressBar, public IGProgressShower
{
	Q_OBJECT
public:
	GMusicProgressBar(QWidget *parent = nullptr);
	virtual ~GMusicProgressBar();

	virtual void ShowProgress(qint64 curTime, qint64 totalTime) override;
};

class GVoiceProgressBar : public GProgressBar
{
	Q_OBJECT
public:
	GVoiceProgressBar(QWidget *parent = nullptr);
	virtual ~GVoiceProgressBar();
};

#endif // GPROGRESSBAR_H