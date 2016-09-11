#include "outputdock.h"
#include "qboxlayout.h"
#include "qtoolbutton.h"

OutputDock::OutputDock(QWidget *parent) :
	QDockWidget("Engine Output", parent) 
{
	setObjectName("Output");
	setAllowedAreas(Qt::BottomDockWidgetArea);
	setStyleSheet("QDockWidget { border: 3px solid; }");
	setMinimumWidth(120);
	setFeatures(DockWidgetFeature::DockWidgetClosable | DockWidgetFeature::DockWidgetVerticalTitleBar
				| DockWidgetFeature::DockWidgetMovable | DockWidgetFeature::DockWidgetFloatable);

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

	auto vlayout = new QVBoxLayout(htools);
	vlayout->setMargin(0);
	vlayout->setSpacing(0);

	auto btnClaer = new QToolButton(htools);
	btnClaer->setDefaultAction(clearAll);
	btnClaer->setToolButtonStyle(Qt::ToolButtonIconOnly);
	vlayout->addWidget(btnClaer);

	auto btnToggleWrap = new QToolButton(htools);
	btnToggleWrap->setDefaultAction(wordWrap);
	btnToggleWrap->setToolButtonStyle(Qt::ToolButtonIconOnly);
	vlayout->addWidget(btnToggleWrap);

	auto sepBrush = QBrush(Qt::gray, Qt::BrushStyle::Dense6Pattern);
	QPalette sepPalette;
	sepPalette.setBrush(QPalette::Background, sepBrush);

	auto seprator = new QLabel(htools);
	seprator->setAutoFillBackground(true);
	seprator->setPalette(sepPalette);
	seprator->setMaximumWidth(10);
	seprator->setMinimumWidth(10);
	vlayout->addWidget(seprator, 1, Qt::AlignHCenter);

	htools->setLayout(vlayout);
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