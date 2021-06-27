#ifndef GMUSICPLAYERMAINWIDGET_H
#define GMUSICPLAYERMAINWIDGET_H

#include <QWidget>
#include "gmusicplayservice.h"

class GProgressBar;
class GLyricShowView;
class GPushButton;
class GTitleBar;
class GMusicProgressLabel;
class GMusicProgressBar;
class GVoiceProgressBar;
class GStartPauseButton;
class GFileListWidget;
class QLabel;

QT_BEGIN_NAMESPACE
namespace Ui { class GMusicPlayerMainWidget; }
QT_END_NAMESPACE

class GMusicPlayerMainWidget : public QWidget
{
	Q_OBJECT

public:
	GMusicPlayerMainWidget(IUnknown *pServProvider, QWidget *parent = nullptr);
	~GMusicPlayerMainWidget();

public slots:
	void onPlayBtnClicked();
	void onNextBtnClicked();
	void onPreBtnClicked();
	void onLyricLoaded(QMap<int, QString>& parsedLyric);
	void onCloseApp();
	void onMusicListBtnClicked();
	void onMuisicListDblClicked(QString songPath);
	void onMusicProgressChanged(double curProgress);
	void onVoiceProgressChanged(double curProgress);

signals:
	void currentSongChanged(int offset);

protected:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
	void initUI();
	void loadLyric(QString lyricPath);

private:
	Ui::GMusicPlayerMainWidget *m_ui;

	GStartPauseButton *m_playBtn;
	GPushButton *m_nextBtn;
	GPushButton *m_preBtn;
	GPushButton *m_musicListBtn;
	QLabel *m_volumeLabel;

	GMusicProgressLabel *m_progressLabel;
	GMusicProgressLabel *m_totalTimeLabel;

	IUnknown *m_servProvider;
	GMusicProgressBar *m_progressBar;
	GVoiceProgressBar *m_voiceBar;
	GLyricShowView *m_lyricView;
	GTitleBar *m_titleBar;
	GFileListWidget *m_musicListWidget;
	QPoint m_basePoint;
	bool m_bDrag;
};
#endif // GMUSICPLAYERMAINWIDGET_H
