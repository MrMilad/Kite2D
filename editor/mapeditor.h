#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include "tabwidget.h"
#include <Kite/graphic/korthogonalmap.h>

class QUndoStack;

class MapEditor : public TabWidget{
public:
	MapEditor(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent = nullptr);

	void inite() override;
	bool saveChanges() override;
	void reload() override;

	static TabWidget *factory(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent = nullptr) {
		return new MapEditor(Res, KInfo, Parent);
	}

private:
	Kite::KOrthogonalMap *orthoMap;
	QUndoStack *ustack;
};

#endif // MAPEDITOR_H