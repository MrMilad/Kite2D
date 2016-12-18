#include "shprogeditor.h"
#include <QFormLayout>
#include <qcombobox.h>
#include <qstringlist.h>
#include <qevent.h>
#include <qundostack.h>

class ShaderChangeCMD : public QUndoCommand {
public:
	ShaderChangeCMD(QComboBox *Combo, const QString &NewShader, const QString &PrevShader) :
		cmb(Combo),
		shader(NewShader),
		prevShader(PrevShader)
	{
		setText("Shader Changed");
	}

	virtual void undo() override {
		cmb->blockSignals(true);
		cmb->setCurrentText(prevShader);
		cmb->blockSignals(false);
	}

	virtual void redo() override {
		cmb->blockSignals(true);
		cmb->setCurrentText(shader);
		cmb->blockSignals(false);
	}

private:
	QComboBox *cmb;
	QString shader;
	QString prevShader;
};

ShProgEditor::ShProgEditor(Kite::KResource *Res, Kite::KIStream *stream, QUndoStack *UStack, QWidget *Parent):
    TabWidget(Res, stream, Parent),
	ustack(UStack),
    prog((Kite::KShaderProgram *)Res)
{}

void ShProgEditor::inite() {
	// main layout
	auto flayout = new QFormLayout(this);

	// vertex
	cmbVert = new QComboBox(this);
	cmbVert->setObjectName("vert");
	cmbVert->installEventFilter(this);
	connect(cmbVert, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),this, &ShProgEditor::cmbChanged);
	flayout->addRow("Vertex Shader: ", cmbVert);

	// fragment
	cmbFrag = new QComboBox(this);
	cmbFrag->setObjectName("frag");
	cmbFrag->installEventFilter(this);
	connect(cmbFrag, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &ShProgEditor::cmbChanged);
	flayout->addRow("Fragment Shader: ", cmbFrag);

	// Geomety
	cmbGeom = new QComboBox(this);
	cmbGeom->setObjectName("geom");
	cmbGeom->installEventFilter(this);
	connect(cmbGeom, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &ShProgEditor::cmbChanged);
	flayout->addRow("Geometry Shader (Optional): ", cmbGeom);

	reload();
}

bool ShProgEditor::eventFilter(QObject *Obj, QEvent *Event) {
	if (Event->type() == QEvent::MouseButtonPress) {
		QComboBox *combo = qobject_cast<QComboBox *>(Obj);
		combo->blockSignals(true);

		// request shaders
		QStringList rlist;
		emit(TabWidget::reqResList(Kite::RTypes::Shader, rlist));

		QStringList slist;
		slist.push_back("");

		Kite::ShaderType stype;
		if (combo->objectName() == "vert") { stype = Kite::ShaderType::VERTEX; }
		else if (combo->objectName() == "frag") { stype = Kite::ShaderType::FRAGMENT; }
		else if (combo->objectName() == "geom") { stype = Kite::ShaderType::GEOMETRY; }

		for (auto it = rlist.cbegin(); it != rlist.cend(); ++it) {
			auto shd = (const Kite::KShader *)emit(reqResLoad(stream, (*it)));
			if (shd->getShaderType() == stype) {
				slist.push_back(shd->getName().c_str());
			}
		}

		auto text = combo->currentText();
		combo->clear();

		combo->addItems(slist);
		combo->setCurrentText(text);
		combo->blockSignals(false);
	}
	return false;
}

void ShProgEditor::saveChanges() {
	Kite::KResource *res = nullptr;

	//vertex
	emit(res = reqResLoad(stream, cmbVert->currentText()));
	prog->setShader((Kite::KShader *)res, Kite::ShaderType::VERTEX);

	//fragment
	emit(res = reqResLoad(stream, cmbFrag->currentText()));
	prog->setShader((Kite::KShader *)res, Kite::ShaderType::FRAGMENT);

	//geometry
	emit(res = reqResLoad(stream, cmbGeom->currentText()));
	prog->setShader((Kite::KShader *)res, Kite::ShaderType::GEOMETRY);
}

void ShProgEditor::reload() {
	cmbVert->blockSignals(true);
	cmbFrag->blockSignals(true);
	cmbGeom->blockSignals(true);

	// request shaders
	QStringList rlist;
	emit(TabWidget::reqResList(Kite::RTypes::Shader, rlist));

	// splite shader types
	QStringList vshader;
	QStringList fshader;
	QStringList gshader;

	vshader.push_back("");
	fshader.push_back("");
	gshader.push_back("");

	for (auto it = rlist.cbegin(); it != rlist.cend(); ++it) {
		auto shd = (const Kite::KShader *)emit(reqResLoad(stream, (*it)));
		if (shd->getShaderType() == Kite::ShaderType::VERTEX) {
			vshader.push_back(shd->getName().c_str());
		} else if (shd->getShaderType() == Kite::ShaderType::FRAGMENT) {
			fshader.push_back(shd->getName().c_str());
		} else if (shd->getShaderType() == Kite::ShaderType::GEOMETRY) {
			gshader.push_back(shd->getName().c_str());
		}
	}

	prevVert.clear();
	cmbVert->clear();
	cmbVert->addItems(vshader);
	if ((Kite::KResource *)prog->getShader(Kite::ShaderType::VERTEX)) {
		prevVert = prog->getShader(Kite::ShaderType::VERTEX)->getName().c_str();
		cmbVert->setCurrentText(prog->getShader(Kite::ShaderType::VERTEX)->getName().c_str());
	}

	prevFrag.clear();
	cmbFrag->clear();
	cmbFrag->addItems(fshader);
	if ((Kite::KResource *)prog->getShader(Kite::ShaderType::FRAGMENT)) {
		prevFrag = prog->getShader(Kite::ShaderType::FRAGMENT)->getName().c_str();
		cmbFrag->setCurrentText(prog->getShader(Kite::ShaderType::FRAGMENT)->getName().c_str());
	}

	prevGeom.clear();
	cmbGeom->clear();
	cmbGeom->addItems(gshader);
	if ((Kite::KResource *)prog->getShader(Kite::ShaderType::GEOMETRY)) {
		prevGeom = prog->getShader(Kite::ShaderType::GEOMETRY)->getName().c_str();
		cmbGeom->setCurrentText(prog->getShader(Kite::ShaderType::GEOMETRY)->getName().c_str());
	}

	cmbVert->blockSignals(false);
	cmbFrag->blockSignals(false);
	cmbGeom->blockSignals(false);
}

void ShProgEditor::onRemoveRes(const QString &Name, Kite::RTypes Type) {
	if (Type == Kite::RTypes::Shader) {
		ustack->clear();
		reload();
	}
}

void ShProgEditor::cmbChanged(const QString &Text) {
	auto cmb = (QComboBox *)sender();
	cmb->blockSignals(true);

	if (cmb->objectName() == "vert") {
		ustack->push(new ShaderChangeCMD(cmb, Text, prevVert));
		prevVert = cmbVert->currentText();
	} else if (cmb->objectName() == "frag") {
		ustack->push(new ShaderChangeCMD(cmb, Text, prevFrag));
		prevFrag = cmbFrag->currentText();
	} else if (cmb->objectName() == "geom") {
		ustack->push(new ShaderChangeCMD(cmb, Text, prevGeom));
		prevGeom = cmbGeom->currentText();
	}

	cmb->blockSignals(false);
}