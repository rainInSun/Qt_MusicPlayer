#include "../include/gmusicplayermainwidget.h"
#include "./ui_gmusicplayermainwidget.h"
#include "gprogressbar.h"
#include "glyricshowview.h"
#include "gpushbutton.h"
#include "gcommonwidget.h"
#include "gfilelistwidget.h"
#include "gstyledef.h"
#include <QUrl>
#include <QPainter>
#include <QDir>
#include <QMouseEvent>
#include <qDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>

GMusicPlayerMainWidget::GMusicPlayerMainWidget(IUnknown *pServProvider, QWidget *parent)
	: QWidget(parent)
	, m_ui(new Ui::GMusicPlayerMainWidget)
	, m_playBtn(nullptr)
	, m_nextBtn(nullptr)
	, m_preBtn(nullptr)
	, m_musicListBtn(nullptr)
	, m_volumeLabel(nullptr)
	, m_progressLabel(nullptr)
	, m_totalTimeLabel(nullptr)
	, m_servProvider(pServProvider)
	, m_progressBar(nullptr)
	, m_voiceBar(nullptr)
	, m_lyricView(nullptr)
	, m_titleBar(nullptr)
	, m_musicListWidget(nullptr)
	, m_basePoint(QPoint())
	, m_bDrag(false)
{
	m_ui->setupUi(this);
	Q_INIT_RESOURCE(res);
	initUI();
	setWindowFlags(Qt::FramelessWindowHint);
	connect(m_playBtn, &GStartPauseButton::clicked, this, &GMusicPlayerMainWidget::onPlayBtnClicked);
	connect(m_musicListBtn, &GPushButton::clicked, this, &GMusicPlayerMainWidget::onMusicListBtnClicked);
	connect(m_nextBtn, &GPushButton::clicked, this, &GMusicPlayerMainWidget::onNextBtnClicked);
	connect(m_preBtn, &GPushButton::clicked, this, &GMusicPlayerMainWidget::onPreBtnClicked);
	connect(m_musicListWidget, &GFileListWidget::sigMusicListDblClicked, this, &GMusicPlayerMainWidget::onMuisicListDblClicked);
	connect(m_progressBar, &GMusicProgressBar::progressChanged, this, &GMusicPlayerMainWidget::onMusicProgressChanged);
	connect(m_voiceBar, &GVoiceProgressBar::progressChanged, this, &GMusicPlayerMainWidget::onVoiceProgressChanged);
	connect(this, &GMusicPlayerMainWidget::currentSongChanged, m_musicListWidget, &GFileListWidget::onCurrentSongChanged);
}

GMusicPlayerMainWidget::~GMusicPlayerMainWidget()
{
	Q_CLEANUP_RESOURCE(res);
	delete m_ui;

	if (m_musicListWidget)
		delete m_musicListWidget;
}

void GMusicPlayerMainWidget::onPlayBtnClicked()
{
	if (GStartPauseButton::begin == m_playBtn->getMusicState())
	{
		QString songPath = m_musicListWidget->getCurrentPlaySong();
		qDebug() << songPath;
		if (songPath.isEmpty())
			return;

		if (m_servProvider && dynamic_cast<IGMusicPlayService *>(m_servProvider))
		{
			IGMusicPlayService *pMusicSer = dynamic_cast<IGMusicPlayService *>(m_servProvider);
			pMusicSer->SetVolume(m_voiceBar->getCurProgress());
			pMusicSer->MusicPlay(songPath);
			m_playBtn->setMusicState(GStartPauseButton::play);
		}

		QStringList pathComponent = songPath.split(".");
		if (pathComponent.size() != 2)
			return;
		QString lyricPath = pathComponent[0] + ".lrc";
		loadLyric(lyricPath);
	}
	else if (GStartPauseButton::play == m_playBtn->getMusicState())
	{
		if (m_servProvider && dynamic_cast<IGMusicPlayService *>(m_servProvider))
		{
			IGMusicPlayService *pMusicSer = dynamic_cast<IGMusicPlayService *>(m_servProvider);
			pMusicSer->MusicPause();
			m_playBtn->setMusicState(GStartPauseButton::pause);
		}
	}
	else if (GStartPauseButton::pause == m_playBtn->getMusicState())
	{
		if (m_servProvider && dynamic_cast<IGMusicPlayService *>(m_servProvider))
		{
			IGMusicPlayService *pMusicSer = dynamic_cast<IGMusicPlayService *>(m_servProvider);
			pMusicSer->MusicContinue();
			m_playBtn->setMusicState(GStartPauseButton::play);
		}
	}
}

