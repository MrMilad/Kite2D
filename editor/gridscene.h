#ifndef GRIDSCENE
#define GRIDSCENE

#include <QGraphicsScene>
#include <QVector>
#include <QPainter>
#include <QColor>
#include <Kite/graphic/korthotilestamp.h>

class QEvent;
class QGraphicsView;
class QGraphicsItem;
class GridScene : public QGraphicsScene
{
	Q_OBJECT
public:
    GridScene(bool Selectable = false, bool ShowAnchor = false, QObject *Parent = nullptr);
	~GridScene();

    inline void setLineColor(const QColor &Color) { lcolor = Color; }
	inline void setPenStyle(Qt::PenStyle Style) { pstyle = Style; }
	void reshape(quint32 Width, quint32 Height, quint32 TileWidth, quint32 TileHeight);
	void selectTile(unsigned int TileID);
	inline auto getView() const { return gview; }
	inline const auto getStamp() const { return &stamp; }
	void setSelectable(bool Selectable);
	void setShowGrid(bool Show);

signals:
	void stampChanged(const Kite::KOrthoTileStamp *Stamp);

protected:
    void drawForeground(QPainter *painter, const QRectF &rect) override;
	virtual bool event(QEvent *Event) override;

private:
	void drawQueriedTiles(bool ClearOnly = false);
	void drawSelectedTiles();

    QVector<QLine> lines;
    QColor lcolor;
	bool selectable;
	bool showAnchor;
	bool showGrid;
	Qt::PenStyle pstyle;
	QGraphicsPixmapItem *marker;
	QGraphicsView *gview;
	Kite::KOrthoTileStamp stamp;
	QVector<QGraphicsItem *> slist;
	QVector<QGraphicsItem *> qlist;
};

#endif // GRIDSCENE

