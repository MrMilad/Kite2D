#ifndef MAPEDITORCMD_H
#define MAPEDITORCMD_H

#include <Kite/graphic/korthogonalmap.h>
#include <qundostack.h>
#include <vector>
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <qlist.h>
#include <qhash.h>
#include <qgraphicseffect.h>
#include <qpainter.h>
#include <qtablewidget.h>
#include <Kite/graphic/kgraphicdef.h>

struct MarkerItem {
	QGraphicsPixmapItem *gitem;
	Kite::KTileStamp stamp;
	Kite::KColor blend;
};
static void initePixmap(QGraphicsPixmapItem *PItem, const QPixmap &Pixmap, const Kite::KOrthoLayer &Layer) {
	auto pix = Pixmap.copy(Layer.atlas.xpos, Layer.atlas.ypos, Layer.atlas.width, Layer.atlas.height);
	if (Layer.atlas.getFlipH()) {
		pix = pix.transformed(QTransform().scale(-1, 1));
	}

	if (Layer.atlas.getFlipV()) {
		pix = pix.transformed(QTransform().scale(1, -1));
	}

	//blend
	if (Layer.blend != Kite::KColor(Kite::Colors::WHITE) || Layer.blend.getA() < 255) {
		QColor col(Layer.blend.getR(), Layer.blend.getG(),
				   Layer.blend.getB(), Layer.blend.getA());

		auto image = pix.toImage();
		auto alpha = image.alphaChannel();
		for (int x = 0; x != image.width(); ++x) {
			for (int y(0); y != image.height(); ++y) {
				if (qAlpha(image.pixel(x, y)) == 0) continue; // transparrent pixels

				QColor icol(image.pixel(x, y));
				icol.setRed(BLEND_Multiply(icol.red(), col.red()));
				icol.setBlue(BLEND_Multiply(icol.blue(), col.blue()));
				icol.setGreen(BLEND_Multiply(icol.green(), col.green()));
				image.setPixel(x, y, icol.rgb());
			}
		}
		image.setAlphaChannel(alpha);
		PItem->setPixmap(QPixmap::fromImage(image));
		PItem->setOpacity(Layer.blend.getGLA());
	} else {
		PItem->setPixmap(pix);
	}
}

class RaiseLayerCMD : public QUndoCommand {
public:
	RaiseLayerCMD(QGraphicsScene *Scene, Kite::KOrthogonalMap *Map, QList<QGraphicsItemGroup *> *Group,
				  QTableWidget *Table, unsigned short LayerIndex) :
		scene(Scene), map(Map), tview(Table), layerIndex(LayerIndex),
		group(Group) {
		setText("Raise Layer");
	}

	virtual void undo() override {
		map->swapMapLayer(layerIndex, layerIndex + 1);

		group->at(layerIndex)->setZValue(layerIndex + 1);
		group->at(layerIndex + 1)->setZValue(layerIndex);

		group->swap(layerIndex, layerIndex + 1);

		auto item1 = tview->item(layerIndex, 0);
		auto item2 = tview->item(layerIndex + 1, 0);

		auto tempText = item1->text();
		auto tempCheck = item1->checkState();

		item1->setText(item2->text());
		item1->setCheckState(item2->checkState());

		item2->setText(tempText);
		item2->setCheckState(tempCheck);

		map->setModified(true);
		scene->update();
	}

	virtual void redo() override {
		map->swapMapLayer(layerIndex, layerIndex + 1);

		group->at(layerIndex)->setZValue(layerIndex + 1);
		group->at(layerIndex + 1)->setZValue(layerIndex);

		group->swap(layerIndex, layerIndex + 1);

		auto item1 = tview->item(layerIndex, 0);
		auto item2 = tview->item(layerIndex + 1, 0);

		auto tempText = item1->text();
		auto tempCheck = item1->checkState();

		item1->setText(item2->text());
		item1->setCheckState(item2->checkState());

		item2->setText(tempText);
		item2->setCheckState(tempCheck);

		tview->setCurrentItem(item2);
		map->setModified(true);
		scene->update();
	}

private:
	QGraphicsScene *scene;
	Kite::KOrthogonalMap *map;
	QList<QGraphicsItemGroup *> *group;
	QTableWidget *tview;
	unsigned short layerIndex;
};

