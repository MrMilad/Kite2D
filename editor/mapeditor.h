#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include "tabwidget.h"
#include <Kite/graphic/korthogonalmap.h>
#include <Kite/graphic/korthotilestamp.h>

class QUndoStack;
class QGraphicsScene;
class GridScene;
class QGraphicsItem;
class QLabel;
class QTabWidget;
class QComboBox;
class QSpinBox;

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

	void recreateMarker(const Kite::KOrthoTileStamp *Stamp);
	void redrawMarker(unsigned int AnchorID, bool OutOfView);
	void drawPushedTiles();

	void selectTintColor();

	void stampTool(bool Check);
	void selectTool(bool check);
	void eraserTool(bool Check);

	inline auto getMap() { return orthoMap; }

protected:
	bool eventFilter(QObject *Obj, QEvent *Event);

private:
	Kite::KOrthogonalMap *orthoMap;
	QUndoStack *ustack;
	GridScene *gscene;
	QLabel *lblWarning;
	QComboBox *cmbStampMode;
	QSpinBox *spnLayer;
	QString totalStackedSize;
	QTabWidget *tileset;
	QColor tint;
	QVector<GridScene *> viewPool;
	QVector<QGraphicsItem *> stampList;
	QAction *actStamp;
	QAction *actSelect;
	QAction *actEraser;
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