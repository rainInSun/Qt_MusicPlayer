#ifndef GMUSICPLAYSERVICE_H
#define GMUSICPLAYSERVICE_H

#define interface struct

#include <QString>
#include <QObject>

interface IUnknown
{
    IUnknown() {};
    virtual ~IUnknown() {};
};

class GLyricParserBase : public QObject, virtual public IUnknown
{
    Q_OBJECT
public:
	GLyricParserBase() {};
	virtual ~GLyricParserBase() {};

    virtual void parseStart(QString lyricPath) = 0;

signals:
    void sigLyricParseFinish(QMap<int, QString>& parsedLyric);
};

interface IGProgressShower : virtual public IUnknown
{
	virtual void ShowProgress(qint64 curTime, qint64 totalTime) = 0;
};

interface IGMusicPlayService : virtual public IUnknown
{
    virtual bool MusicPlay(const QString& fileName) = 0;
    virtual bool MusicPause() = 0;
    virtual bool MusicContinue() = 0;
    virtual bool SetPosition(double curProgress) = 0;
    virtual bool SetVolume(double curProgress) = 0;
    virtual void SetProgressShower(IGProgressShower *shower) = 0;
    virtual void RemoveProgressShower(IGProgressShower *shower) = 0;
};

#endif // GMUSICPLAYSERVICE_H