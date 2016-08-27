#include "shprogeditor.h"
#include <QFormLayout>
#include <qcombobox.h>
#include <qstringlist.h>
#include <qevent.h>

ShProgEditor::ShProgEditor(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent):
    TabWidget(Res, KInfo, Parent),
    prog((Kite::KShaderProgram *)Res)
{}

void ShProgEditor::inite() {
	// request shaders
	QList<const Kite::KResource *> rlist;
	emit(TabWidget::requestResList(Kite::RTypes::Shader, rlist));

	// splite shader types
	QStringList vshader;
	QStringList fshader;
	QStringList gshader;

	vshader.push_back("");
	fshader.push_back("");
	gshader.push_back("");

	for (auto it = rlist.cbegin(); it != rlist.cend(); ++it) {
		auto shd = (const Kite::KShader *)(*it);
		if (shd->getShaderType() == Kite::ShaderType::VERTEX) {
			vshader.push_back(shd->getName().c_str());
		} else if (shd->getShaderType() == Kite::ShaderType::FRAGMENT) {
			fshader.push_back(shd->getName().c_str());
		} else if (shd->getShaderType() == Kite::ShaderType::GEOMETRY) {
			gshader.push_back(shd->getName().c_str());
		}
	}

	// main layout
	auto flayout = new QFormLayout(this);

	// vertex
	cmbVert = new QComboBox(this);
	cmbVert->setObjectName("vert");
	cmbVert->addItems(vshader);
	if (prog->getShader(Kite::ShaderType::VERTEX) != nullptr) {
		cmbVert->setCurrentText(prog->getShader(Kite::ShaderType::VERTEX)->getName().c_str());
	}
	cmbVert->installEventFilter(this);
	flayout->addRow("Vertex Shader: ", cmbVert);

	// fragment
	cmbFrag = new QComboBox(this);
	cmbFrag->setObjectName("frag");
	cmbFrag->addItems(fshader);
	if (prog->getShader(Kite::ShaderType::FRAGMENT) != nullptr) {
		cmbFrag->setCurrentText(prog->getShader(Kite::ShaderType::FRAGMENT)->getName().c_str());
	}
	cmbFrag->installEventFilter(this);
	flayout->addRow("Fragment Shader: ", cmbFrag);

	// Geomety
	cmbGeom = new QComboBox(this);
	cmbGeom->setObjectName("geom");
	cmbGeom->addItems(gshader);
	if (prog->getShader(Kite::ShaderType::GEOMETRY) != nullptr) {
		cmbGeom->setCurrentText(prog->getShader(Kite::ShaderType::GEOMETRY)->getName().c_str());
	}
	cmbGeom->installEventFilter(this);
	flayout->addRow("Geometry Shader (Optional): ", cmbGeom);
}

bool ShProgEditor::eventFilter(QObject *Obj, QEvent *Event) {
	if (Event->type() == QEvent::MouseButtonPress) {
		QComboBox *combo = qobject_cast<QComboBox *>(Obj);

		// request shaders
		QList<const Kite::KResource *> rlist;
		emit(TabWidget::requestResList(Kite::RTypes::Shader, rlist));

		QStringList slist;
		slist.push_back("");

		Kite::ShaderType stype;
		if (combo->objectName() == "vert") { stype = Kite::ShaderType::VERTEX; }
		else if (combo->objectName() == "frag") { stype = Kite::ShaderType::FRAGMENT; }
		else if (combo->objectName() == "geom") { stype = Kite::ShaderType::GEOMETRY; }

		for (auto it = rlist.cbegin(); it != rlist.cend(); ++it) {
			auto shd = (const Kite::KShader *)(*it);
			if (shd->getShaderType() == stype) {
				slist.push_back(shd->getName().c_str());
			}
		}

		auto text = combo->currentText();
		combo->clear();

		combo->addItems(slist);
		combo->setCurrentText(text);
	}
	return false;
}

bool ShProgEditor::saveChanges() {
	Kite::KResource *res = nullptr;

	//vertex
	emit(res = requestRes(cmbVert->currentText()));
	prog->setShader((Kite::KShader *)res, Kite::ShaderType::VERTEX);

	//fragment
	emit(res = requestRes(cmbFrag->currentText()));
	prog->setShader((Kite::KShader *)res, Kite::ShaderType::FRAGMENT);

	//geometry
	emit(res = requestRes(cmbGeom->currentText()));
	prog->setShader((Kite::KShader *)res, Kite::ShaderType::GEOMETRY);
	
	return true;
}