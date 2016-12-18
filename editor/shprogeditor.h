#ifndef SHPROGEDITOR_H
#define SHPROGEDITOR_H

#include <qstring.h>
#include "tabwidget.h"
#include <Kite/graphic/kshaderprogram.h>

class QComboBox;
class QUndoStack;

class ShProgEditor: public TabWidget{
public:
    ShProgEditor(Kite::KResource *Res, Kite::KIStream *Stream, QUndoStack *UStack, QWidget *Parent = nullptr);

	void inite() override;
	void saveChanges() override;
	void reload() override;

	static TabWidget *factory(Kite::KResource *Res, Kite::KIStream *Stream, QUndoStack *UStack, KiteInfo *KInfo, QWidget *Parent = nullptr) {
		return new ShProgEditor(Res, Stream, UStack, Parent);
	}

	void onRemoveRes(const QString &Name, Kite::RTypes Type) override;

protected:
	bool eventFilter(QObject *Obj, QEvent *Event);

private slots:
	void cmbChanged(const QString &Text);

private:
	QComboBox *cmbVert;
	QComboBox *cmbFrag;
	QComboBox *cmbGeom;
    Kite::KShaderProgram *prog;
	QUndoStack *ustack;
	QString prevVert;
	QString prevFrag;
	QString prevGeom;
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
