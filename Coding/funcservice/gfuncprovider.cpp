#include "gfuncprovider.h"
#include <QString>
#include <QMediaPlayer> 

GFuncProvider::GFuncProvider()
	: m_mediaPlayer(nullptr)
	, m_curDuration(0)
	, m_curPosition(0)
	, m_shower(QVector<IGProgressShower *>())
	, m_lyricParser(nullptr)
{
	m_mediaPlayer = new QMediaPlayer();
	connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &GFuncProvider::onPositionChanged);

	m_lyricParser = new GLyricParser();
	connect(m_lyricParser, &GLyricParser::sigLyricParseFinish, this, &GFuncProvider::sigLyricParseFinish);
}

GFuncProvider::~GFuncProvider()
{
	if (m_mediaPlayer)
		delete m_mediaPlayer;

	if (m_lyricParser)
		delete m_lyricParser;
}

bool GFuncProvider::MusicPlay(const QString& fileName)
{
	m_mediaPlayer->setMedia(QUrl::fromLocalFile(fileName));
	m_mediaPlayer->setVolume(30);
	m_mediaPlayer->play();

	m_curDuration = m_mediaPlayer->duration();

	return true;
}

bool GFuncProvider::MusicPause()
{
	m_mediaPlayer->pause();
	return true;
}

bool GFuncProvider::MusicContinue()
{
	m_mediaPlayer->play();
	return true;
}

bool GFuncProvider::SetPosition(double curProgress)
{
	m_curDuration = m_mediaPlayer->duration();
	if (qAbs(m_curDuration) < 0.0000001)
		return false;

	if (curProgress < 0 || curProgress > 1)
		return false;

	double setPos = (double)m_curDuration * curProgress;
	m_mediaPlayer->setPosition(setPos);
	return true;
}

bool GFuncProvider::SetVolume(double curProgress)
{
	if (curProgress < 0 || curProgress > 1)
		return false;

	double setPos = (double)100 * curProgress;
	m_mediaPlayer->setVolume(setPos);
	return true;
}

void GFuncProvider::SetProgressShower(IGProgressShower *shower)
{
	if (!shower)
		return;
	m_shower.push_back(shower);
}

void GFuncProvider::RemoveProgressShower(IGProgressShower *shower)
{
	if (!shower)
		return;
	for (int i = 0; i < m_shower.size(); i++)
	{
		if (shower == m_shower[i])
		{
			m_shower.remove(i);
			break;
		}
	}
}

GFuncProvider* GFuncProvider::getInstance()
{
	static GFuncProvider s_instance;
	return &s_instance;
}

void GFuncProvider::onPositionChanged(qint64 curPos)
{
	m_curPosition = curPos;
	m_curDuration = m_mediaPlayer->duration();
	if (qAbs(m_curDuration) < 0.000001)
		return;

	for (int i = 0; i < m_shower.size(); i++)
	{
		m_shower[i]->ShowProgress(m_curPosition, m_curDuration);
	}
}

void GFuncProvider::parseStart(QString lyricPath)
{
	m_lyricParser->parseStart(lyricPath);
}