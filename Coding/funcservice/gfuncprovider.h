#ifndef GFUNCPROVIDER_H
#define GFUNCPROVIDER_H

#include "gmusicplayservice.h"
#include "glyricparser.h"

#include <QMediaPlayer>

class GFuncProvider
    : public GLyricParserBase
    , public IGMusicPlayService
{
    Q_OBJECT
public:
    static GFuncProvider* getInstance();
    virtual bool MusicPlay(const QString& fileName) override;
    virtual bool MusicPause() override;
    virtual bool MusicContinue() override;
    virtual bool SetPosition(double curProgress) override;
    virtual bool SetVolume(double curProgress) override;
    virtual void SetProgressShower(IGProgressShower *shower) override;
    virtual void RemoveProgressShower(IGProgressShower *shower) override;

    virtual void parseStart(QString lyricPath) override;

protected:
    GFuncProvider();
    virtual ~GFuncProvider();

protected slots:
    virtual void onPositionChanged(qint64 curPos);

private:
    QMediaPlayer *m_mediaPlayer;
    qint64 m_curDuration;
    qint64 m_curPosition;

    QVector<IGProgressShower *> m_shower;
    GLyricParser *m_lyricParser;
};
#endif // GFUNCPROVIDER_H