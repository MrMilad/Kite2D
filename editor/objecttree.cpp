#include "objecttree.h"
#include <QtWidgets>
#include <qinputdialog.h>
#include <kmeta.khgen.h>

ObjectTree::ObjectTree(QWidget *parent):
	QTreeWidget(parent)
{
	setColumnCount(1);
	setHeaderHidden(true);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &QTreeWidget::itemSelectionChanged, this, &ObjectTree::actClicked);
	connect(this, &QTreeWidget::customContextMenuRequested, this, &ObjectTree::actRClicked);
	connect(this, &QTreeWidget::itemChanged, this, &ObjectTree::entityChecked);

	setupActions();
	actionsControl(OA_ON_INITE);
	setupHTools();
	setupShortcuts();
}

ObjectTree::~ObjectTree() {}

void ObjectTree::focusInEvent(QFocusEvent *Event) {
	for (auto it = shortcuts.begin(); it != shortcuts.end(); ++it) {
		(*it)->setEnabled(true);
	}
}

void ObjectTree::focusOutEvent(QFocusEvent *Event) {
	for (auto it = shortcuts.begin(); it != shortcuts.end(); ++it) {
		(*it)->setEnabled(false);
	}
}

void ObjectTree::setupActions() {
	addRootObj = new QAction(QIcon(":/icons/add"), "Add New Object", this);
	connect(addRootObj, &QAction::triggered, this, &ObjectTree::actAdd);

	addChildObj = new QAction(QIcon(":/icons/addChild"), "Add New Child", this);
	connect(addChildObj, &QAction::triggered, this, &ObjectTree::actAdd);

	renameObj = new QAction(QIcon(":/icons/edit"), "Rename Object", this);
	connect(renameObj, &QAction::triggered, this, &ObjectTree::actRename);

	remObj = new QAction(QIcon(":/icons/remove"), "Remove Object", this);
	connect(remObj, &QAction::triggered, this, &ObjectTree::actRemove);
}

void ObjectTree::setupShortcuts() {
	shortcuts.push_back(new QShortcut(QKeySequence(Qt::Key_Delete), this));
	connect(shortcuts.back(), &QShortcut::activated, remObj, &QAction::trigger);

	shortcuts.push_back(new QShortcut(QKeySequence(Qt::Key_Control + Qt::Key_N), this));
	connect(shortcuts.back(), &QShortcut::activated, addRootObj, &QAction::trigger);

	shortcuts.push_back(new QShortcut(QKeySequence(Qt::Key_Control + Qt::Key_Alt + Qt::Key_N), this));
	connect(shortcuts.back(), &QShortcut::activated, addChildObj, &QAction::trigger);

	shortcuts.push_back(new QShortcut(QKeySequence(Qt::Key_F2), this));
	connect(shortcuts.back(), &QShortcut::activated, renameObj, &QAction::trigger);
}

void ObjectTree::setupHTools() {
	htools = new QFrame(this);
	auto vlayout = new QVBoxLayout(htools);
	vlayout->setMargin(2);
	vlayout->setSpacing(0);

	auto name = new QLabel(htools);
	name->setText("Hierarchy");
	name->setStyleSheet("color: DodgerBlue;");
	vlayout->addWidget(name);

	auto hlayout = new QHBoxLayout(htools);
	hlayout->setMargin(3);

	auto btnAddRoot = new QToolButton(htools);
	btnAddRoot->setDefaultAction(addRootObj);
	btnAddRoot->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnAddRoot);

	hlayout->addSpacing(10);

	auto btnAddChild = new QToolButton(htools);
	btnAddChild->setDefaultAction(addChildObj);
	btnAddChild->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnAddChild);

	auto btnRemove = new QToolButton(htools);
	btnRemove->setDefaultAction(remObj);
	btnRemove->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnRemove);

	auto btnRename = new QToolButton(htools);
	btnRename->setDefaultAction(renameObj);
	btnRename->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnRename);

	hlayout->addSpacing(10);

	ledit = new QLineEdit(htools);
	ledit->addAction(QIcon(":/icons/search"), QLineEdit::ActionPosition::TrailingPosition);
	ledit->setStyleSheet("background-color: gray;");
	connect(ledit, &QLineEdit::textChanged, this, &ObjectTree::actSearch);

	hlayout->addWidget(ledit);

	vlayout->addLayout(hlayout);

	htools->setLayout(vlayout);
}

void ObjectTree::actionsControl(ObjectActState State) {

}

void ObjectTree::loadChilds(Kite::KEntityManager *Eman, const Kite::KHandle &Entity, QTreeWidgetItem *Parrent) {

}

void ObjectTree::sceneEdit(Kite::KScene *Scene) {

}

void ObjectTree::sceneDelete(Kite::KScene *Scene){

}

void ObjectTree::entityChecked(QTreeWidgetItem *Item, int Col) {

}

void ObjectTree::actClicked() {

}

void ObjectTree::actRClicked(const QPoint & pos) {

}

void ObjectTree::actAdd() {

}

void ObjectTree::actRemove(){
}

void ObjectTree::actRename() {

}

void ObjectTree::actSearch(const QString &Pharase) {

}




