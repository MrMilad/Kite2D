#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include "tabwidget.h"
#include <Kite/graphic/korthogonalmap.h>
#include <Kite/graphic/korthotilestamp.h>
#include <qgraphicsitem.h>
#include <qlist.h>
#include "mapeditorcmd.h"

class QUndoStack;
class QGraphicsScene;
class GridScene;
class QGraphicsItem;
class QLabel;
class QTabWidget;
class QComboBox;
class QSpinBox;
class QToolButton;
class QTableWidget;
class QTableWidgetItem;

class MapEditor : public TabWidget{
public:
	MapEditor(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent = nullptr);

	void inite() override;
	bool saveChanges() override;
	void reload() override;

	static TabWidget *factory(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent = nullptr) {
		return new MapEditor(Res, KInfo, Parent);
	}

	void onRemoveRes(Kite::RTypes Type) override;

private slots:
	void createNew();
	void tilesetChanged(int);

	void initeMarkerPixmap(QPixmap *Pixmap, MarkerItem &Marker);
	void recreateMarker(const std::vector<Kite::KTileStamp> *Stamp);
	void redrawMarker(unsigned int AnchorID, bool OutOfView);
	void drawPushedTiles();

	void stampTool(bool Check);
	void selectTool(bool check);
	void eraserTool(bool Check);

	void blendColorPicker();
	void selectBlend();
	void selectFlip();
	void selectDuplicate();

	void addLayer();
	void removeLayer();
	void clearLayer();
	void lowerLayer();
	void raiseLayer();
	void layerItemChanged(QTableWidgetItem * Item);

	void manageSceneSelection(const std::vector<Kite::KTileStamp> *Stamp);

protected:
	bool eventFilter(QObject *Obj, QEvent *Event);

private:
	Kite::KOrthogonalMap *orthoMap;
	QUndoStack *ustack;
	GridScene *gscene;
	QLabel *lblWarning;
	QToolButton *btnBlend;
	QTabWidget *tileset;
	QTableWidget *layerTable;
	QColor blend;
	QVector<GridScene *> viewPool;
	QVector<MarkerItem> markerList;
	QAction *actStamp;
	QAction *actSelect;
	QAction *actEraser;
	QVector<QPixmap> tilesetPixmap;
	QList<QGraphicsItemGroup *> layerList;
};

class OrthoMapDepChecker {
public:
	static void onResRemove(Kite::KResource *Self, Kite::KResource *RemovedRes) {
		auto map = (Kite::KOrthogonalMap *)Self;
		if (RemovedRes->getType() == Kite::RTypes::TextureGroup && RemovedRes == map->getTileset()) {
			map->setTileset(nullptr);
			Self->setModified(true);
		}
	}
};

#endif // MAPEDITOR_H