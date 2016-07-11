#include "objectdock.h"
#include <QtWidgets>
#include <qinputdialog.h>
#include "frmaddobj.h"
#include <kmeta.khgen.h>

ObjectDock::ObjectDock(QWidget *parent) :
	QDockWidget("Hierarchy", parent),
	currRes(nullptr),
	currEman(nullptr),
	preEman(nullptr),
	clipboard("clipboard")
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

Kite::RecieveTypes ObjectDock::onMessage(Kite::KMessage *Message, Kite::MessageScope Scope) {
	if (Message->getType() == "ENTITY_CREATED") {
		Kite::KHandle ehndl = *(Kite::KHandle *)Message->getData();
		emit(objectAdded(currEman, currEman->getEntity(ehndl)));
		return Kite::RecieveTypes::RECEIVED;

	} else if (Message->getType() == "ENTITY_REMOVED") {
		Kite::KHandle ehndl = *(Kite::KHandle *)Message->getData();
		emit(objectDelete(currEman, currEman->getEntity(ehndl)));
		return Kite::RecieveTypes::RECEIVED;
	}
	return Kite::RecieveTypes::IGNORED;
}

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

	prefab = new QAction(QIcon(":/icons/edit"), "Create Prefab", this);
	prefab->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
	prefab->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(prefab, &QAction::triggered, this, &ObjectDock::actPrefab);
	this->addAction(prefab);

	copy = new QAction(QIcon(":/icons/edit"), "Copy", this);
	copy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
	copy->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(copy, &QAction::triggered, this, &ObjectDock::actCopy);
	this->addAction(copy);

	cut = new QAction(QIcon(":/icons/edit"), "Cut", this);
	cut->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
	cut->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(cut, &QAction::triggered, this, &ObjectDock::actCut);
	this->addAction(cut);

	paste = new QAction(QIcon(":/icons/edit"), "Paste", this);
	paste->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
	paste->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	paste->setDisabled(true);
	connect(paste, &QAction::triggered, this, &ObjectDock::actPaste);
	this->addAction(paste);
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

	auto btnAddChild = new QToolButton(htools);
	btnAddChild->setDefaultAction(addChildObj);
	btnAddChild->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnAddChild);

	hlayout->addSpacing(10);

	auto btnPrefab = new QToolButton(htools);
	btnPrefab->setDefaultAction(prefab);
	btnPrefab->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnPrefab);

	hlayout->addSpacing(10);

	auto btnRemove = new QToolButton(htools);
	btnRemove->setDefaultAction(remObj);
	btnRemove->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnRemove);

	auto btnRename = new QToolButton(htools);
	btnRename->setDefaultAction(renameObj);
	btnRename->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnRename);

	hlayout->addStretch(1);

	vlayout->addLayout(hlayout);

	ledit = new QLineEdit(htools);
	ledit->setPlaceholderText("Search");
	ledit->addAction(QIcon(":/icons/search"), QLineEdit::ActionPosition::TrailingPosition);
	ledit->setStyleSheet("background-color: gray;");
	connect(ledit, &QLineEdit::textChanged, this, &ObjectDock::actSearch);

	vlayout->addWidget(ledit);

	htools->setLayout(vlayout);
	setTitleBarWidget(htools);
}

void ObjectDock::actionsControl(ActionsState State) {
	if (State == AS_ON_INITE) {
		addRootObj->setDisabled(true);
		addChildObj->setDisabled(true);
		renameObj->setDisabled(true);
		remObj->setDisabled(true);
		prefab->setDisabled(true);
	} else if(State == AS_ON_ITEM) {
		addRootObj->setDisabled(false);
		addChildObj->setDisabled(false);
		renameObj->setDisabled(false);
		remObj->setDisabled(false);
		prefab->setDisabled(false);
	} else if (State == AS_ON_LOAD) {
		addRootObj->setDisabled(false);
		addChildObj->setDisabled(true);
		renameObj->setDisabled(true);
		remObj->setDisabled(true);
		prefab->setDisabled(true);
	} else if (State == AS_ON_PREFAB) {
		addRootObj->setDisabled(true);
		addChildObj->setDisabled(false);
		renameObj->setDisabled(true);
		remObj->setDisabled(false);
		prefab->setDisabled(false);
	}
}