class LowerLayerCMD : public QUndoCommand {
public:
	LowerLayerCMD(QGraphicsScene *Scene, Kite::KOrthogonalMap *Map, QList<QGraphicsItemGroup *> *Group,
				  QTableWidget *Table, unsigned short LayerIndex) :
		scene(Scene), map(Map), tview(Table), layerIndex(LayerIndex),
		group(Group) {
		setText("Lower Layer");
	}

	virtual void undo() override {
		map->swapMapLayer(layerIndex, layerIndex - 1);

		group->at(layerIndex)->setZValue(layerIndex - 1);
		group->at(layerIndex - 1)->setZValue(layerIndex);

		group->swap(layerIndex, layerIndex - 1);

		auto item1 = tview->item(layerIndex, 0);
		auto item2 = tview->item(layerIndex - 1, 0);

		auto tempText = item1->text();
		auto tempCheck = item1->checkState();

		item1->setText(item2->text());
		item1->setCheckState(item2->checkState());

		item2->setText(tempText);
		item2->setCheckState(tempCheck);

		map->setModified(true);
		scene->update();
	}

	virtual void redo() override {
		map->swapMapLayer(layerIndex, layerIndex - 1);

		group->at(layerIndex)->setZValue(layerIndex - 1);
		group->at(layerIndex - 1)->setZValue(layerIndex);

		group->swap(layerIndex, layerIndex - 1);

		auto item1 = tview->item(layerIndex, 0);
		auto item2 = tview->item(layerIndex - 1, 0);

		auto tempText = item1->text();
		auto tempCheck = item1->checkState();

		item1->setText(item2->text());
		item1->setCheckState(item2->checkState());

		item2->setText(tempText);
		item2->setCheckState(tempCheck);

		tview->setCurrentItem(item2);
		map->setModified(true);
		scene->update();
	}

private:
	QGraphicsScene *scene;
	Kite::KOrthogonalMap *map;
	QList<QGraphicsItemGroup *> *group;
	QTableWidget *tview;
	unsigned short layerIndex;
};

class AddLayerCMD : public QUndoCommand {
public:
	AddLayerCMD(QGraphicsScene *Scene, Kite::KOrthogonalMap *Map, QTableWidget *Table,
				QList<QGraphicsItemGroup *> *Group) :
		scene(Scene), map(Map), group(Group), tview(Table){
		setText("Layer Added");
	}

	virtual void undo() override {
		auto layerGroup = group->back();
		group->pop_back();
		scene->removeItem(layerGroup);
		delete layerGroup;
		scene->update();

		tview->removeRow(tview->rowCount() - 1);
	}

	virtual void redo() override {
		auto layerGroup = new QGraphicsItemGroup();
		group->push_back(layerGroup);
		layerGroup->setZValue(group->size() - 1);
		scene->addItem(layerGroup);

		QString name("Layer " + QString::number(group->size()));
		auto titem = new QTableWidgetItem(name);
		titem->setCheckState(Qt::CheckState::Checked);
		tview->insertRow(tview->rowCount());
		tview->setItem(tview->rowCount() - 1, 0, titem);
		tview->setCurrentItem(titem);
		map->setMapLayerName(group->size() - 1, name.toStdString());

		scene->update();
	}

private:
	QGraphicsScene *scene;
	QList<QGraphicsItemGroup *> *group;
	Kite::KOrthogonalMap *map;
	QTableWidget *tview;
};

class RemoveLayerCMD : public QUndoCommand {
public:
	RemoveLayerCMD(QGraphicsScene *Scene, Kite::KOrthogonalMap *Map, const QVector<QPixmap> *TilesetPixmap,
				   QList<QGraphicsItemGroup *> *Group, QTableWidget *Table) :
		scene(Scene), map(Map), tsetPixmap(TilesetPixmap),
		group(Group), tview(Table){
		setText("Layer Removed");
	}

