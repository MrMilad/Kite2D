#ifndef SHPROGEDITOR_H
#define SHPROGEDITOR_H

#include "tabwidget.h"
#include <Kite/graphic/kshaderprogram.h>

class QComboBox;
class ShProgEditor: public TabWidget{
public:
    ShProgEditor(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent = nullptr);

	void inite() override;
	bool saveChanges() override;
	void reload() override;

	static TabWidget *factory(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent = nullptr) {
		return new ShProgEditor(Res, KInfo, Parent);
	}

protected:
	bool eventFilter(QObject *Obj, QEvent *Event);

private:
	QComboBox *cmbVert;
	QComboBox *cmbFrag;
	QComboBox *cmbGeom;
    Kite::KShaderProgram *prog;
};

#endif // SHPROGEDITOR_H
