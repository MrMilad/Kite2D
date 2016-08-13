#include "componentdock.h"
#include <QtWidgets>
#include <qinputdialog.h>
#include "expander.h"
#include "frmexeorder.h"
#include "kmeta.khgen.h"

ComponentDock::ComponentDock(QWidget *Par) :
	QDockWidget("Components Editor", Par),
	eman(nullptr), mtypes(new QMenu(this))
{
	setObjectName("Components Editor");
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	setupActions();
	setupTree();
	actionsControl(AS_ON_INITE);
	setupHTools();

	Kite::registerKiteMeta(&mman);
}

ComponentDock::~ComponentDock() {}

void ComponentDock::addComCallb(Kite::KComponent *NewComp, void *Ptr) {
	auto cls = (ComponentDock *)Ptr;
	cls->fetchFromPool(NewComp);
	emit(cls->componentAdded(cls->eman->getEntity(cls->currEntity), NewComp));
}

void ComponentDock::onExpand(const QModelIndex &Index) {
	auto frame = (QFrame *)comTree->indexWidget(Index);
	auto btnExpand = frame->findChild<QPushButton *>("btnExpand");

	btnExpand->setIcon(QIcon(":/icons/col"));
	btnExpand->setStyleSheet("QPushButton { text-align: left;"
									 "color: rgb(255, 255, 255);"
									 "background-color: rgb(38, 38, 38);"
									 "border: 1px solid rgb(38, 38, 38);"
									 "border-top-left-radius: 3px;"
									 "border-top-right-radius: 3px;}");
}

void ComponentDock::onCollpase(const QModelIndex &Index) {
	auto frame = (QFrame *)comTree->indexWidget(Index);
	auto btnExpand = frame->findChild<QPushButton *>("btnExpand");

	btnExpand->setIcon(QIcon(":/icons/exp"));
	btnExpand->setStyleSheet("QPushButton { text-align: left;"
							 "color: rgb(255, 255, 255);"
							 "background-color: rgb(38, 38, 38);"
							 "border: 1px solid rgb(38, 38, 38);"
							 "border-radius: 3px;}");
}

void ComponentDock::setupTree() {
	auto mainFrame = new QFrame(this);
	auto vlayout = new QVBoxLayout(mainFrame);
	vlayout->setMargin(0);
	vlayout->setSpacing(0);

	// tree
	comTree = new QTreeWidget(this);
	comTree->setMinimumWidth(280);
	comTree->setHeaderLabel("Components Editor");
	comTree->setHeaderHidden(true);
	comTree->setSelectionMode(QAbstractItemView::SingleSelection);
	comTree->setExpandsOnDoubleClick(false);
	comTree->setRootIsDecorated(false);
	comTree->setIndentation(0);
	comTree->setAnimated(true);
	comTree->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	comTree->setStyleSheet("QTreeView {background-color: rgb(88,88,88);"
						   "padding: 3px;"
						   "selection-background-color: transparent;}");

	connect(comTree, &QTreeWidget::expanded, this, &ComponentDock::onExpand);
	connect(comTree, &QTreeWidget::collapsed, this, &ComponentDock::onCollpase);

	vlayout->addWidget(comTree);

	// prefab frame
	prefabFrame = new QFrame(this);
	auto hLayout = new QHBoxLayout(prefabFrame);
	hLayout->setMargin(0);
	hLayout->setSpacing(0);

	auto btnApply = new QToolButton(prefabFrame);
	preApply->setData(qVariantFromValue((void *)btnApply));
	btnApply->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
	btnApply->setDefaultAction(preApply);
	btnApply->setToolButtonStyle(Qt::ToolButtonTextOnly);
	hLayout->addWidget(btnApply);

	auto btnSelect = new QToolButton(prefabFrame);
	preSelect->setData(qVariantFromValue((void *)btnSelect));
	btnSelect->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
	btnSelect->setDefaultAction(preSelect);
	btnSelect->setToolButtonStyle(Qt::ToolButtonTextOnly);
	hLayout->addWidget(btnSelect);

	auto btnRevert = new QToolButton(prefabFrame);
	preRevert->setData(qVariantFromValue((void *)btnRevert));
	btnRevert->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
	btnRevert->setDefaultAction(preRevert);
	btnRevert->setToolButtonStyle(Qt::ToolButtonTextOnly);
	hLayout->addWidget(btnRevert);

	vlayout->addWidget(prefabFrame);

	prefabFrame->hide();
	
	connect(mtypes, &QMenu::triggered, this, &ComponentDock::actAdd);
	this->setWidget(mainFrame);
}