	virtual void undo() override {
		auto layerGroup = new QGraphicsItemGroup();
		group->push_back(layerGroup);
		layerGroup->setZValue(group->size() - 1);
		scene->addItem(layerGroup);

		for (auto it = editedList.begin(); it != editedList.end(); ++it) {
			auto dim = map->getTileDimension(it->first);
			auto scenePtr = new QGraphicsPixmapItem();
			initePixmap(scenePtr, tsetPixmap->at(it->second.textureID), it->second);
			scenePtr->setPos(dim.left, dim.bottom);
			layerGroup->addToGroup(scenePtr);
			map->setTileLayer(map->convertID(it->first), it->second.layerIndex, it->second);
			map->setScenePtr(map->convertID(it->first), it->second.layerIndex, scenePtr);
		}
		map->setMapLayerName(group->size() - 1, layerName.toStdString());
		scene->addItem(layerGroup);

		editedList.clear();
		map->setModified(true);
		scene->update();

		auto titem = new QTableWidgetItem(layerName);
		titem->setCheckState(Qt::CheckState::Checked);
		tview->insertRow(tview->rowCount());
		tview->setItem(tview->rowCount() - 1, 0, titem);
		map->setMapLayerName(group->size() - 1, layerName.toStdString());
	}

	virtual void redo() override {
		auto layerGroup = group->back();
		group->pop_back();
		scene->removeItem(layerGroup);
		delete layerGroup;
		for (size_t i = 0; i < map->getTilesCount(); ++i) {
			Kite::KOrthoLayer layer;
			if (map->getTileLayer(map->convertID(i), group->size(), layer)) {
				layer.sceneItem = nullptr;
				editedList.push_back({ i, layer });
			}
		}
		layerName = map->getMapLayerName(group->size()).c_str();
		map->removeMapLayer(group->size());
		map->setModified(true);
		scene->update();

		tview->removeRow(tview->rowCount() - 1);
	}

private:
	QGraphicsScene *scene;
	Kite::KOrthogonalMap *map;
	const QVector<QPixmap> *tsetPixmap;
	QVector<QPair<unsigned int, Kite::KOrthoLayer>> editedList; // <tid, layer>
	QList<QGraphicsItemGroup *> *group;
	QString layerName;
	QTableWidget *tview;
};

class ClearLayerCMD : public QUndoCommand {
public:
	ClearLayerCMD(QGraphicsScene *Scene, Kite::KOrthogonalMap *Map, const QVector<QPixmap> *TilesetPixmap,
					  QList<QGraphicsItemGroup *> *Group, unsigned int LayerIndex) :
		scene(Scene), map(Map), tsetPixmap(TilesetPixmap),
		group(Group), layerIndex(LayerIndex){
		setText("Layer Cleared");
	}

	virtual void undo() override {
		auto layerGroup = group->at(layerIndex);
		for (auto it = editedList.begin(); it != editedList.end(); ++it) {
			auto dim = map->getTileDimension(it->first);
			auto scenePtr = new QGraphicsPixmapItem();
			initePixmap(scenePtr, tsetPixmap->at(it->second.textureID), it->second);
			scenePtr->setPos(dim.left, dim.bottom);
			layerGroup->addToGroup(scenePtr);
			map->setTileLayer(map->convertID(it->first), it->second.layerIndex, it->second);
			map->setScenePtr(map->convertID(it->first), layerIndex, scenePtr);
		}
		editedList.clear();
		map->setModified(true);
		scene->update();
	}

	virtual void redo() override {
		auto layerGroup = group->at(layerIndex);
		scene->removeItem(layerGroup);
		delete layerGroup;

		group->operator[](layerIndex) = new QGraphicsItemGroup();
		group->at(layerIndex)->setZValue(layerIndex);
		scene->addItem(group->at(layerIndex));
		for (size_t i = 0; i < map->getTilesCount(); ++i) {
			Kite::KOrthoLayer layer;
			if (map->getTileLayer(map->convertID(i), layerIndex, layer)) {
				layer.sceneItem = nullptr;
				editedList.push_back({ i, layer });
			}
		}
		map->removeMapLayer(layerIndex);
		map->setModified(true);
		scene->update();
	}

private:
	QGraphicsScene *scene;
	Kite::KOrthogonalMap *map;
	const QVector<QPixmap> *tsetPixmap;
	unsigned int layerIndex;
	QVector<QPair<unsigned int, Kite::KOrthoLayer>> editedList;
	QList<QGraphicsItemGroup *> *group;
};

