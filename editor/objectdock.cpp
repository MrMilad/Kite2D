#include "objectdock.h"
#include <QtWidgets>
#include <qinputdialog.h>
#include <kmeta.khgen.h>

ObjectDock::ObjectDock(QWidget *parent):
	QDockWidget("Hierarchy", parent), currScene(nullptr)
{
	setObjectName("Hierarchy");
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setMinimumWidth(120);

	setupTree();
	setupActions();
	actionsControl(AS_ON_INITE);
	setupHTools();
}

ObjectDock::~ObjectDock() {}

void ObjectDock::setupTree() {
	objTree = new QTreeWidget(this);
	objTree->setColumnCount(1);
	objTree->setHeaderHidden(true);
	objTree->setSelectionMode(QAbstractItemView::SingleSelection);
	objTree->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(objTree, &QTreeWidget::itemSelectionChanged, this, &ObjectDock::actClicked);
	connect(objTree, &QTreeWidget::customContextMenuRequested, this, &ObjectDock::actRClicked);
	setWidget(objTree);
}

void ObjectDock::setupActions() {
	addRootObj = new QAction(QIcon(":/icons/add"), "Add New Object", this);
	addRootObj->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	addRootObj->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(addRootObj, &QAction::triggered, this, &ObjectDock::actAddRoot);
	this->addAction(addRootObj);

	addChildObj = new QAction(QIcon(":/icons/addChild"), "Add New Child", this);
	addChildObj->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_N));
	addChildObj->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(addChildObj, &QAction::triggered, this, &ObjectDock::actAddChild);
	this->addAction(addChildObj);

	renameObj = new QAction(QIcon(":/icons/edit"), "Rename Object", this);
	renameObj->setShortcut(QKeySequence(Qt::Key_F2));
	renameObj->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(renameObj, &QAction::triggered, this, &ObjectDock::actRename);
	this->addAction(renameObj);

	remObj = new QAction(QIcon(":/icons/remove"), "Remove Object", this);
	remObj->setShortcut(QKeySequence(Qt::Key_Delete));
	remObj->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(remObj, &QAction::triggered, this, &ObjectDock::actRemove);
	this->addAction(remObj);
}

void ObjectDock::setupHTools() {
	htools = new QFrame(this);
	auto vlayout = new QVBoxLayout(htools);
	vlayout->setMargin(2);
	vlayout->setSpacing(0);

	hlabel = new QLabel(htools);
	hlabel->setText("Hierarchy");
	hlabel->setStyleSheet("color: DodgerBlue;");
	vlayout->addWidget(hlabel);

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
	ledit->setPlaceholderText("Search");
	ledit->addAction(QIcon(":/icons/search"), QLineEdit::ActionPosition::TrailingPosition);
	ledit->setStyleSheet("background-color: gray;");
	connect(ledit, &QLineEdit::textChanged, this, &ObjectDock::actSearch);

	hlayout->addWidget(ledit);

	vlayout->addLayout(hlayout);

	htools->setLayout(vlayout);
	setTitleBarWidget(htools);
}

void ObjectDock::actionsControl(ActionsState State) {
	if (State == AS_ON_INITE) {
		addRootObj->setDisabled(true);
		addChildObj->setDisabled(true);
		renameObj->setDisabled(true);
		remObj->setDisabled(true);
	} else if(State == AS_ON_ITEM) {
		addRootObj->setDisabled(false);
		addChildObj->setDisabled(false);
		renameObj->setDisabled(false);
		remObj->setDisabled(false);
	} else if (State == AS_ON_LOAD) {
		addRootObj->setDisabled(false);
		addChildObj->setDisabled(true);
		renameObj->setDisabled(true);
		remObj->setDisabled(true);
	}
}

void ObjectDock::loadChilds(Kite::KEntityManager *Eman, const Kite::KHandle &Entity, QTreeWidgetItem *Parrent) {
	auto ent = Eman->getEntity(Entity);

	for (auto it = ent->beginChild(); it != ent->endChild(); ++it) {
		auto child = Eman->getEntity((*it));
		auto chnode = new QTreeWidgetItem(Parrent);
		chnode->setFlags(chnode->flags() | Qt::ItemIsUserCheckable);
		if (child->getActive()) {
			chnode->setCheckState(0, Qt::Checked);
		} else {
			chnode->setCheckState(0, Qt::Unchecked);
		}
		chnode->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
		chnode->setText(0, child->getName().c_str());

		if (child->hasChild()) {
			loadChilds(Eman, (*it), chnode);
		}
	}
}

void ObjectDock::sceneEdit(Kite::KResource *Scene) {
	if (Scene->getResourceType() != "KScene" || Scene == currScene) {
		return;
	}

	objTree->clear();

	auto scene = (Kite::KScene *)Scene;
	auto eman = scene->getEManager();
	auto root = eman->getEntity(eman->getRoot());

	// loading tree recursively
	disconnect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);

	for (auto it = root->beginChild(); it != root->endChild(); ++it) {
		auto child = eman->getEntity((*it));
		auto node = new QTreeWidgetItem(objTree);
		node->setText(0, child->getName().c_str());
		node->setFlags(node->flags() | Qt::ItemIsUserCheckable);
		if (child->getActive()) {
			node->setCheckState(0, Qt::Checked);
		} else {
			node->setCheckState(0, Qt::Unchecked);
		}
		node->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
		loadChilds(eman, (*it), node);
	}

	// set current scene
	currScene = scene;
	QString name(Scene->getResourceName().c_str());
	hlabel->setText("Hierarchy (" + name + ")");

	connect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);

	if (root->hasChild()) {
		objTree->setCurrentItem(objTree->topLevelItem(0));
	} else {
		emit(objectDelete(nullptr, nullptr));
		actionsControl(AS_ON_LOAD);
	}
}

