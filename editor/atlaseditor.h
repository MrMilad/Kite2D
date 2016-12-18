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
	AtlasEditor(Kite::KResource *Res, Kite::KIStream *Stream, QWidget *Parent = nullptr);
	~AtlasEditor();

	void inite() override;
	void saveChanges() override;
	void reload() override;

	static TabWidget *factory(Kite::KResource *Res, Kite::KIStream *Stream, KiteInfo *KInfo, QWidget *Parent = nullptr) {
		return new AtlasEditor(Res, Stream, Parent);
	}

	void onRemoveRes(const QString &Name, Kite::RTypes Type) override;

private slots:
	void createNew();
	void importFlashXML();
	void importBMFontXML();
	void listItemChange();

protected:
	bool eventFilter(QObject *Obj, QEvent *Event);
	void procData(const QString &ImagePatch, const QVector<Kite::KAtlasItem> &Items);

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
		}
	}
};

#endif // ATLASEDITOR_H