class PushStampLayerCMD : public QUndoCommand {
public:
	PushStampLayerCMD(QGraphicsScene *Scene, Kite::KOrthogonalMap *Map, const QVector<QPixmap> *TilesetPixmap,
					  QList<QGraphicsItemGroup *> *Group, unsigned int TileID, unsigned int TextureID,
					  unsigned int LayerIndex, const QVector<MarkerItem> &List) :
		scene(Scene), map(Map), tsetPixmap(TilesetPixmap), group(Group),
		tileID(TileID), texID(TextureID), layerIndex(LayerIndex), stamp(List) {
		setText("Stamp Brush");
	}

	virtual void undo() override {
		auto layerGroup = group->at(layerIndex);
		for (auto it = editedList.begin(); it != editedList.end(); ++it) {
			Kite::KOrthoLayer layer;
			map->getTileLayer(map->convertID(it->tid), layerIndex, layer);
			QGraphicsPixmapItem *scenePtr = (QGraphicsPixmapItem *)layer.sceneItem;

			//layer was empty
			if (it->wasEmpty) {
				layerGroup->removeFromGroup(scenePtr);
				scene->removeItem(scenePtr);
				delete scenePtr;
				map->removeTileLayer(map->convertID(it->tid), layerIndex);
			} else {
				initePixmap(scenePtr, tsetPixmap->at(it->layer.textureID), it->layer);
				map->setTileLayer(map->convertID(it->tid), layerIndex, it->layer);
			}

		}
		editedList.clear();
		map->setModified(true);
		scene->update();
	}

	virtual void redo() override {
		auto anchDim = map->getTileDimension(tileID);
		for (auto it = stamp.begin(); it != stamp.end(); ++it) {
			// calculate position
			int left = anchDim.left + (int)(it->stamp.col * map->getTileWidth());
			int bottom = anchDim.bottom - (int)(it->stamp.row * map->getTileHeight());

			// culling 
			if (left >= map->getMapWidthPixel() || bottom >= map->getMapHeightPixel()) {
				continue;
			}
			
			// change item pixmap
			unsigned int tid;
			map->getTileID(Kite::KVector2F32(left + 1, bottom + 1), tid);

			// check layer 
			QGraphicsPixmapItem *sceneItem = nullptr;
			Kite::KOrthoLayer currentLayer;
			if (map->getTileLayer(map->convertID(tid), layerIndex, currentLayer)) {
				editedList.push_back(editedLayer(currentLayer, tid, false));
				//layer is exist
				sceneItem = static_cast<QGraphicsPixmapItem *>(currentLayer.sceneItem);
			} else {
				editedList.push_back(editedLayer(currentLayer, tid, true));
				// layer is not exist
				sceneItem = new QGraphicsPixmapItem();
				group->at(layerIndex)->addToGroup(sceneItem);
			}
			short texid = texID;
			if (it->stamp.textureIndex >= 0) texid = it->stamp.textureIndex;

			// create new layer and set it to tile
			Kite::KOrthoLayer newLayer;
			newLayer.atlas = it->stamp.atlas;
			newLayer.textureID = texid;
			newLayer.blend = it->blend;
			
			initePixmap(sceneItem, tsetPixmap->at(texid), newLayer);
			sceneItem->setPos(left, bottom);
			map->setTileLayer(map->convertID(tid), layerIndex, newLayer);
			map->setScenePtr(map->convertID(tid), layerIndex, (void *)sceneItem);
		}
		map->setModified(true);
		scene->update();
	}

private:
	struct editedLayer {
		bool wasEmpty;
		unsigned int tid;
		Kite::KOrthoLayer layer;
		editedLayer():
			wasEmpty(true) {}

