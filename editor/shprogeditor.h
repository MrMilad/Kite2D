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

	void onRemoveRes(Kite::RTypes Type) override;

protected:
	bool eventFilter(QObject *Obj, QEvent *Event);

private:
	QComboBox *cmbVert;
	QComboBox *cmbFrag;
	QComboBox *cmbGeom;
    Kite::KShaderProgram *prog;
};

class SHProgDepChecker {
public:
	static void onResRemove(Kite::KResource *Self, Kite::KResource *RemovedRes) {
		auto prog = (Kite::KShaderProgram *)Self;
		if (RemovedRes->getType() == Kite::RTypes::Shader) {
			if (prog->getShader(Kite::ShaderType::VERTEX) == RemovedRes) {
				prog->setShader(nullptr, Kite::ShaderType::VERTEX);
			}
			if (prog->getShader(Kite::ShaderType::FRAGMENT) == RemovedRes) {
				prog->setShader(nullptr, Kite::ShaderType::FRAGMENT);
			}
			if (prog->getShader(Kite::ShaderType::GEOMETRY) == RemovedRes) {
				prog->setShader(nullptr, Kite::ShaderType::GEOMETRY);
			}
		}
	}
};

#endif // SHPROGEDITOR_H
