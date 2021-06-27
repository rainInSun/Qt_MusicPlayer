#include "glyricparser.h"

#include <QFile>
#include <QDebug>
#include <QRegularExpression>
#include <QString>
#include <QTextCodec>
#include <QThread>

GLRCLyricParserWorker::GLRCLyricParserWorker()
{

}

GLRCLyricParserWorker::~GLRCLyricParserWorker()
{

}

void GLRCLyricParserWorker::parseLyric(QString lyricPath)
{
    QFile lyricFile(lyricPath);
	if (!lyricFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		emit sigLyricParseFinish(QMap<int, QString>());
		return;
	}

    QVector<int> time;
    QMap<int, QString> parsedLyric;
    QTextStream in(&lyricFile);

    while(!in.atEnd())
    {
        QString lineStr = in.readLine();
        // qDebug()<<lineStr;
        QRegularExpression rex("\\[(\\d+)?:(\\d+)?(\\.\\d+)?\\]");
        int offset = 0;
        int capLen = 1;
        while (capLen > 0)
        {
            QRegularExpressionMatch  match = rex.match(lineStr, offset);
            capLen = match.capturedLength();
            if (capLen > 0)
                offset += capLen;
            if (match.hasMatch())
            {
                int totalTime = match.captured(1).toInt() * 60000 + match.captured(2).toInt() * 1000 + match.captured(3).toInt();
                time.push_back(totalTime);
            }
        }

        for (int i = 0; i < time.size(); i++)
        {
            parsedLyric.insert(time[i], lineStr.right(lineStr.length() - offset));
        }
		time.clear();
    }

    emit sigLyricParseFinish(parsedLyric);
}

///////////////////////////////////////////////////////////////////////////////////
GLyricParser::GLyricParser()
    : m_worker(nullptr)
{
	qRegisterMetaType<QMap<int, QString>>("QMap<int, QString>&");
    m_worker = new GLRCLyricParserWorker();
    m_worker->moveToThread(&m_workThread);

    connect(this, &GLyricParser::sigLyricParseBegin, m_worker, &GLRCLyricParserWorker::parseLyric);
    connect(m_worker, &GLRCLyricParserWorker::sigLyricParseFinish, this, &GLyricParser::parseFinish);
}

GLyricParser::~GLyricParser()
{
    m_workThread.quit();
    m_workThread.wait();
    delete m_worker;
}

void GLyricParser::parseStart(QString lyricPath)
{
    m_workThread.start();
    emit sigLyricParseBegin(lyricPath);
}

void GLyricParser::parseFinish(QMap<int, QString>& parsedLyric)
{
    emit sigLyricParseFinish(parsedLyric);
}