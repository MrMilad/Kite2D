#include "objecttree.h"
#include <QtWidgets>
#include <qinputdialog.h>
#include <kmeta.khgen.h>

ObjectTree::ObjectTree(QWidget *parent):
	QTreeWidget(parent), currScene(nullptr)
{
	setColumnCount(1);
	setHeaderHidden(true);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &QTreeWidget::itemSelectionChanged, this, &ObjectTree::actClicked);
	connect(this, &QTreeWidget::customContextMenuRequested, this, &ObjectTree::actRClicked);

	setupActions();
	actionsControl(AS_ON_INITE);
	setupHTools();
}

ObjectTree::~ObjectTree() {}

void ObjectTree::setupActions() {
	addRootObj = new QAction(QIcon(":/icons/add"), "Add New Object", this);
	addRootObj->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	addRootObj->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(addRootObj, &QAction::triggered, this, &ObjectTree::actAddRoot);
	this->addAction(addRootObj);

	addChildObj = new QAction(QIcon(":/icons/addChild"), "Add New Child", this);
	addChildObj->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_N));
	addChildObj->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(addChildObj, &QAction::triggered, this, &ObjectTree::actAddChild);
	this->addAction(addChildObj);

	renameObj = new QAction(QIcon(":/icons/edit"), "Rename Object", this);
	renameObj->setShortcut(QKeySequence(Qt::Key_F2));
	renameObj->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(renameObj, &QAction::triggered, this, &ObjectTree::actRename);
	this->addAction(renameObj);

	remObj = new QAction(QIcon(":/icons/remove"), "Remove Object", this);
	remObj->setShortcut(QKeySequence(Qt::Key_Delete));
	remObj->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(remObj, &QAction::triggered, this, &ObjectTree::actRemove);
	this->addAction(remObj);
}

void ObjectTree::setupHTools() {
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
	ledit->addAction(QIcon(":/icons/search"), QLineEdit::ActionPosition::TrailingPosition);
	ledit->setStyleSheet("background-color: gray;");
	connect(ledit, &QLineEdit::textChanged, this, &ObjectTree::actSearch);

	hlayout->addWidget(ledit);

	vlayout->addLayout(hlayout);

	htools->setLayout(vlayout);
}

void ObjectTree::actionsControl(ActionsState State) {
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

void ObjectTree::loadChilds(Kite::KEntityManager *Eman, const Kite::KHandle &Entity, QTreeWidgetItem *Parrent) {
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

void ObjectTree::sceneEdit(Kite::KResource *Scene) {
	if (Scene->getResourceType() != "KScene" || Scene == currScene) {
		return;
	}

	clear();

	auto scene = (Kite::KScene *)Scene;
	auto eman = scene->getEManager();
	auto root = eman->getEntity(eman->getRoot());

	// loading tree recursively
	disconnect(this, &QTreeWidget::itemChanged, this, &ObjectTree::entityChecked);

	for (auto it = root->beginChild(); it != root->endChild(); ++it) {
		auto child = eman->getEntity((*it));
		auto node = new QTreeWidgetItem(this);
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

	connect(this, &QTreeWidget::itemChanged, this, &ObjectTree::entityChecked);

	actionsControl(AS_ON_LOAD);
}

void ObjectTree::sceneDelete(Kite::KResource *Scene){
	if (Scene == currScene) {
		clear();
		currScene = nullptr;
		actionsControl(AS_ON_INITE);
		hlabel->setText("Hierarchy");
		emit(objectDelete(nullptr));
	}
}

void ObjectTree::entityChecked(QTreeWidgetItem *Item, int Col) {
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

void ObjectTree::actClicked() {
	auto item = currentItem();
	if (item != nullptr) {
		auto eman = currScene->getEManager();
		auto entity = eman->getEntityByName(item->text(0).toStdString());
		actionsControl(AS_ON_ITEM);
		emit(objectSelected(entity));
	} else {
		actionsControl(AS_ON_LOAD);
	}
}

void ObjectTree::actRClicked(const QPoint & pos) {
	auto item = itemAt(pos);
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

QString ObjectTree::getAvailName() {
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

void ObjectTree::actAddChild() {
	if (currScene == nullptr) {
		return;
	}
	auto text = getAvailName();
	if (text.isEmpty()) {
		return;
	}

	auto item = currentItem();
	if (item != nullptr) {
		auto eman = currScene->getEManager();
		auto pitem = eman->getEntityByName(item->text(0).toStdString())->getHandle();
		auto ent = eman->createEntity(text.toStdString());
		eman->getEntity(pitem)->addChild(ent);

		disconnect(this, &QTreeWidget::itemChanged, this, &ObjectTree::entityChecked);
		auto node = new QTreeWidgetItem(item);
		node->setText(0, text);
		node->setFlags(node->flags() | Qt::ItemIsUserCheckable);
		node->setCheckState(0, Qt::Checked);
		node->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
		item->setExpanded(true);
		connect(this, &QTreeWidget::itemChanged, this, &ObjectTree::entityChecked);

		emit(objectAdded(eman->getEntity(ent)));
		setCurrentItem(node);
	}
}

void ObjectTree::actAddRoot() {
	if (currScene == nullptr) {
		return;
	}
	auto text = getAvailName();
	if (text.isEmpty()) {
		return;
	}

	auto eman = currScene->getEManager();
	eman->createEntity(text.toStdString());

	disconnect(this, &QTreeWidget::itemChanged, this, &ObjectTree::entityChecked);
	auto node = new QTreeWidgetItem(this);
	node->setText(0, text);
	node->setFlags(node->flags() | Qt::ItemIsUserCheckable);
	node->setCheckState(0, Qt::Checked);
	node->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
	setCurrentItem(node);
	connect(this, &QTreeWidget::itemChanged, this, &ObjectTree::entityChecked);
}

void ObjectTree::actRemove(){
	if (currScene == nullptr) {
		return;
	}
	auto item = currentItem();

	if (item != nullptr) {
		auto eman = currScene->getEManager();
		auto entity = eman->getEntityByName(item->text(0).toStdString());

		emit(objectDelete(entity));

		eman->removeEntity(entity->getHandle());
		eman->postWork();
		delete item;
	}
}

void ObjectTree::actRename() {
	if (currScene == nullptr) {
		return;
	}
	auto text = getAvailName();
	if (text.isEmpty()) {
		return;
	}

	auto item = currentItem();
	if (item != nullptr) {
		auto eman = currScene->getEManager();
		auto ent = eman->getEntityByName(item->text(0).toStdString());
		eman->renameEntity(ent->getHandle(), text.toStdString());

		disconnect(this, &QTreeWidget::itemChanged, this, &ObjectTree::entityChecked);
		item->setText(0, text);
		connect(this, &QTreeWidget::itemChanged, this, &ObjectTree::entityChecked);

		emit(objectRenamed(ent));
	}
}

void ObjectTree::actSearch(const QString &Pharase) {
	disconnect(this, &QTreeWidget::itemChanged, this, &ObjectTree::entityChecked);
	auto allItems = QTreeWidgetItemIterator(this);
	while (*allItems) {
		if ((*allItems)->text(0).contains(Pharase) && !Pharase.isEmpty()) {
			(*allItems)->setBackgroundColor(0, QColor(Qt::gray));
		} else {
			(*allItems)->setBackgroundColor(0, QColor(34, 34, 34));
		} 
		++allItems;
	}
	connect(this, &QTreeWidget::itemChanged, this, &ObjectTree::entityChecked);
}




