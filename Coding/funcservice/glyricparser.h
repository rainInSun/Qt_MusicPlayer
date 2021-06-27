#ifndef GLYRICPARSER_H
#define GLYRICPARSER_H

#include <QMap>
#include <QObject>
#include <QThread>

class GLRCLyricParserWorker : public QObject
{
    Q_OBJECT
public:
    GLRCLyricParserWorker();
    virtual ~GLRCLyricParserWorker();

    virtual void parseLyric(QString lyricPath);

signals:
    void sigLyricParseFinish(QMap<int, QString>& parsedLyric);
};

class GLyricParser : public QObject
{
    Q_OBJECT
public:
    GLyricParser();
    virtual ~GLyricParser();

    void parseStart(QString lyricPath);
    void parseFinish(QMap<int, QString>& parsedLyric);

signals:
    void sigLyricParseFinish(QMap<int, QString>& parsedLyric);
    void sigLyricParseBegin(QString lyricPath);

private:
	GLRCLyricParserWorker *m_worker;
    QThread m_workThread;
};

#endif // GLYRICPARSER_H
