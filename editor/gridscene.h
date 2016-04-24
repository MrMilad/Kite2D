#ifndef GRIDSCENE
#define GRIDSCENE

#include <QGraphicsScene>
#include <QVector>
#include <QPainter>
#include <QColor>

class GridScene : public QGraphicsScene
{
public:
    GridScene(quint32 Width, quint32 Height, float TileWidth, float TileHeight);

    inline void setLineColor(const QColor &Color) { lcolor = Color; }
    inline void setBackColor(const QColor &Color) { bcolor = Color; }

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    void reshape();
    QVector<QLine> lines;
    quint32 swidth;
    quint32 sheight;
    float twidth;
    float theight;
    QColor lcolor;
    QColor bcolor;
};

#endif // GRIDSCENE