void ComponentDock::setupActions() {
	addDefComp = new QAction(QIcon(":/icons/add"), "Add Logic Component", this);
	addDefComp->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	addDefComp->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(addDefComp, &QAction::triggered, this, &ComponentDock::actAddDef);
	this->addAction(addDefComp);

	collAll = new QAction(QIcon(":/icons/colall"), "Collapse All", this);
	connect(collAll, &QAction::triggered, this, &ComponentDock::actCollAll);
	this->addAction(collAll);

	exeOrder = new QAction(QIcon(":/icons/order"), "Logic Execution Order", this);
	//exeOrder->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	//exeOrder->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(exeOrder, &QAction::triggered, this, &ComponentDock::actExeOrder);
	this->addAction(exeOrder);

	preSelect = new QAction("Select", this);
	connect(preSelect, &QAction::triggered, this, &ComponentDock::actSelectPrefab);
	this->addAction(preSelect);

	preRevert = new QAction("Revert", this);
	connect(preRevert, &QAction::triggered, this, &ComponentDock::actRevertPrefab);
	this->addAction(preRevert);

	preApply = new QAction("Apply", this);
	connect(preApply, &QAction::triggered, this, &ComponentDock::actApllyPrefab);
	this->addAction(preApply);
}

void ComponentDock::setupHTools() {
	htools = new QFrame(this);
	auto vlayout = new QVBoxLayout(htools);
	vlayout->setMargin(2);
	vlayout->setSpacing(0);

	hlabel = new QLabel(htools);
	hlabel->setText("Components Editor");
	hlabel->setStyleSheet("color: DodgerBlue;");
	vlayout->addWidget(hlabel);

	auto hlayout = new QHBoxLayout(htools);
	hlayout->setMargin(3);

	auto btnAddComps = new QToolButton(htools);
	btnAddComps->setMenu(mtypes);
	btnAddComps->setDefaultAction(addDefComp);
	btnAddComps->setIcon(QIcon(":/icons/add"));
	btnAddComps->setPopupMode(QToolButton::MenuButtonPopup);
	btnAddComps->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnAddComps);

	hlayout->addSpacing(10);

	auto btnExeOrder = new QToolButton(htools);
	btnExeOrder->setDefaultAction(exeOrder);
	btnExeOrder->setIcon(QIcon(":/icons/order"));
	btnExeOrder->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
	hlayout->addWidget(btnExeOrder);

	hlayout->addStretch(1);

	// lua table name
	llabel = new QLabel(htools);
	hlayout->addWidget(llabel);

	vlayout->addLayout(hlayout);

	auto hlayout2 = new QHBoxLayout(htools);
	hlayout2->setMargin(0);

	auto lblLayer = new QLabel(this);
	lblLayer->setText("Layer: ");
	hlayout2->addWidget(lblLayer);

	cmbLayer = new QComboBox(this);
	hlayout2->addWidget(cmbLayer, 1);
	hlayout2->addSpacing(5);

	auto chkStatic = new QCheckBox(this);
	chkStatic->setText("Static");
	hlayout2->addWidget(chkStatic);

	vlayout->addLayout(hlayout2);

	auto hlayout3 = new QHBoxLayout(htools);
	hlayout3->setMargin(3);

	auto btnCollpaseAll = new QToolButton(htools);
	btnCollpaseAll->setDefaultAction(collAll);
	btnCollpaseAll->setIcon(QIcon(":/icons/col"));
	btnCollpaseAll->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout3->addWidget(btnCollpaseAll);

	hlayout3->addSpacing(10);

	ledit = new QLineEdit(htools);
	ledit->setPlaceholderText("Search");
	ledit->addAction(QIcon(":/icons/search"), QLineEdit::ActionPosition::TrailingPosition);
	ledit->setStyleSheet("background-color: gray;");
	connect(ledit, &QLineEdit::textChanged, this, &ComponentDock::actSearch);
	hlayout3->addWidget(ledit);

	vlayout->addLayout(hlayout3);

	htools->setLayout(vlayout);
	setTitleBarWidget(htools);
}

void ComponentDock::actionsControl(ActionsState State) {
	if (State == AS_ON_INITE) {
		addDefComp->setDisabled(true);
		mtypes->setDisabled(true);
		collAll->setDisabled(true);
		exeOrder->setDisabled(true);
	} else if (State == AS_ON_LOAD) {
		addDefComp->setDisabled(false);
		mtypes->setDisabled(false);
		collAll->setDisabled(false);
		exeOrder->setDisabled(false);
	}
}

