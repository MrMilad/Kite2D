#include "objectdock.h"
#include <QtWidgets>
#include <qinputdialog.h>
#include "frmaddobj.h"
#include <kmeta.khgen.h>
#include <qundostack.h>
#include <qaction.h>
#include <Kite/core/kscene.h>
#include <Kite/core/kentity.h>
#include <Kite/core/klistener.h>
#include <Kite/core/kentitymanager.h>

ObjectDock::ObjectDock(QUndoGroup *UndoGroup, QWidget *parent) :
	QDockWidget("Hierarchy", parent),
	undoStack(new QUndoStack(UndoGroup)),
	currRes(nullptr),
	currEman(nullptr),
	preEman(nullptr)
{
	setObjectName("Hierarchy");
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setStyleSheet("QDockWidget { border: 3px solid; }");
	setMinimumWidth(120);

	setupTree();
	setupActions();
	setupHTools();

	this->setDisabled(true);
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

void ObjectDock::initePanel() {
	connect(objTree, &QTreeWidget::itemSelectionChanged, this, &ObjectDock::clicked);
	connect(objTree, &QTreeWidget::customContextMenuRequested, this, &ObjectDock::rightClicked);

	this->setDisabled(false);
}

void ObjectDock::clearPanel() {
	// disconnect all connections
	disconnect(objTree, &QTreeWidget::itemSelectionChanged, this, &ObjectDock::clicked);
	disconnect(objTree, &QTreeWidget::customContextMenuRequested, this, &ObjectDock::rightClicked);

	currRes = nullptr;
	currEman = nullptr;
	preEman = nullptr;

	objTree->clear();
	this->setDisabled(true);
}

void ObjectDock::setupTree() {
	objTree = new QTreeWidget(this);
	objTree->setColumnCount(1);
	objTree->setHeaderHidden(true);
	objTree->setSelectionMode(QAbstractItemView::SingleSelection);
	objTree->setContextMenuPolicy(Qt::CustomContextMenu);
	setWidget(objTree);
}

void ObjectDock::setupActions() {
	actAddRoot = new QAction(QIcon(":/icons/add"), "Add New Object", this);
	actAddRoot->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	actAddRoot->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(actAddRoot, &QAction::triggered, this, &ObjectDock::addRoot);
	this->addAction(actAddRoot);

	actAddChild = new QAction(QIcon(":/icons/addChild"), "Add New Child Object", this);
	actAddChild->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_N));
	actAddChild->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(actAddChild, &QAction::triggered, this, &ObjectDock::addChild);
	this->addAction(actAddChild);

	actRename = new QAction(QIcon(":/icons/edit"), "Rename Object", this);
	actRename->setShortcut(QKeySequence(Qt::Key_F2));
	actRename->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(actRename, &QAction::triggered, this, &ObjectDock::rename);
	this->addAction(actRename);

	actRemove = new QAction(QIcon(":/icons/close"), "Remove Object", this);
	actRemove->setShortcut(QKeySequence(Qt::Key_Delete));
	actRemove->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(actRemove, &QAction::triggered, this, &ObjectDock::remove);
	this->addAction(actRemove);

	actPrefab = new QAction(QIcon(":/icons/addpre"), "Create Prefab", this);
	actPrefab->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
	actPrefab->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(actPrefab, &QAction::triggered, this, &ObjectDock::prefab);
	this->addAction(actPrefab);

	actCopy = new QAction(QIcon(":/icons/copy"), "Copy", this);
	actCopy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
	actCopy->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(actCopy, &QAction::triggered, this, &ObjectDock::copy);
	this->addAction(actCopy);

	actCut = new QAction(QIcon(":/icons/cut"), "Cut", this);
	actCut->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
	actCut->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(actCut, &QAction::triggered, this, &ObjectDock::cut);
	this->addAction(actCut);

	actPaste = new QAction(QIcon(":/icons/paste"), "Paste", this);
	actPaste->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
	actPaste->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	actPaste->setDisabled(true);
	connect(actPaste, &QAction::triggered, this, &ObjectDock::paste);
	this->addAction(actPaste);
}

