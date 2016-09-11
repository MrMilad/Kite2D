#include "mapeditor.h"
#include "atlaseditor.h"
#include "Kite/core/kfistream.h"
#include <QtWidgets>
#include <qundostack.h>

MapEditor::MapEditor(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent) :
	TabWidget(Res, KInfo, Parent),
	orthoMap((Kite::KOrthogonalMap *)Res),
	ustack(new QUndoStack(Parent))
	{}

void MapEditor::inite() {

}

bool MapEditor::saveChanges() {
	return true;
}

void MapEditor::reload() {

}