void ComponentDock::inite(const QStringList &TypeList) {
	types = TypeList;
	
	mtypes->clear();
	for (auto it = types.begin(); it != types.end(); ++it) {
		auto comtype = new QAction(QIcon(":/icons/comp32"), (*it), mtypes);
		mtypes->addAction(comtype);
	}


	initePool(TypeList);
}

QString ComponentDock::getAvailName(Kite::KEntity *Entity) {
	QString text;
	bool ok;
	do {
		text = QInputDialog::getText(this, tr("New Logic Component"),
									 tr("Component name:"), QLineEdit::Normal,
									 "", &ok);
		// cancel pressed
		if (!ok) {
			text.clear();
			break;
		}

		// empty name
		if (text.isEmpty()) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("component name is empty!");
			msg.exec();
			continue;
		}

		// available name
		auto isExist = Entity->hasComponent("Logic", text.toStdString());
		if (isExist) {
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

Expander *ComponentDock::createCom(const QString &CType) {
	auto fakeEman = Kite::KEntityManager();
	auto fakeEnt = fakeEman.createEntity("fake");
	auto Comp = fakeEnt->addComponent(CType.toStdString(), "");
	
	auto header = new Expander(Comp, comTree);
	connect(header, &Expander::closeClicked, this, &ComponentDock::actRemove);
	connect(header, &Expander::componentEdited, this, &ComponentDock::actEdit);
	connect(header, &Expander::updateResList, this, &ComponentDock::updateResList);

	return header;
}

void ComponentDock::addLogicToPool(int Count) {
	if ((treePool.logicPool.size() + Count) > treePool.logicPool.capacity()) {
		treePool.logicPool.reserve(treePool.logicPool.size() + Count);
	}
	while (Count > 0) {
		auto head = createCom("Logic");
		treePool.logicPool.push_back(head);
		--Count;
	}
}

void ComponentDock::initePool(const QStringList &TypeList, unsigned int LogicCount) {
	for (auto it = TypeList.begin(); it != TypeList.end(); ++it) {
		auto head = createCom((*it));

		if ((*it) == "Logic") {
			addLogicToPool(LogicCount);
			continue;
		}
		treePool.fixedPool.insert((*it), head);
	}
}

void ComponentDock::fetchFromPool(Kite::KComponent *Comp) {
	Expander *expand = nullptr;
	if (Comp->getType() == "Logic") {
		if (treePool.logicPool.empty()) {
			addLogicToPool(5); 
		}
		expand = treePool.logicPool.back();
		expand->getTreeItem()->setExpanded(false);
		treePool.logicPool.pop_back();
	} else {
		expand = treePool.fixedPool[Comp->getType().c_str()];
	}
	
	expand->reset(Comp);
	expand->getTreeItem()->setHidden(false);
	treeList.push_back(expand);
}

void ComponentDock::putIntoPool(Expander *Exp) {
	for (auto it = treeList.begin(); it != treeList.end(); it) {
		if ((*it) == Exp || Exp == nullptr) {
			if ((*it)->getCType() == "Logic") {
				treePool.logicPool.push_back((*it));
			}
			(*it)->getTreeItem()->setHidden(true);
			it = treeList.erase(it);
			continue;
		}
		++it;
	}
}

void ComponentDock::showFrame(Kite::KEntity *Entity, bool isPrefab) {
	// prefab frame
	if (isPrefab) {
		llabel->hide();
		auto btn = (QToolButton *)preSelect->data().value<void *>();
		btn->hide();
		preSelect->setDisabled(true);

		btn = (QToolButton *)preRevert->data().value<void *>();
		btn->hide();
		preRevert->setDisabled(true);

		btn = (QToolButton *)preApply->data().value<void *>();
		btn->show();
		preApply->setDisabled(false);
	} else {
		llabel->show();
		auto btn = (QToolButton *)preSelect->data().value<void *>();
		btn->show();
		preSelect->setDisabled(false);

		btn = (QToolButton *)preRevert->data().value<void *>();
		btn->show();
		preRevert->setDisabled(false);

		btn = (QToolButton *)preApply->data().value<void *>();
		btn->hide();
		preApply->setDisabled(true);
	}

	if (Entity->isPrefab() || isPrefab) {
		prefabFrame->show();
	} else {
		prefabFrame->hide();
	}
}

void ComponentDock::entityEdit(Kite::KEntityManager *Eman, Kite::KEntity *Entity, bool isPrefab) {
	if (Entity->getHandle() == currEntity && Eman == eman) {
		return;
	}
	eman = Eman;
	actClear();

	// entity info
	currEntity = Entity->getHandle();

	// set callback
	Entity->setAddComCallback(addComCallb, this);

	// prefab frame
	showFrame(Entity, isPrefab);

	// components
	for (auto it = types.begin(); it != types.end(); ++it) {
		if ((*it) == "Logic") {
			// load logic components at the end
			continue;
		} else {
			auto has = Entity->hasComponent(it->toStdString(), it->toStdString());
			if (has) {
				auto comp = Entity->getComponentByName(it->toStdString(), it->toStdString());
				fetchFromPool(comp);
			}
		}
	}

	std::vector<Kite::KHandle> compList;
	Entity->getScriptComponents(compList);
	for (auto it = compList.begin(); it != compList.end(); ++it) {
		auto comp = Entity->getComponent("Logic", (*it));
		fetchFromPool(comp);
	}

	actionsControl(AS_ON_LOAD);
	QString name(Entity->getName().c_str());
	hlabel->setText("Components Editor (" + name + ")");
	llabel->setText(QString("Lua Table: <font color = \"orange\">") + Entity->getLuaTName().c_str() + "</font>");
	actSearch(ledit->text());
}

void ComponentDock::entityDelete(Kite::KEntityManager *Eman, Kite::KEntity *Entity) {
	if (Eman == nullptr || Entity == nullptr) {
		actClear();
		actionsControl(AS_ON_INITE);
		currEntity = Kite::KHandle();
		eman = nullptr;
		hlabel->setText("Components Editor");
		llabel->clear();
	}
}

void ComponentDock::updateResList(const QString &Type, QStringList &List) {
	emit(requestResNames(Type, List));
}

void ComponentDock::actExeOrder() {
	auto ent = eman->getEntity(currEntity);
	frmexeorder frm(ent, this);
	frm.exec();
}

void ComponentDock::actCollAll() {
	for (auto it = treeList.begin(); it != treeList.end(); ++it) {
		(*it)->getTreeItem()->setExpanded(false);
	}
}

void ComponentDock::actAdd(QAction *Action) {
	auto ent = eman->getEntity(currEntity);
	// logic components need a unique name
	if (Action->text() == "Logic") {
		auto text = getAvailName(ent);
		if (text.isEmpty()) {
			return;
		}

		ent->addComponent("Logic", text.toStdString());
		// fixed components
	} else {
		// available name
		auto isExist = ent->hasComponentType(Action->text().toStdString());
		if (isExist) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("this component is already exist!");
			msg.exec();
			return;
		}
		ent->addComponent(Action->text().toStdString(), Action->text().toStdString());
	}

}

void ComponentDock::actAddDef() {
	auto ent = eman->getEntity(currEntity);
	auto text = getAvailName(ent);
	if (text.isEmpty()) {
		return;
	}

	ent->addComponent("Logic", text.toStdString());
}

void ComponentDock::actRemove(Kite::KHandle CHandle, const QString &CType) {
	auto obj = (Expander *)sender();
	auto ent = eman->getEntity(currEntity);
	auto cptr = ent->getComponent(CType.toStdString(), CHandle);
	emit(componentDelete(ent, cptr));
	putIntoPool(obj);
	ent->removeComponent(CType.toStdString(), cptr->getName());
}

void ComponentDock::actEdit(Kite::KHandle Chandle, const QString &CType, const QString &Pname, QVariant &Value) {
	Kite::KAny *vptr = (Kite::KAny *)Value.value<void *>();
	eman->getEntity(currEntity)->getComponent(CType.toStdString(), Chandle)->setProperty(Pname.toStdString(), *vptr);
	//emit(componentEdited(currEntity, Chandle, Pname));
}

void ComponentDock::actClear() {
	putIntoPool(nullptr);
}

void ComponentDock::actSearch(const QString &Pharase) {
	for (auto it = treeList.begin(); it != treeList.end(); ++it) {
		if ((*it)->getTreeItem()->text(0).contains(Pharase, Qt::CaseInsensitive) || Pharase.isEmpty()) {
			(*it)->highlight(true);
		} else {
			(*it)->highlight(false);
		}
	}
}

void ComponentDock::actSelectPrefab() {
	auto ent = eman->getEntity(currEntity);
	emit(resSelected(ent->getPrefab().c_str()));
}

void ComponentDock::actRevertPrefab() {
	auto ent = eman->getEntity(currEntity);
	emit(revertPrefab(ent));
}

void ComponentDock::actApllyPrefab() {
	auto ent = eman->getEntity(currEntity);
	emit(applyPrefab(ent));
}