		editedLayer(const Kite::KOrthoLayer &Layer, unsigned int TID, bool WasEmpty) :
			layer(Layer), tid(TID), wasEmpty(WasEmpty) {}
	};

	QGraphicsScene *scene;
	Kite::KOrthogonalMap *map;
	const QVector<QPixmap> *tsetPixmap;
	unsigned int tileID;
	unsigned int texID;
	unsigned int layerIndex;
	QVector<MarkerItem> stamp;
	QVector<editedLayer> editedList;
	QList<QGraphicsItemGroup *> *group;
};

class EraserCMD : public QUndoCommand {
public:
	EraserCMD(QGraphicsScene *Scene, Kite::KOrthogonalMap * Map, const QVector<QPixmap> *TilesetPixmap,
			  QList<QGraphicsItemGroup *> *Group, unsigned int TileID, unsigned int LayerIndex,
			  const std::vector<Kite::KTileStamp> &List) :
		scene(Scene), map(Map), tsetPixmap(TilesetPixmap),
		group(Group), tileID(TileID), layerIndex(LayerIndex), stamp(List)
	{
		setText("Push Stamp");
	}

	virtual void undo() override {
		auto layerGroup = group->at(layerIndex);
		for (auto it = editedList.begin(); it != editedList.end(); ++it) {
			auto sitem = new QGraphicsPixmapItem();
			initePixmap(sitem, tsetPixmap->at(it->second.textureID), it->second);
			auto dim = map->getTileDimension(it->first);
			sitem->setPos(dim.left, dim.bottom);
			group->at(layerIndex)->addToGroup(sitem);
			map->setTileLayer(map->convertID(it->first), layerIndex, it->second);
			map->setScenePtr(map->convertID(it->first), layerIndex, sitem);
		}
		editedList.clear();
		map->setModified(true);
		scene->update();
	}

	virtual void redo() override {
		auto anchDim = map->getTileDimension(tileID);
		for (auto it = stamp.begin(); it != stamp.end(); ++it) {

			// calculate position
			int left = anchDim.left + (int)(it->col * map->getTileWidth());
			int bottom = anchDim.bottom - (int)(it->row * map->getTileHeight());

			unsigned int tid;
			map->getTileID(Kite::KVector2F32(left + 1, bottom + 1), tid);

			// check layer 
			Kite::KOrthoLayer currentLayer;
			if (map->getTileLayer(map->convertID(tid), layerIndex, currentLayer)) {
				group->at(layerIndex)->removeFromGroup((QGraphicsPixmapItem *)currentLayer.sceneItem);
				delete (QGraphicsPixmapItem *)currentLayer.sceneItem;
				currentLayer.sceneItem = nullptr;
				editedList.push_back({ tid, currentLayer });
				map->removeTileLayer(map->convertID(tid), layerIndex);
			}
		}

		map->setModified(true);
		scene->update();
	}

private:
	QGraphicsScene *scene;
	Kite::KOrthogonalMap *map;
	const QVector<QPixmap> *tsetPixmap;
	unsigned int tileID;
	unsigned int layerIndex;
	QList<QGraphicsItemGroup *> *group;
	QVector<QPair<unsigned int, Kite::KOrthoLayer>> editedList;
	std::vector<Kite::KTileStamp> stamp;
};

class BlendColorCMD : public QUndoCommand {
public:
	BlendColorCMD(QGraphicsScene *Scene, Kite::KOrthogonalMap * Map,
				  const QVector<QPixmap> *Pixmap, unsigned int TileID,
				  unsigned short Layer, const QColor &Blend, const std::vector<Kite::KTileStamp> &List) :
		scene(Scene), map(Map), tileID(TileID), tsetPixmap(Pixmap),
		layerIndex(Layer) , blend(Blend), stamp(List)
	{
		setText("Change Blend Color");
	}

	virtual void undo() override {
		for (auto it = editedList.begin(); it != editedList.end(); ++it) {
			Kite::KOrthoLayer layer;
			map->getTileLayer(map->convertID(it->first), layerIndex, layer);
			auto sitem = (QGraphicsPixmapItem *)layer.sceneItem;
			layer.blend = it->second;
			initePixmap(sitem, tsetPixmap->at(layer.textureID), layer);
			map->setTileLayer(map->convertID(it->first), layerIndex, layer);
		}
		editedList.clear();
		scene->update();
		map->setModified(true);
	}

