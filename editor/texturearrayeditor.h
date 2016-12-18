#ifndef TEXTUREARRAYEDITOR_H
#define TEXTUREARRAYEDITOR_H

#include "tabwidget.h"
#include <Kite/graphic/katlastexturearray.h>

class QListWidget;
class QLabel;
class QComboBox;
class QAction;
class QFrame;

class TextureArrayEditor : public TabWidget{
public:
	TextureArrayEditor(Kite::KResource *Res, Kite::KIStream *Stream, KiteInfo *KInfo, QWidget *Parent = nullptr);

	void inite() override;
	void saveChanges() override;
	void reload() override;

	static TabWidget *factory(Kite::KResource *Res, Kite::KIStream *Stream, KiteInfo *KInfo, QWidget *Parent = nullptr) {
		return new TextureArrayEditor(Res, Stream, KInfo, Parent);
	}

	void onRemoveRes(const QString &Name, Kite::RTypes Type) override;
	void onAddRes(const QString &Name, Kite::RTypes Type) override;

private slots:
void createNew();
void flistContextMenu(const QPoint &pos);
void slistContextMenu(const QPoint &pos);

private:
	void fillFinalList();
	void fillSelectList();
	void add();
	void swap();
	void remove();

	QComboBox *cmbFilter;
	QComboBox *cmbWrap;
	QFrame *botFrame;
	QListWidget *finalList;
	QListWidget *selectList;
	QAction *actAdd;
	QAction *actSwap;
	QAction *actRemove;
	Kite::KAtlasTextureArray *atlasArray;
};

class TextureArrayDepChecker {
public:
	static void onResRemove(Kite::KResource *Self, Kite::KResource *RemovedRes) {
		auto tarray = (Kite::KAtlasTextureArray *)Self;
		auto alist = tarray->getContiner();
		if (RemovedRes->getType() == Kite::RTypes::AtlasTexture) {
			for (size_t i = 0; i < alist->size(); ++i) {
				if (alist->operator[](i) == (Kite::KAtlasTexture *)RemovedRes) {
					alist->operator[](i) = nullptr;
					break;
				}
			}
		}
	}
};

#endif // TEXTUREARRAYEDITOR_H