void ObjectDock::setupHTools() {
	auto htools = new QFrame(this);
	auto vlayout = new QVBoxLayout(htools);
	vlayout->setMargin(2);
	vlayout->setSpacing(0);

	auto hlayoutTitle = new QHBoxLayout();

	auto hlabel = new QLabel(htools);
	hlabel->setText("Hierarchy ");
	hlabel->setStyleSheet("color: lightGray;");
	hlayoutTitle->addWidget(hlabel);

	auto sepBrush = QBrush(Qt::gray, Qt::BrushStyle::Dense6Pattern);
	QPalette sepPalette;
	sepPalette.setBrush(QPalette::Background, sepBrush);

	auto seprator = new QLabel(htools);
	seprator->setAutoFillBackground(true);
	seprator->setPalette(sepPalette);
	seprator->setMaximumHeight(10);
	hlayoutTitle->addWidget(seprator, 1, Qt::AlignBottom);

	auto btnClose = new QToolButton(htools);
	btnClose->setText("X");
	btnClose->setStyleSheet("color: lightGray\n");
	btnClose->setAutoRaise(true);
	btnClose->setMaximumWidth(16);
	btnClose->setMaximumHeight(16);
	hlayoutTitle->addWidget(btnClose);
	connect(btnClose, &QToolButton::clicked, this, &QDockWidget::hide);

	vlayout->addLayout(hlayoutTitle);
	vlayout->addSpacing(2);

	auto hlayoutTools = new QHBoxLayout();
	hlayoutTools->setMargin(0);
	hlayoutTools->setSpacing(0);

	auto btnCollAll = new QToolButton(htools);
	btnCollAll->setIcon(QIcon(":/icons/col"));
	btnCollAll->setToolButtonStyle(Qt::ToolButtonIconOnly);
	btnCollAll->setToolTip("Collapse All");
	connect(btnCollAll, &QToolButton::clicked, objTree, &QTreeWidget::collapseAll);
	hlayoutTools->addWidget(btnCollAll);
	hlayoutTools->addSpacing(5);

	auto ledit = new QLineEdit(htools);
	ledit->setPlaceholderText("Search");
	ledit->addAction(QIcon(":/icons/search"), QLineEdit::ActionPosition::TrailingPosition);
	ledit->setStyleSheet("background-color: gray;");
	hlayoutTools->addWidget(ledit, 1);
	connect(ledit, &QLineEdit::textChanged, this, &ObjectDock::search);

	vlayout->addLayout(hlayoutTools);

	htools->setLayout(vlayout);
	setTitleBarWidget(htools);
}

/*void ObjectDock::actionsControl(ActionsState State) {
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
}*/

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
	disconnect(objTree, &QTreeWidget::itemSelectionChanged, this, &ObjectDock::clicked);

	objTree->clear();

	for (auto it = Root->childList()->cbegin(); it != Root->childList()->cend(); ++it) {
		loadChilds(currEman, (*it), objTree->invisibleRootItem());
	}

	connect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
	connect(objTree, &QTreeWidget::itemSelectionChanged, this, &ObjectDock::clicked);
}

void ObjectDock::installEntityCallback(Kite::KResource *Res) {
	if (Res->getType() == Kite::RTypes::Scene) {
		auto scene = (Kite::KScene *)Res;
		scene->getEManager()->invoke(this, "ENTITY_CREATED");
		scene->getEManager()->invoke(this, "ENTITY_REMOVED");
	}
}