void ObjectDock::sceneDelete(Kite::KResource *Scene){
	if (Scene == currScene) {
		objTree->clear();
		currScene = nullptr;
		actionsControl(AS_ON_INITE);
		hlabel->setText("Hierarchy");
		emit(objectDelete(nullptr, nullptr));
	}
}

void ObjectDock::entityChecked(QTreeWidgetItem *Item, int Col) {
	if (currScene != nullptr) {
		auto eman = currScene->getEManager();
		auto entity = eman->getEntityByName(Item->text(0).toStdString());
		if (Item->checkState(0) == Qt::Checked) {
			entity->setActive(true);
		} else {
			entity->setActive(false);
		}
	}
}

void ObjectDock::actClicked() {
	auto item = objTree->currentItem();
	if (item != nullptr) {
		auto eman = currScene->getEManager();
		auto entity = eman->getEntityByName(item->text(0).toStdString());
		actionsControl(AS_ON_ITEM);
		emit(objectSelected(currScene->getEManager(), entity));
	} else {
		actionsControl(AS_ON_LOAD);
	}
}

void ObjectDock::actRClicked(const QPoint & pos) {
	auto item = objTree->itemAt(pos);
	QMenu cmenu(this);

	// checking user rclicked on tree itself or entity??
	if (item == nullptr) {
		QPoint pt(pos);
		addRootObj->setText("Add New Entity");
		cmenu.addAction(addRootObj);
		cmenu.exec(mapToGlobal(pos));
	} else {
		addChildObj->setText("Add New Child");
		cmenu.addSeparator();
		cmenu.addAction(addChildObj);
		cmenu.addSeparator();
		cmenu.addAction(remObj);
		cmenu.addAction(renameObj);
		cmenu.exec(mapToGlobal(pos));
	}
}

QString ObjectDock::getAvailName() {
	auto eman = currScene->getEManager();
	bool ok = false;
	QString text;
	do {
		text = QInputDialog::getText(this, tr("New Entity"),
									 tr("Entity name:"), QLineEdit::Normal,
									 "", &ok);

		// cancel pressed
		if (!ok) {
			text.clear();
			return text;
		}

		// empty name
		if (text.isEmpty()) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("entity name is empty!");
			msg.exec();
			continue;
		}

		// available name
		auto ent = eman->getEntityByName(text.toStdString());
		if (ent != nullptr) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("this name is already exist!");
			msg.exec();
			continue;
		}

		// ok pressed
		if (ok) {
			break;
		}
	} while (true);

	return text;
}

void ObjectDock::actAddChild() {
	if (currScene == nullptr) {
		return;
	}
	auto text = getAvailName();
	if (text.isEmpty()) {
		return;
	}

	auto item = objTree->currentItem();
	if (item != nullptr) {
		auto eman = currScene->getEManager();
		auto pitem = eman->getEntityByName(item->text(0).toStdString())->getHandle();
		auto ent = eman->createEntity(text.toStdString());
		eman->getEntity(pitem)->addChild(ent->getHandle());

		disconnect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
		auto node = new QTreeWidgetItem(item);
		node->setText(0, text);
		node->setFlags(node->flags() | Qt::ItemIsUserCheckable);
		node->setCheckState(0, Qt::Checked);
		node->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
		item->setExpanded(true);
		connect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);

		emit(objectAdded(eman, ent));
		objTree->setCurrentItem(node);
	}
}

void ObjectDock::actAddRoot() {
	if (currScene == nullptr) {
		return;
	}
	auto text = getAvailName();
	if (text.isEmpty()) {
		return;
	}

	auto eman = currScene->getEManager();
	eman->createEntity(text.toStdString());

	disconnect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
	auto node = new QTreeWidgetItem(objTree);
	node->setText(0, text);
	node->setFlags(node->flags() | Qt::ItemIsUserCheckable);
	node->setCheckState(0, Qt::Checked);
	node->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
	objTree->setCurrentItem(node);
	connect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
}

void ObjectDock::actRemove(){
	if (currScene == nullptr) {
		return;
	}
	auto item = objTree->currentItem();

	if (item != nullptr) {
		auto eman = currScene->getEManager();
		auto entity = eman->getEntityByName(item->text(0).toStdString());

		emit(objectDelete(eman, entity));

		eman->removeEntity(entity->getHandle());
		eman->postWork();
		delete item;
	}
}

void ObjectDock::actRename() {
	if (currScene == nullptr) {
		return;
	}
	auto text = getAvailName();
	if (text.isEmpty()) {
		return;
	}

	auto item = objTree->currentItem();
	if (item != nullptr) {
		auto eman = currScene->getEManager();
		auto ent = eman->getEntityByName(item->text(0).toStdString());
		eman->renameEntity(ent->getHandle(), text.toStdString());

		disconnect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
		item->setText(0, text);
		connect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);

		emit(objectRenamed(eman, ent));
	}
}

void ObjectDock::actSearch(const QString &Pharase) {
	disconnect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
	auto allItems = QTreeWidgetItemIterator(objTree);
	while (*allItems) {
		if ((*allItems)->text(0).contains(Pharase) && !Pharase.isEmpty()) {
			(*allItems)->setBackgroundColor(0, QColor(Qt::gray));
		} else {
			(*allItems)->setBackgroundColor(0, QColor(34, 34, 34));
		} 
		++allItems;
	}
	connect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
}




