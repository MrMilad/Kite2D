#ifndef TEXTUREEDITOR_H
#define TEXTUREEDITOR_H

#include "tabwidget.h"
#include <Kite/graphic/ktexture.h>

class QComboBox;
class QSpinBox;
class QLabel;
class TextureEditor: public TabWidget{
public:
    TextureEditor(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent = nullptr);

    void inite() override;

    bool saveChanges() override;

	void reload() override;

	static TabWidget *factory(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent = nullptr) {
		return new TextureEditor(Res, KInfo, Parent);
	}

private slots:
	void loadImage();
	void createColor();
	void resizeWidth(int Width);
	void resizeHeight(int Height);

protected:
	bool eventFilter(QObject *Obj, QEvent *Event);

private:
	QColor color;
	QComboBox *cmbFilter;
	QComboBox *cmbWrap;
	QSpinBox *spinY;
	QSpinBox *spinX;
	QLabel *lblImageView;
	QLabel *lblPos;
	Kite::KTexture *texture;
};

#endif // TEXTUREEDITOR_H