QTreeWidgetItem *ObjectDock::loadChilds(Kite::KEntityManager *Eman, const Kite::KHandle &Entity, QTreeWidgetItem *Parrent) {
	auto ent = Eman->getEntity(Entity);
	auto selfNode = new QTreeWidgetItem(Parrent);
	selfNode->setFlags(selfNode->flags() | Qt::ItemIsUserCheckable);

	// active
	if (ent->isActive()) {
		selfNode->setCheckState(0, Qt::Checked);
	} else {
		selfNode->setCheckState(0, Qt::Unchecked);
	}

	// prefab
	if (ent->isPrefab()) {
		selfNode->setTextColor(0, QColor("Orange"));
	}

	selfNode->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
	selfNode->setText(0, ent->getName().c_str());

	for (auto it = ent->childList()->cbegin(); it != ent->childList()->cend(); ++it) {
		loadChilds(Eman, (*it), selfNode);
	}

	return selfNode;
}

void ObjectDock::recursiveLoad(Kite::KEntity *Root) {
	// loading tree recursively
	// disable check
	disconnect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);

	// disable click
	disconnect(objTree, &QTreeWidget::itemSelectionChanged, this, &ObjectDock::actClicked);

	objTree->clear();

	for (auto it = Root->childList()->cbegin(); it != Root->childList()->cend(); ++it) {
		loadChilds(currEman, (*it), objTree->invisibleRootItem());
	}

	connect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
	connect(objTree, &QTreeWidget::itemSelectionChanged, this, &ObjectDock::actClicked);
}

void ObjectDock::installEntityCallback(Kite::KResource *Res) {
	if (Res->getType() == "KScene") {
		auto scene = (Kite::KScene *)Res;
		scene->getEManager()->subscribe(*this, "ENTITY_CREATED");
		scene->getEManager()->subscribe(*this, "ENTITY_REMOVED");
	}
}

void ObjectDock::resEdit(Kite::KResource *Res) {
	if (Res == currRes) {
		return;
	}

	Kite::KEntity *root = nullptr;

	// scene
	if (Res->getType() == "KScene") {
		auto scene = (Kite::KScene *)Res;
		hlabel->setText("Hierarchy (KScene: " + QString(scene->getName().c_str()) + ")");
		currEman = scene->getEManager();
		root = currEman->getEntity(currEman->getRoot());

		if (!root->hasChild()) {
			actionsControl(AS_ON_LOAD);
		}

	// prefab
	} else if (Res->getType() == "KPrefab") {
		auto pre = (Kite::KPrefab *)Res;
		hlabel->setText("Hierarchy (KPrefab: " + QString(pre->getName().c_str()) + ")");
		delete preEman;
		preEman = new Kite::KEntityManager();
		preEman->loadPrefab(pre);
		currEman = preEman;
		root = currEman->getEntity(currEman->getRoot());
		actionsControl(AS_ON_PREFAB);
	} else {
		return;
	}

	currRes = Res;
	recursiveLoad(root);

	if (root->hasChild()) {
		objTree->setCurrentItem(objTree->topLevelItem(0));
	} else {
		emit(objectDelete(nullptr, nullptr));
	}
}

void ObjectDock::resDelete(Kite::KResource *Res){
	if (Res == currRes) {
		objTree->clear();
		currRes = nullptr;
		actionsControl(AS_ON_INITE);
		hlabel->setText("Hierarchy");
		emit(objectDelete(nullptr, nullptr));
	}
}

void ObjectDock::revertPrefab(Kite::KEntity *Entity) {
	auto phndl = Entity->getParentHandle();
	std::string name = Entity->getName();
	auto res = emit(requestGetResource(Entity->getPrefab().c_str()));

	if (res == nullptr) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("there is no prefab with this name.");
		msg.exec();
		return;
	}

	if (res->getType() != "KPrefab") {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("there is no prefab with this name.");
		msg.exec();
		return;
	}

	auto prefab = (Kite::KPrefab *)res;
	currEman->removeEntity(Entity->getHandle());
	currEman->postWork();

	// create and rename prefab
	auto newhndl = currEman->loadPrefab(prefab);
	currEman->renameEntity(newhndl, name);

	// add new entity to its parent
	if (phndl != currEman->getRoot()) {
		currEman->getEntity(phndl)->addChild(newhndl);
	}

	// reload tree
	auto item = objTree->currentItem();
	auto pitem = item->parent();
	if (pitem == nullptr) {
		pitem = objTree->invisibleRootItem();
	}

	delete item;

	disconnect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
	item = loadChilds(currEman, newhndl, pitem);
	objTree->setCurrentItem(item);
	item->setExpanded(true);
	connect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
}

