#include "outputdock.h"
#include "qboxlayout.h"
#include "qtoolbutton.h"

OutputDock::OutputDock(QWidget *parent) :
	QDockWidget("Engine Output", parent) 
{
	setObjectName("Output");
	setAllowedAreas(Qt::BottomDockWidgetArea);
	setMinimumWidth(120);
	setFeatures(QDockWidget::DockWidgetFeature::DockWidgetClosable);

	setupTEdit();
	setupActions();
	setupHTools();
}

OutputDock::~OutputDock() {}

void OutputDock::setupTEdit() {
	textEdit = new QPlainTextEdit(this);
	textEdit->setReadOnly(true);
	textEdit->setMaximumBlockCount(200);
	textEdit->setWordWrapMode(QTextOption::NoWrap);
	setWidget(textEdit);
}

void OutputDock::toggleWordWrap() {
	if (textEdit->wordWrapMode() == QTextOption::NoWrap) {
		textEdit->setWordWrapMode(QTextOption::WordWrap);
	} else {
		textEdit->setWordWrapMode(QTextOption::NoWrap);
	}
}

void OutputDock::setupActions() {
	clearAll = new QAction(QIcon(":/icons/clr"), "Clear All", this);
	connect(clearAll, &QAction::triggered, textEdit, &QPlainTextEdit::clear);

	wordWrap = new QAction(QIcon(":/icons/word"), "Toggle Word Wrap", this);
	connect(wordWrap, &QAction::triggered, this, &OutputDock::toggleWordWrap);
}

void OutputDock::setupHTools() {
	htools = new QFrame(this);

	auto hlayout = new QHBoxLayout(htools);
	hlayout->setMargin(0);
	hlayout->setSpacing(0);

	auto btnClaer = new QToolButton(htools);
	btnClaer->setDefaultAction(clearAll);
	btnClaer->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnClaer);

	auto btnToggleWrap = new QToolButton(htools);
	btnToggleWrap->setDefaultAction(wordWrap);
	btnToggleWrap->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnToggleWrap);

	hlayout->addStretch(1);

	htools->setLayout(hlayout);
	setTitleBarWidget(htools);
}

void OutputDock::autoShow() {
	wasHide = isHidden();
	show();
}

void OutputDock::autoHide() {
	if (wasHide) {
		hide();
	}
}