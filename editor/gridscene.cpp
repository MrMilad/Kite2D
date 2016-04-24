#include <gridscene.h>
#include <QGraphicsItem>
GridScene::GridScene(quint32 Width, quint32 Height, float TileWidth, float TileHeight) :
    QGraphicsScene(0, 0, Width * TileWidth, Height * TileHeight),
    swidth(Width),
    sheight(Height),
    twidth(TileWidth),
    theight(TileHeight),
    lcolor(Qt::gray),
    bcolor(Qt::darkGray)
{
    reshape();
}

void GridScene::drawBackground(QPainter *painter, const QRectF &rect){
    painter->fillRect(rect, bcolor);
	QPen pen(lcolor);
	pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    painter->drawLines(lines.data(), lines.size());
}

void GridScene::reshape(){
    lines.clear();
    lines.reserve(swidth * sheight);

    for (quint32 i = 0; i <= sheight; i++){
        QLine line(0, i * theight, swidth * twidth, i * theight);
        lines.push_back(line);
    }

    for (quint32 i = 0; i <= swidth; i++){
        QLine line(i * twidth, 0, i * twidth, sheight * theight);
        lines.push_back(line);
    }
}
