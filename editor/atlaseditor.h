#ifndef ATLASEDITOR_H
#define ATLASEDITOR_H

#include <Kite/graphic/katlastexture.h>
#include "tabwidget.h"

class QListWidget;
class QGraphicsScene;
class QGraphicsItem;
class QGraphicsView;
class QLabel;
class QGraphicsPixmapItem;
class AtlasEditor : public TabWidget {

public:
	AtlasEditor(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent = nullptr);
	~AtlasEditor();

	void inite() override;
	bool saveChanges() override;
	void reload() override;

	static TabWidget *factory(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent = nullptr) {
		return new AtlasEditor(Res, KInfo, Parent);
	}

	void onRemoveRes(Kite::RTypes Type) override;

private slots:
	void createNew();
	void importXML();
	void listItemChange();

protected:
	bool eventFilter(QObject *Obj, QEvent *Event);

private:
	QGraphicsPixmapItem *sheet;
	QGraphicsItem *marker;
	QGraphicsScene *gscene;
	QGraphicsView *gview;
	QListWidget *listView;
	QLabel *lblPos;
	Kite::KAtlasTexture *atlas;
};

class AtlasDepChecker {
public:
	static void onResRemove(Kite::KResource *Self, Kite::KResource *RemovedRes) {
		auto atlas = (Kite::KAtlasTexture *)Self;
		if (RemovedRes->getType() == Kite::RTypes::Texture && RemovedRes == atlas->getTexture()) {
			atlas->setTexture(nullptr);
			Self->setModified(true);
		}
	}
};

#endif // ATLASEDITOR_H