void GMusicPlayerMainWidget::onNextBtnClicked()
{
	QString nextSong = m_musicListWidget->getNextPlaySong();
	if (nextSong.isEmpty())
		return;

	if (m_servProvider && dynamic_cast<IGMusicPlayService *>(m_servProvider))
	{
		IGMusicPlayService *pMusicSer = dynamic_cast<IGMusicPlayService *>(m_servProvider);
		pMusicSer->SetVolume(m_voiceBar->getCurProgress());
		pMusicSer->MusicPlay(nextSong);
		m_playBtn->setMusicState(GStartPauseButton::play);
	}

	QStringList pathComponent = nextSong.split(".");
	if (pathComponent.size() != 2)
		return;
	QString lyricPath = pathComponent[0] + ".lrc";
	loadLyric(lyricPath);

	emit currentSongChanged(1);
}

void GMusicPlayerMainWidget::onPreBtnClicked()
{
	QString preSong = m_musicListWidget->getPrePlaySong();
	if (preSong.isEmpty())
		return;

	if (m_servProvider && dynamic_cast<IGMusicPlayService *>(m_servProvider))
	{
		IGMusicPlayService *pMusicSer = dynamic_cast<IGMusicPlayService *>(m_servProvider);
		pMusicSer->SetVolume(m_voiceBar->getCurProgress());
		pMusicSer->MusicPlay(preSong);
		m_playBtn->setMusicState(GStartPauseButton::play);
	}

	QStringList pathComponent = preSong.split(".");
	if (pathComponent.size() != 2)
		return;
	QString lyricPath = pathComponent[0] + ".lrc";
	loadLyric(lyricPath);

	emit currentSongChanged(-1);

}

void GMusicPlayerMainWidget::loadLyric(QString lyricPath)
{
	QMap<int, QString> initMap;
	initMap.insert(0, "Loading ... ...");
	m_lyricView->setLyric(initMap);
	m_lyricView->setIsLoadedLyric(false);

	if (m_servProvider && dynamic_cast<GLyricParserBase *>(m_servProvider))
	{
		GLyricParserBase *pLyricParser = dynamic_cast<GLyricParserBase *>(m_servProvider);
		pLyricParser->parseStart(lyricPath);
		connect(pLyricParser, &GLyricParserBase::sigLyricParseFinish, this, &GMusicPlayerMainWidget::onLyricLoaded, Qt::UniqueConnection);
	}
}

void GMusicPlayerMainWidget::initUI()
{
	setFixedSize(MUSIC_PLAYER_WIDTH, MUSIC_PLAYER_HEIGHT);
	QVBoxLayout *mainLayout = new QVBoxLayout(this);

	m_titleBar = new GTitleBar(this);
	connect(m_titleBar, &GTitleBar::sigCloseApp, this, &GMusicPlayerMainWidget::onCloseApp);

	m_voiceBar = new GVoiceProgressBar(this);
	m_voiceBar->setProgress(0.3);

	m_volumeLabel = new QLabel(this);
	m_volumeLabel->setFixedSize(25, 25);
	m_volumeLabel->setPixmap(QPixmap(":/button/volume.svg"));

	QVBoxLayout *VLLayout = new QVBoxLayout(this);
	VLLayout->addWidget(m_volumeLabel);
	VLLayout->addSpacing(8);
	
	QHBoxLayout *voiceLayout = new QHBoxLayout(this);
	voiceLayout->addStretch();
	voiceLayout->addLayout(VLLayout);
	voiceLayout->addSpacing(2);
	voiceLayout->addWidget(m_voiceBar);
	voiceLayout->addStretch();

	m_progressBar = new GMusicProgressBar(this);
	m_progressLabel = new GMusicProgressLabel(GMusicProgressLabel::progressType, this);
	m_totalTimeLabel = new GMusicProgressLabel(GMusicProgressLabel::totalTimeType, this);

	QVBoxLayout *PLLayout = new QVBoxLayout(this);
	PLLayout->addWidget(m_progressLabel);
	PLLayout->addSpacing(8);

	QVBoxLayout *TTLLayout = new QVBoxLayout(this);
	TTLLayout->addWidget(m_totalTimeLabel);
	TTLLayout->addSpacing(8);

	QHBoxLayout *progressLyout = new QHBoxLayout(this);
	progressLyout->addStretch();
	progressLyout->addLayout(PLLayout);
	progressLyout->addWidget(m_progressBar);
	progressLyout->addLayout(TTLLayout);
	progressLyout->addStretch();

	QMap<int, QString> preText;
	preText.insert(0, "Loading ... ...");
	m_lyricView = new GLyricShowView(preText, this);

	m_playBtn = new GStartPauseButton(this);
	m_playBtn->setFixedSize(48, 48);
	m_nextBtn = new GPushButton(this);
	m_nextBtn->setFixedSize(48, 48);
	m_nextBtn->setStyleSheet("GPushButton {border-image: url(:/button/go_next.svg)}"
								"GPushButton:hover {border-image: url(:/button/go_next_hover.svg)}"
								"GPushButton:pressed {border-image: url(:/button/go_next_pressed.svg)}");
	m_preBtn = new GPushButton(this);
	m_preBtn->setFixedSize(48, 48);
	m_preBtn->setStyleSheet("GPushButton {border-image: url(:/button/go_pre.svg)}"
								"GPushButton:hover {border-image: url(:/button/go_pre_hover.svg)}"
								"GPushButton:pressed {border-image: url(:/button/go_pre_pressed.svg)}");
	m_musicListBtn = new GPushButton(this);
	m_musicListBtn->setFixedSize(48, 48);
	m_musicListBtn->setStyleSheet("GPushButton {border-image: url(:/button/music_list.svg)}"
									"GPushButton:hover {border-image: url(:/button/music_list_hover.svg)}"
									"GPushButton:pressed {border-image: url(:/button/music_list_pressed.svg)}");

	QHBoxLayout *btnLayout = new QHBoxLayout(this);
	btnLayout->addStretch(2);
	btnLayout->addWidget(m_preBtn);
	btnLayout->addSpacing(20);
	btnLayout->addWidget(m_playBtn);
	btnLayout->addSpacing(20);
	btnLayout->addWidget(m_nextBtn);
	btnLayout->addSpacing(50);
	btnLayout->addWidget(m_musicListBtn);
	btnLayout->addStretch(1);

	if (m_servProvider && dynamic_cast<IGMusicPlayService *>(m_servProvider))
	{
		IGMusicPlayService *pMusicSer = dynamic_cast<IGMusicPlayService *>(m_servProvider);
		pMusicSer->SetProgressShower(m_progressBar);
		pMusicSer->SetProgressShower(m_lyricView);
		pMusicSer->SetProgressShower(m_progressLabel);
		pMusicSer->SetProgressShower(m_totalTimeLabel);
	}

	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(m_titleBar);
	mainLayout->addSpacing(30);
	mainLayout->addLayout(voiceLayout);
	mainLayout->addWidget(m_lyricView);
	mainLayout->addSpacing(30);
	mainLayout->addLayout(progressLyout);
	mainLayout->addSpacing(10);
	mainLayout->addLayout(btnLayout);
	mainLayout->addSpacing(60);

	setLayout(mainLayout);

	m_musicListWidget = new GFileListWidget();
	m_musicListWidget->hide();
}

void GMusicPlayerMainWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QPixmap bgImg(":/background/background_03.jpg");
	painter.drawPixmap(rect(), bgImg, bgImg.rect());
}

void GMusicPlayerMainWidget::onLyricLoaded(QMap<int, QString>& parsedLyric)
{
	m_lyricView->setLyric(parsedLyric);
}

void GMusicPlayerMainWidget::onCloseApp()
{
	close();
}

void GMusicPlayerMainWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (m_bDrag)
	{
		move(event->globalPos() - m_basePoint);
	}
}

void GMusicPlayerMainWidget::mousePressEvent(QMouseEvent *event)
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

void GMusicPlayerMainWidget::mouseReleaseEvent(QMouseEvent *event)
{
	m_bDrag = false;
}

void GMusicPlayerMainWidget::onMusicListBtnClicked()
{
	m_musicListWidget->show();
}

void GMusicPlayerMainWidget::onMuisicListDblClicked(QString songPath)
{
	if (songPath.isEmpty())
		return;

	if (m_servProvider && dynamic_cast<IGMusicPlayService *>(m_servProvider))
	{
		IGMusicPlayService *pMusicSer = dynamic_cast<IGMusicPlayService *>(m_servProvider);
		pMusicSer->SetVolume(m_voiceBar->getCurProgress());
		pMusicSer->MusicPlay(songPath);
		m_playBtn->setMusicState(GStartPauseButton::play);
	}

	QStringList pathComponent = songPath.split(".");
	if (pathComponent.size() != 2)
		return;
	QString lyricPath = pathComponent[0] + ".lrc";
	loadLyric(lyricPath);
}

void GMusicPlayerMainWidget::onMusicProgressChanged(double curProgress)
{
	if (m_servProvider && dynamic_cast<IGMusicPlayService *>(m_servProvider))
	{
		IGMusicPlayService *pMusicSer = dynamic_cast<IGMusicPlayService *>(m_servProvider);
		pMusicSer->SetPosition(curProgress);
	}
}

void GMusicPlayerMainWidget::onVoiceProgressChanged(double curProgress)
{
	if (m_servProvider && dynamic_cast<IGMusicPlayService *>(m_servProvider))
	{
		IGMusicPlayService *pMusicSer = dynamic_cast<IGMusicPlayService *>(m_servProvider);
		pMusicSer->SetVolume(curProgress);
	}
}