	virtual void redo() override {
		// redraw tiles
		auto anchDim = map->getTileDimension(tileID);
		for (auto it = stamp.begin(); it != stamp.end(); ++it) {
			// calculate position
			int left = anchDim.left + (int)(it->col * map->getTileWidth());
			int bottom = anchDim.bottom - (int)(it->row * map->getTileHeight());

			unsigned int tid;
			map->getTileID(Kite::KVector2F32(left + 1, bottom + 1), tid);
			Kite::KOrthoLayer layer;
			if (map->getTileLayer(map->convertID(tid), layerIndex, layer)) {
				auto sitem = (QGraphicsPixmapItem *)layer.sceneItem;
				editedList.push_back({ tid, layer.blend });
				layer.blend = Kite::KColor(blend.red(), blend.green(), blend.blue(), blend.alpha());
				initePixmap(sitem, tsetPixmap->at(layer.textureID), layer);
				map->setTileLayer(map->convertID(tid), layerIndex, layer);
			}
		}
		scene->update();
		map->setModified(true);
	}

private:
	QGraphicsScene *scene;
	Kite::KOrthogonalMap *map;
	unsigned int tileID;
	unsigned short layerIndex;
	QColor blend;
	QVector<QPair<unsigned int, Kite::KColor>> editedList;
	const QVector<QPixmap> *tsetPixmap;
	std::vector<Kite::KTileStamp> stamp;
};

class FlipCMD : public QUndoCommand {
public:
	FlipCMD(QGraphicsScene *Scene, Kite::KOrthogonalMap * Map,
				  const QVector<QPixmap> *Pixmap, unsigned int TileID,
				  unsigned short Layer, const std::vector<Kite::KTileStamp> &List, bool FlipH) :
		scene(Scene), map(Map), tileID(TileID), tsetPixmap(Pixmap),
		layerIndex(Layer), stamp(List), fliph(FlipH){
		setText("Flip");
	}

	virtual void undo() override {
		for (auto it = editedList.begin(); it != editedList.end(); ++it) {
			Kite::KOrthoLayer layer;
			map->getTileLayer(map->convertID((*it)), layerIndex, layer);
			auto sitem = (QGraphicsPixmapItem *)layer.sceneItem;
			if (fliph) {
				layer.atlas.flipH();
			} else {
				layer.atlas.flipV();
			}
			initePixmap(sitem, tsetPixmap->at(layer.textureID), layer);
			map->setTileLayer(map->convertID((*it)), layerIndex, layer);
		}
		editedList.clear();
	}

	virtual void redo() override {
		// redraw tiles
		auto anchDim = map->getTileDimension(tileID);
		for (auto it = stamp.begin(); it != stamp.end(); ++it) {
			// calculate position
			int left = anchDim.left + (int)(it->col * map->getTileWidth());
			int bottom = anchDim.bottom - (int)(it->row * map->getTileHeight());

			unsigned int tid;
			map->getTileID(Kite::KVector2F32(left + 1, bottom + 1), tid);
			Kite::KOrthoLayer layer;
			if (map->getTileLayer(map->convertID(tid), layerIndex, layer)) {
				auto sitem = (QGraphicsPixmapItem *)layer.sceneItem;
				if (fliph) {
					layer.atlas.flipH();
				} else {
					layer.atlas.flipV();
				}
				initePixmap(sitem, tsetPixmap->at(layer.textureID), layer);

				editedList.push_back(tid);
				map->setTileLayer(map->convertID(tid), layerIndex, layer);
			}
		}
		scene->update();
	}

private:
	QGraphicsScene *scene;
	Kite::KOrthogonalMap *map;
	unsigned int tileID;
	unsigned short layerIndex;
	std::vector<unsigned int> editedList;
	const QVector<QPixmap> *tsetPixmap;
	std::vector<Kite::KTileStamp> stamp;
	bool fliph;
};

#endif // MAPEDITORCMD_H