void ObjectDock::applyPrefab(Kite::KEntity *Entity) {
	if (currRes != nullptr) {
		if (currRes->getType() == "KPrefab") {
			auto pre = (Kite::KPrefab *)currRes;
			auto ent = currEman->getEntity(currEman->getRoot())->childList()->front();
			if (!currEman->createPrefab(ent, pre)) {
				QMessageBox msg;
				msg.setWindowTitle("Message");
				msg.setText("can't apply changes to the prefab.");
				msg.exec();
			}
		}
	}
}

void ObjectDock::entityChecked(QTreeWidgetItem *Item, int Col) {
	if (currRes != nullptr && currEman != nullptr) {
		auto entity = currEman->getEntityByName(Item->text(0).toStdString());
		bool active;

		if (Item->checkState(0) == Qt::Checked) { active = true;} else { active = false; }
		entity->setActive(active);
		for (auto counter = 0; counter < Item->childCount(); ++counter) {
			Item->child(counter)->setCheckState(0, Item->checkState(0));
		}
	}
}

void ObjectDock::actClicked() {
	auto item = objTree->currentItem();
	if (item != nullptr) {
		std::string str = item->text(0).toStdString();
		auto entity = currEman->getEntityByName(item->text(0).toStdString());
		bool isPrefab = false;
		if (currRes->getType() == "KScene") {
			actionsControl(AS_ON_ITEM);
		} else if (currRes->getType() == "KPrefab") {
			isPrefab = true;
			actionsControl(AS_ON_PREFAB);
			if (entity->getParentHandle() == currEman->getRoot()) {
				remObj->setDisabled(true); // removing root entity in prefab not allowed
			}
		}
		
		emit(objectSelected(currEman, entity, isPrefab));
	} else {
		actionsControl(AS_ON_LOAD);
	}
}

void ObjectDock::actRClicked(const QPoint & pos) {
	auto item = objTree->itemAt(pos);
	QMenu cmenu(this);
	QPoint mpos(pos);
	mpos.setY(mpos.y() + 50);

	// checking user rclicked on tree itself or entity??
	if (item == nullptr) {
		addRootObj->setText("Add New Entity");
		cmenu.addAction(addRootObj);
		cmenu.addSeparator();
		if (currRes->getType() != "KPrefab") {
			paste->setData(true);
			cmenu.addAction(paste);
		}
		cmenu.exec(mapToGlobal(mpos));
	} else {
		addChildObj->setText("Add New Child");
		cmenu.addSeparator();
		cmenu.addAction(addChildObj);
		cmenu.addSeparator();
		cmenu.addAction(prefab);
		cmenu.addSeparator();
		cmenu.addAction(cut);
		cmenu.addAction(copy);
		paste->setData(false);
		cmenu.addAction(paste);
		cmenu.addAction(remObj);
		cmenu.addAction(renameObj);
		cmenu.exec(mapToGlobal(mpos));
	}
}

Kite::KHandle ObjectDock::createObject(bool Prefab) {
	QStringList prefabList;
	emit(requestResName("KPrefab", prefabList));
	auto newObjFrm = new frmAddObj(prefabList, Prefab, this);
	do {
		newObjFrm->exec();

		// ok clicked
		if (newObjFrm->isOk()) {
			// prefab
			if (newObjFrm->isPrefab()) {
				if (newObjFrm->preName().isEmpty()) {
					return Kite::KHandle();
				}
				auto prefab = (Kite::KPrefab *) emit(requestGetResource(newObjFrm->preName()));
				return currEman->loadPrefab(prefab);

			// object
			} else {
				// empty name (auto generated name)
				if (newObjFrm->objName().isEmpty()) {
					return currEman->createEntity("")->getHandle();

				// given name
				} else {
					auto ent = currEman->getEntityByName(newObjFrm->objName().toStdString());
					// name is available
					if (ent == nullptr) {
						return currEman->createEntity(newObjFrm->objName().toStdString())->getHandle();

					// name is not available
					} else {
						QMessageBox msg;
						msg.setWindowTitle("Message");
						msg.setText("this name is already exist!");
						msg.exec();
						continue;
					}
				}

			}

		// cancel clicked
		} else {
			break;
		}
	} while (true);

	delete newObjFrm;
	return Kite::KHandle();
}

