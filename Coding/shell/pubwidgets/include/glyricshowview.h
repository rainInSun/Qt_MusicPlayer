#ifndef GLYRICSHOWVIEW_H
#define GLYRICSHOWVIEW_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "gmusicplayservice.h"

class GLyricItem : public QGraphicsItem
{
public:
    GLyricItem(const QString& lyricStr, const QColor& textColor = Qt::gray,
               const QFont& textFont = QFont(), int textPixelSize = 20, int width = 100, int height = 30);
    virtual ~GLyricItem();

    void setTextColor(const QColor& textColor);
    void setTextFont(const QFont& textFont);
    void setTextPixelSize(int textSize);
    void setWidth(int width);
    void setHeight(int height);
    int width();
    int height();

protected:

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

private:
    QColor m_color;
    QFont m_font;
    int m_textSize;
    QString m_text;
    int m_width;
    int m_height;
};

class GLyricScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GLyricScene(QObject *parent = nullptr);
    virtual ~GLyricScene();
};

class GLyricShowView : public QGraphicsView, public IGProgressShower
{
    Q_OBJECT
public:
    GLyricShowView(const QMap<int, QString>& parsedLyric, QWidget *parent = nullptr);
    virtual ~GLyricShowView();

    void startAnimation();
    void stopAnimation();
    void setLyric(const QMap<int, QString>& parsedLyric);
    virtual void ShowProgress(qint64 curTime, qint64 totalTime) override;
    void setIsLoadedLyric(bool isLoad);

protected:
    void init();
    void onTime();

private:
    QVector<GLyricItem *> m_lyricItems;
    GLyricScene *m_lyricScene;
    GLyricItem *m_curCenterItem;
    QMap<int, QString> m_parsedLyric;
    QTimer *m_timer;
    int m_curCenterOnHeight;
    int m_lstCenterOnHeight;
    int m_moveDistance;
    bool m_isLoadedLyric;

};

#endif // GLYRICSHOWVIEW_H
