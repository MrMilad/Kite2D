#ifndef MAPEDITORCMD_H
#define MAPEDITORCMD_H

#include <Kite/graphic/korthogonalmap.h>
#include <qundostack.h>
#include <vector>
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <qlist.h>
#include <qhash.h>

class PushStampCMD : public QUndoCommand {
public:
	PushStampCMD(QGraphicsScene *Scene, Kite::KOrthogonalMap * Map, unsigned TextureID,
			  unsigned int TileID, const std::vector<Kite::KTileStamp> &List, unsigned short TileIndex):
		scene(Scene), map(Map), texID(TextureID), tileID(TileID),
		stamp(List), tindex(TileIndex)
	{
		setText("Push Stamp");
	}

	virtual void undo() override {
		// pop tiles
		map->popTileStamp(map->convertID(tileID), stamp);
		map->setModified(true);

		// remove items from scene
		auto anchDim = map->getTileDimension(tileID);
		int bottom = 0;
		int left = 0;
		for (auto it = stamp.begin(); it != stamp.end(); ++it) {
				left = anchDim.left + (int)(it->col * map->getTileWidth());
				bottom = anchDim.bottom - (int)(it->row * map->getTileHeight());

				auto items = scene->items(QRectF(left, bottom, map->getTileWidth(), map->getTileHeight()));
				for (auto iit = items.begin(); iit != items.end(); ++iit) {
					if ((*iit)->zValue() < 1) { // ignore marker
						scene->removeItem((*iit));
						delete (*iit);
						break;
					}
				}
		}
	}

	virtual void redo() override {
		// push tiles
		map->pushTileStamp(map->convertID(tileID), stamp, tindex, Kite::KColor(Kite::Colors::WHITE));
		map->setModified(true);

		// create and draw tiles
		auto atlasArray = map->getTileset();
		Kite::KImage image;
		atlasArray->getContiner()->at(texID)->getTexture()->getImage(image);

		QImage qimage(image.getPixelsData(), image.getWidth(), image.getHeight(), QImage::Format::Format_RGBA8888);

		auto anchDim = map->getTileDimension(tileID);
		int bottom = 0;
		int left = 0;

		for (auto it = stamp.begin(); it != stamp.end(); ++it) {
			auto item = new QGraphicsPixmapItem(QPixmap::fromImage(
				qimage.copy(it->atlas.xpos, it->atlas.ypos, it->atlas.width, it->atlas.height)));

			left = anchDim.left + (int)(it->col * map->getTileWidth());
			bottom = anchDim.bottom - (int)(it->row * map->getTileHeight());

			item->setPos(left, bottom);
			scene->addItem(item);
		}
		scene->update();
	}

private:
	QGraphicsScene *scene;
	Kite::KOrthogonalMap *map;
	unsigned int tileID;
	unsigned int texID;
	std::vector<Kite::KTileStamp> stamp;
	unsigned short tindex;
};

class EraserCMD : public QUndoCommand {
public:
	EraserCMD(QGraphicsScene *Scene, Kite::KOrthogonalMap * Map, unsigned int TileID) :
		scene(Scene), map(Map), tileID(TileID)
	{
		setText("Push Stamp");
	}

	virtual void undo() override {
		auto atlasArray = map->getTileset();
		Kite::KImage image;
		auto dim = map->getTileDimension(tileID);
		QHash<unsigned int, QImage> pmMap;

		// add stack to map
		for (size_t i = 0; i < texList.size(); ++i) {
			map->pushTile(map->convertID(tileID), texList[i], texIndList[i], tintList[i]);

			// create items and add to scene
			// create and draw tiles
			if (pmMap.find(texIndList[i]) == pmMap.end()) {
				atlasArray->getContiner()->at(texIndList[i])->getTexture()->getImage(image);
				pmMap.insert(texIndList[i], QImage(image.getPixelsData(), image.getWidth(), image.getHeight(), QImage::Format::Format_RGBA8888));
			}

			auto item = new QGraphicsPixmapItem(QPixmap::fromImage(pmMap[texIndList[i]]
																   .copy(texList[i].xpos, texList[i].ypos, texList[i].width, texList[i].height)));

			item->setPos(dim.left, dim.bottom);
			scene->addItem(item);
		}

		texList.clear();
		texIndList.clear();
		tintList.clear();
		scene->update();
	}

	virtual void redo() override {
		// retrive stacked tiles
		std::vector<Kite::KOrthoTile> stack;
		map->queryTiles(map->convertID(tileID), stack);

		for (auto it = stack.begin(); it != stack.end(); ++it) {
			// save information
			texList.push_back(it->atlas);
			texIndList.push_back(it->textureIndex);
			tintList.push_back(it->tint);
		}

		// clear tiles
		map->clearTile(map->convertID(tileID));
		map->setModified(true);


		// clear scene items
		auto dim = map->getTileDimension(tileID);
		auto items = scene->items(QRectF(dim.left, dim.bottom, map->getTileWidth(), map->getTileHeight()));

		for (auto it = items.begin(); it != items.end(); ++it) {
			if ((*it)->zValue() < 1) { // ignore marker
				scene->removeItem((*it));
				delete (*it);
			}
		}

		scene->update();
	}

private:
	QGraphicsScene *scene;
	Kite::KOrthogonalMap *map;
	unsigned int tileID;
	QList<Kite::KAtlasItem> texList;
	QList<unsigned int> texIndList;
	QList<Kite::KColor> tintList;
};

#endif // MAPEDITORCMD_H