void ObjectDock::resEdit(Kite::KResource *Res) {
	if (Res == currRes) {
		return;
	}

	Kite::KEntity *root = nullptr;

	// scene
	if (Res->getType() == Kite::RTypes::Scene) {
		auto scene = (Kite::KScene *)Res;
		//hlabel->setText("Hierarchy (Scene: " + QString(scene->getName().c_str()) + ")");
		currEman = scene->getEManager();
		root = currEman->getEntity(currEman->getRoot());

		if (!root->hasChild()) {
			// actionsControl(AS_ON_LOAD);
		}

	// prefab
	} else if (Res->getType() == Kite::RTypes::Prefab) {
		auto pre = (Kite::KPrefab *)Res;
		//hlabel->setText("Hierarchy (Prefab: " + QString(pre->getName().c_str()) + ")");
		delete preEman;
		preEman = new Kite::KEntityManager();
		preEman->loadPrefab(pre);
		currEman = preEman;
		root = currEman->getEntity(currEman->getRoot());
		// actionsControl(AS_ON_PREFAB);
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
		clearPanel();
		initePanel();

		// actionsControl(AS_ON_INITE);
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

	if (res->getType() != Kite::RTypes::Prefab) {
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
		if (currRes->getType() == Kite::RTypes::Prefab) {
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

void ObjectDock::clicked() {
	auto item = objTree->currentItem();
	if (item != nullptr) {
		std::string str = item->text(0).toStdString();
		auto entity = currEman->getEntityByName(item->text(0).toStdString());
		bool isPrefab = false;
		if (currRes->getType() == Kite::RTypes::Prefab) {
			isPrefab = true;
			if (entity->getParentHandle() == currEman->getRoot()) {
				actRemove->setDisabled(true); // removing root entity in prefab not allowed
			}
		}
		
		emit(objectSelected(currEman, entity, isPrefab));
	}
}

void ObjectDock::rightClicked(const QPoint & pos) {
	if (currRes == nullptr) {
		return;
	}
	auto item = objTree->itemAt(pos);
	QMenu cmenu(this);
	QPoint mpos(pos);
	mpos.setY(mpos.y() + 50);

	// checking user rclicked on tree itself or entity??
	if (item == nullptr) {
		if (currRes->getType() != Kite::RTypes::Prefab) {
			cmenu.addAction(actAddRoot);
			cmenu.addSeparator();
			actPaste->setData(true);
			cmenu.addAction(actPaste);
			cmenu.exec(mapToGlobal(mpos));
		}
	} else {
		if (currRes->getType() != Kite::RTypes::Prefab) {
			cmenu.addAction(actAddRoot);
			cmenu.addSeparator();
			actRemove->setDisabled(false);
		}
		cmenu.addAction(actAddChild);
		cmenu.addSeparator();
		cmenu.addAction(actPrefab);
		cmenu.addSeparator();
		cmenu.addAction(actCut);
		cmenu.addAction(actCopy);
		actPaste->setData(false);
		cmenu.addAction(actPaste);
		cmenu.addAction(actRemove);
		cmenu.addAction(actRename);
		cmenu.exec(mapToGlobal(mpos));
	}
}

Kite::KHandle ObjectDock::createObject(bool Prefab) {
	QStringList prefabList;
	emit(requestResName(Kite::RTypes::Prefab, prefabList));
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

void ObjectDock::addChild() {
	if (currRes == nullptr) {
		return;
	}
	bool pre = true;
	if (currRes->getType() == Kite::RTypes::Prefab) {
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
		if (currRes->getType() == Kite::RTypes::Prefab) {
			auto ent = currEman->getEntity(currEman->getRoot())->childList()->at(0);
			currEman->createPrefab(ent, (Kite::KPrefab *)currRes);
		}

		disconnect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
		auto item = loadChilds(currEman, handle, pitem);
		objTree->setCurrentItem(item);
		connect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
	}
}

void ObjectDock::addRoot() {
	if (currRes == nullptr) {
		return;
	} else if (currRes->getType() == Kite::RTypes::Prefab) {
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

void ObjectDock::remove(){
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
		if (currRes->getType() == Kite::RTypes::Prefab) {
			auto ent = currEman->getEntity(currEman->getRoot())->childList()->at(0);
			currEman->createPrefab(ent, (Kite::KPrefab *)currRes);
		}
	}
}

void ObjectDock::rename() {
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

void ObjectDock::prefab() {
	if (currRes == nullptr) {
		return;
	}
	auto item = objTree->currentItem();

	if (item != nullptr) {
		auto entity = currEman->getEntityByName(item->text(0).toStdString());
		auto pre = (Kite::KPrefab *)emit(requestCreateResource(Kite::RTypes::Prefab));
		if (pre != nullptr) {
			currEman->createPrefab(entity->getHandle(), pre);
		}
	}
}

void ObjectDock::search(const QString &Pharase) {
	disconnect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
	auto allItems = QTreeWidgetItemIterator(objTree);
	while (*allItems) {
		if ((*allItems)->text(0).contains(Pharase) && !Pharase.isEmpty()) {
			(*allItems)->setBackgroundColor(0, QColor(Qt::gray));
		} else {
			(*allItems)->setBackgroundColor(0, QColor(50, 50, 50));
		} 
		++allItems;
	}
	connect(objTree, &QTreeWidget::itemChanged, this, &ObjectDock::entityChecked);
}

void ObjectDock::cut() {
	auto item = objTree->currentItem();

	if (item != nullptr) {
		auto entity = currEman->getEntityByName(item->text(0).toStdString());
		clipb.isCopy = false;
		currEman->copyPrefab(entity->getHandle(), &clipb.data, true);
		remove();
		actPaste->setDisabled(false);
	}
}

void ObjectDock::copy() {
	auto item = objTree->currentItem();

	if (item != nullptr) {
		auto entity = currEman->getEntityByName(item->text(0).toStdString());
		clipb.isCopy = true;
		currEman->copyPrefab(entity->getHandle(), &clipb.data, false);
		actPaste->setDisabled(false);
	}
}

void ObjectDock::paste() {
	if (!clipb.data.isEmpty()) {
		auto action = (QAction *)sender();
		auto root = action->data().toBool();
		auto entity = currEman->pastePrefab(&clipb.data);

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
	// copy
	if (!clipb.isCopy) {
		actPaste->setDisabled(true);
		clipb.data.clear();
	}
}




