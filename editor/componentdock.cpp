#include "componentdock.h"
#include <QtWidgets>
#include <qinputdialog.h>
#include "expander.h"
#include "frmexeorder.h"
#include "kmeta.khgen.h"

ComponentDock::ComponentDock(QWidget *Par) :
	QDockWidget("Components Editor", Par),
	eman(nullptr), mtypes(new QMenu(this)), resDict(nullptr)
{
	setObjectName("Components Editor");
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setMinimumWidth(120);

	setupTree();
	setupActions();
	actionsControl(AS_ON_INITE);
	setupHTools();

	Kite::registerKiteMeta(&mman);
}

ComponentDock::~ComponentDock() {}

void ComponentDock::setupTree() {
	comTree = new QTreeWidget(this);
	comTree->setMinimumWidth(330);
	comTree->setHeaderLabel("Components Editor");
	comTree->setHeaderHidden(true);
	comTree->setSelectionMode(QAbstractItemView::SingleSelection);
	comTree->setRootIsDecorated(false);
	comTree->setExpandsOnDoubleClick(false);
	comTree->setIndentation(0);
	comTree->setAnimated(true);
	connect(mtypes, &QMenu::triggered, this, &ComponentDock::actAdd);
	setWidget(comTree);
}

void ComponentDock::setupActions() {
	addDefComp = new QAction(QIcon(":/icons/add"), "Add Logic Component", this);
	addDefComp->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	addDefComp->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(addDefComp, &QAction::triggered, this, &ComponentDock::actAddDef);
	this->addAction(addDefComp);

	collAll = new QAction(QIcon(":/icons/col"), "Collapse All", this);
	connect(collAll, &QAction::triggered, this, &ComponentDock::actCollAll);
	this->addAction(collAll);

	exeOrder = new QAction(QIcon(":/icons/order"), "Execution Order", this);
	//exeOrder->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	//exeOrder->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(exeOrder, &QAction::triggered, this, &ComponentDock::actExeOrder);
	this->addAction(exeOrder);
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
	btnAddComps->setAutoRaise(true);
	btnAddComps->setDefaultAction(addDefComp);
	btnAddComps->setIcon(QIcon(":/icons/add"));
	btnAddComps->setPopupMode(QToolButton::MenuButtonPopup);
	btnAddComps->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnAddComps);

	hlayout->addSpacing(10);

	auto btnExeOrder = new QToolButton(htools);
	btnExeOrder->setDefaultAction(exeOrder);
	btnExeOrder->setAutoRaise(true);
	btnExeOrder->setIcon(QIcon(":/icons/order"));
	btnExeOrder->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);
	hlayout->addWidget(btnExeOrder);

	hlayout->addStretch(1);
	vlayout->addLayout(hlayout);

	auto hlayout2 = new QHBoxLayout(htools);
	hlayout2->setMargin(3);

	auto btnCollpaseAll = new QToolButton(htools);
	btnCollpaseAll->setAutoRaise(true);
	btnCollpaseAll->setDefaultAction(collAll);
	btnCollpaseAll->setIcon(QIcon(":/icons/col"));
	btnCollpaseAll->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout2->addWidget(btnCollpaseAll);

	hlayout2->addSpacing(10);

	ledit = new QLineEdit(htools);
	ledit->setPlaceholderText("Search");
	ledit->addAction(QIcon(":/icons/search"), QLineEdit::ActionPosition::TrailingPosition);
	ledit->setStyleSheet("background-color: gray;");
	connect(ledit, &QLineEdit::textChanged, this, &ComponentDock::actSearch);
	hlayout2->addWidget(ledit);

	vlayout->addLayout(hlayout2);

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

void ComponentDock::inite(const QStringList &TypeList, const QHash<QString, Kite::KResource *> *Dictionary) {
	types = TypeList;
	
	mtypes->clear();
	for (auto it = types.begin(); it != types.end(); ++it) {
		auto comtype = new QAction(QIcon(":/icons/comp32"), (*it), mtypes);
		mtypes->addAction(comtype);
	}

	resDict = Dictionary;
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
	
	auto header = new Expander(Comp, resDict, comTree);
	connect(header, &Expander::closeClicked, this, &ComponentDock::actRemove);
	connect(header, &Expander::componentEdited, this, &ComponentDock::actEdit);

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

void ComponentDock::entityEdit(Kite::KEntityManager *Eman, Kite::KEntity *Entity) {
	if (Entity->getHandle() == currEntity) {
		return;
	}
	eman = Eman;
	actClear();

	currEntity = Entity->getHandle();
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

	std::vector<Kite::KComponent *> compList;
	Entity->getScriptComponents(compList);
	for (auto it = compList.begin(); it != compList.end(); ++it) {
		fetchFromPool((*it));
	}

	actionsControl(AS_ON_LOAD);
	QString name(Entity->getName().c_str());
	hlabel->setText("Components Editor (" + name + ")");
	actSearch(ledit->text());
}

void ComponentDock::entityDelete(Kite::KEntityManager *Eman, Kite::KEntity *Entity) {
	if (Eman == nullptr || Entity == nullptr) {
		actClear();
		actionsControl(AS_ON_INITE);
		currEntity = Kite::KHandle();
		eman = nullptr;
		hlabel->setText("Components Editor");
	}
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

		auto comp = ent->addComponent("Logic", text.toStdString());
		fetchFromPool(comp);
		//createComponent(ent, comp);
		emit(componentAdded(ent, comp));
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
		auto comp = ent->addComponent(Action->text().toStdString(), Action->text().toStdString());
		fetchFromPool(comp);
		//createComponent(ent, cptr);
		emit(componentAdded(ent, comp));
	}

}

void ComponentDock::actAddDef() {
	auto ent = eman->getEntity(currEntity);
	auto text = getAvailName(ent);
	if (text.isEmpty()) {
		return;
	}

	auto comp = ent->addComponent("Logic", text.toStdString());
	fetchFromPool(comp);
	//createComponent(ent, ent->getComponent("Logic", chandle));
	emit(componentAdded(ent, comp));
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
		if ((*it)->getTreeItem()->text(0).contains(Pharase, Qt::CaseInsensitive) && !Pharase.isEmpty()) {
			(*it)->highlight(true);
		} else {
			(*it)->highlight(false);
		}
	}
}

