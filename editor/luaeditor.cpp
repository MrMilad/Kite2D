#include "luaeditor.h"
#include "luahlight.h"
#include "kiteinfo.h"
#include <qabstractitemview.h>
#include <qboxlayout.h>
#include <qlabel.h>

Completer *LuaEditor::completer = new Completer();

LuaEditor::LuaEditor(Kite::KResource *Res, KiteInfo * KInfo, QWidget *Parent):
	TabWidget(Res, KInfo, Parent),
	editor(nullptr),
	script((Kite::KScript *)Res),
	kinfo(KInfo)
{}

LuaEditor::~LuaEditor() {
	delete hlight;
	delete editor;
}

void LuaEditor::inite() {
	// inite kite2D model
	initeModel();

	// create editor
	editor = new CodeEditor(completer, this);

	// create highlighter 
	hlight = new LuaHLight(editor->document());

	//main layout
	auto vlayout = new QVBoxLayout(this);
	vlayout->setMargin(2);

	vlayout->addWidget(editor);

	// set editor text
	editor->clear();
	if (script->getCode().empty()) {
		editor->setPlainText("function inite(self)\n"
							 "\t\nend\n"
							 "\nfunction start(self)\n"
							 "\t\nend\n"
							 "\nfunction update(self, delta)\n"
							 "\t\nend\n"
							 "\nfunction onMessage(self, msg)\n"
							 "\t\nend\n");
	} else {
		editor->appendPlainText(script->getCode().c_str());
	}
	editor->show();

	// connect completer 
	connect(completer, SIGNAL(activated(QString)),
			editor, SLOT(insertCompletion(QString)));
}

bool LuaEditor::saveChanges() {
	script->setCode(editor->document()->toPlainText().toStdString());
	script->setModified(true);
	return true;
}

void LuaEditor::reload() {
	if (script->getCode().empty()) {
		editor->setPlainText("function inite(self)\n"
							 "\t\nend\n"
							 "\nfunction start(self)\n"
							 "\t\nend\n"
							 "\nfunction update(self, delta)\n"
							 "\t\nend\n"
							 "\nfunction onMessage(self, msg)\n"
							 "\t\nend\n");
	} else {
		editor->appendPlainText(script->getCode().c_str());
	}
}

void LuaEditor::initeModel() {
	static bool inite = false;
	QStandardItemModel *model = kinfo->getModel();
	if (!inite) {
		completer->setModel(model);
		completer->popup()->setStyleSheet("QToolTip { border: 1px solid #2c2c2c; background-color: #242424; color: white;}");
		completer->setMaxVisibleItems(9);
		completer->setCaseSensitivity(Qt::CaseInsensitive);
		completer->setWrapAround(false);

		// add lua keywords into the model
		QStringList luaKeys;
		luaKeys << "and" << "break" << "do"
			<< "false" << "for" << "function"
			<< "local" << "nil" << "not"
			<< "then" << "true" << "until"
			<< "else" << "elseif" << "in"
			<< "goto" << "if" << "return"
			<< "or" << "repeat" << "while"
			<< "end";

		for (auto it = luaKeys.begin(); it != luaKeys.end(); ++it) {
			model->appendRow(new QStandardItem(QIcon(":/icons/key16"), (*it)));
		}
		inite = true;
	}
}
