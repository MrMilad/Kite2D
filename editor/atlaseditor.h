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

private slots:
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

#endif // ATLASEDITOR_H
