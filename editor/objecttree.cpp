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
	connect(addRootObj, &QAction::triggered, this, &ObjectTree::actAddRoot);

	addChildObj = new QAction(QIcon(":/icons/addChild"), "Add New Child", this);
	connect(addChildObj, &QAction::triggered, this, &ObjectTree::actAddChild);

	renameObj = new QAction(QIcon(":/icons/edit"), "Rename Object", this);
	connect(renameObj, &QAction::triggered, this, &ObjectTree::actRename);

	remObj = new QAction(QIcon(":/icons/remove"), "Remove Object", this);
	connect(remObj, &QAction::triggered, this, &ObjectTree::actRemove);
}

void ObjectTree::setupShortcuts() {
	shortcuts.push_back(new QShortcut(QKeySequence(Qt::Key_Delete), this));
	connect(shortcuts.back(), &QShortcut::activated, remObj, &QAction::trigger);

	shortcuts.push_back(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N), this));
	connect(shortcuts.back(), &QShortcut::activated, addRootObj, &QAction::trigger);

	shortcuts.push_back(new QShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_N), this));
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
	if (State == OA_ON_INITE) {
		addRootObj->setDisabled(true);
		addChildObj->setDisabled(true);
		renameObj->setDisabled(true);
		remObj->setDisabled(true);
	} else if(State == OA_ON_ITEM) {
		addRootObj->setDisabled(false);
		addChildObj->setDisabled(false);
		renameObj->setDisabled(false);
		remObj->setDisabled(false);
	} else if (State == OA_ON_LOAD) {
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

	connect(this, &QTreeWidget::itemChanged, this, &ObjectTree::entityChecked);

	actionsControl(OA_ON_LOAD);
}

void ObjectTree::sceneDelete(Kite::KResource *Scene){
	if (Scene == currScene) {
		clear();
		currScene = nullptr;
		actionsControl(OA_ON_INITE);
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
		actionsControl(OA_ON_ITEM);
		emit(objectSelected(entity));
	} else {
		actionsControl(OA_ON_LOAD);
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
	if (!addChildObj->isEnabled() || currScene == nullptr) {
		return;
	}
	auto text = getAvailName();
	if (text.isEmpty()) {
		return;
	}

	auto item = currentItem();
	if (item != nullptr) {
		auto eman = currScene->getEManager();
		auto parrent = eman->getEntityByName(item->text(0).toStdString())->getHandle();
		auto ent = eman->createEntity(text.toStdString());
		eman->getEntity(parrent)->addChild(ent);

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
	if (!addRootObj->isEnabled() || currScene == nullptr) {
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
	if (!remObj->isEnabled() || currScene == nullptr) {
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
	if (!renameObj->isEnabled() || currScene == nullptr) {
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




