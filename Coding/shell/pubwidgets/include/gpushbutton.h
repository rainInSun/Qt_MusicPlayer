#ifndef GPUSHBUTTON_H
#define GPUSHBUTTON_H

#include <QPushButton>

class QWidget;

class GPushButton : public QPushButton
{
	Q_OBJECT
public:
	GPushButton(QWidget *parent = nullptr);
	virtual ~GPushButton();
};

class GCloseButton : public GPushButton
{
	Q_OBJECT
public:
	GCloseButton(QWidget *parent = nullptr);
	virtual ~GCloseButton();

signals:
	void mouseRelease();

protected:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void enterEvent(QEvent *event) override;
	virtual void leaveEvent(QEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
	int m_transparency;
};

class GStartPauseButton : public GPushButton
{
	Q_OBJECT
public:
	enum MusicState
	{
		play = 0,
		pause,
		begin
	};
	enum MouseState
	{
		normal = 0,
		hover = 1,
		pressed = 2
	};
public:
	GStartPauseButton(QWidget *parent = nullptr);
	virtual ~GStartPauseButton();

	void setMusicState(GStartPauseButton::MusicState state);
	GStartPauseButton::MusicState getMusicState();

protected:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void enterEvent(QEvent *event) override;
	virtual void leaveEvent(QEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
	MusicState m_musicState;
	MouseState m_mouseState;
	QVector<QPixmap> m_playImg;
	QVector<QPixmap> m_pauseImg;
};

#endif // GPUSHBUTTON_H