void ObjectDock::actAddChild() {
	if (currRes == nullptr) {
		return;
	}
	bool pre = true;
	if (currRes->getType() == "KPrefab") {
		pre = false;
	}
	auto handle = createObject(pre);
	if (handle == Kite::KHandle()) {
		return;
	}
	auto pitem = objTree->currentItem();
	
	if (pitem != nullptr) {
		auto phndl = currEman->getEntityByName(pitem->text(0).toStdString())->getHandle();
		currEman->getEntity(phndl)->addChild(handle);

		// update prefab
		if (currRes->getType() == "KPrefab") {
			auto ent = currEman->getEntity(currEman->getRoot())->childList()->at(0);
			currEman->createPrefab(ent, (Kite::KPrefab *)currRes);
		}

		disconnect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
		auto item = loadChilds(currEman, handle, pitem);
		objTree->setCurrentItem(item);
		connect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
	}
}

void ObjectDock::actAddRoot() {
	if (currRes == nullptr) {
		return;
	}
	auto handle = createObject(true);
	if (handle == Kite::KHandle()) {
		return;
	}

	disconnect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
	auto item = loadChilds(currEman, handle, objTree->invisibleRootItem());
	objTree->setCurrentItem(item);
	connect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
}

void ObjectDock::actRemove(){
	if (currRes == nullptr) {
		return;
	}
	auto item = objTree->currentItem();

	if (item != nullptr) {
		auto entity = currEman->getEntityByName(item->text(0).toStdString());

		currEman->removeEntity(entity->getHandle());
		currEman->postWork();
		delete item;

		// update prefab
		if (currRes->getType() == "KPrefab") {
			auto ent = currEman->getEntity(currEman->getRoot())->childList()->at(0);
			currEman->createPrefab(ent, (Kite::KPrefab *)currRes);
		}
	}
}

void ObjectDock::actRename() {
	if (currRes == nullptr) {
		return;
	}
	bool ok;
	QString text;
	while (true) {
		text = QInputDialog::getText(this, "Rename Object",
											 "New Name: ", QLineEdit::Normal, "", &ok);

		if (ok) {
			if (text.isEmpty()) {
				QMessageBox msg;
				msg.setWindowTitle("Message");
				msg.setText("empty name is not allowed!");
				msg.exec();
				continue;


			} else {
				auto ent = currEman->getEntityByName(text.toStdString());
				if (ent != nullptr) {
					QMessageBox msg;
					msg.setWindowTitle("Message");
					msg.setText("this name is already exist!");
					msg.exec();
					continue;
				} else {
					break;
				}
			}
		} else {
			return;
		}
	}

	auto item = objTree->currentItem();
	if (item != nullptr) {
		auto ent = currEman->getEntityByName(item->text(0).toStdString());
		currEman->renameEntity(ent->getHandle(), text.toStdString());

		disconnect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
		item->setText(0, text);
		connect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);

		emit(objectRenamed(currEman, ent));
	}
}

void ObjectDock::actPrefab() {
	if (currRes == nullptr) {
		return;
	}
	auto item = objTree->currentItem();

	if (item != nullptr) {
		auto entity = currEman->getEntityByName(item->text(0).toStdString());
		auto pre = (Kite::KPrefab *)emit(requestCreateResource("KPrefab"));
		if (pre != nullptr) {
			currEman->createPrefab(entity->getHandle(), pre);
		}
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

void ObjectDock::actCut() {
	auto item = objTree->currentItem();

	if (item != nullptr) {
		auto entity = currEman->getEntityByName(item->text(0).toStdString());
		currEman->createPrefab(entity->getHandle(), &clipboard, true);
		actRemove();
		paste->setDisabled(false);
	}
}

void ObjectDock::actCopy() {
	auto item = objTree->currentItem();

	if (item != nullptr) {
		auto entity = currEman->getEntityByName(item->text(0).toStdString());
		currEman->createPrefab(entity->getHandle(), &clipboard, false);
		paste->setDisabled(false);
	}
}

void ObjectDock::actPaste() {
	if (!clipboard.isEmpty()) {
		auto action = (QAction *)sender();
		auto root = action->data().toBool();
		auto entity = currEman->loadPrefab(&clipboard, true);

		// pasted to root
		if (root) {
			disconnect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
			auto item = loadChilds(currEman, entity, objTree->invisibleRootItem());
			objTree->setCurrentItem(item);
			connect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);

		// pasted to item
		} else {
			auto pitem = objTree->currentItem();
			auto parent = currEman->getEntityByName(pitem->text(0).toStdString());
			parent->addChild(entity);

			disconnect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
			auto item = loadChilds(currEman, entity, pitem);
			objTree->setCurrentItem(item);
			connect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
		}
	}
	paste->setDisabled(true);
	clipboard.clear();
}




