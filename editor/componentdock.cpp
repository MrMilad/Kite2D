#include "componentdock.h"
#include <QtWidgets>
#include <qinputdialog.h>
#include "expander.h"
#include "Kite/meta/kmetaclass.h"
#include "kmeta.khgen.h"

ComponentDock::ComponentDock(QWidget *Par) :
	QDockWidget("Components Editor", Par),
	eman(nullptr), mtypes(new QMenu(this))
{
	setObjectName("Components Editor");
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setStyleSheet("QDockWidget { border: 3px solid; }");

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

void ComponentDock::remComCallb(Kite::KComponent *RemovedComp, void *Ptr) {
	auto cls = (ComponentDock *)Ptr;
	for (auto it = cls->treeList.begin(); it != cls->treeList.end(); ++it) {
		if (RemovedComp->getType() == (*it)->getCType() &&
			RemovedComp->getHandle() == (*it)->getCHndle()) {
			cls->putIntoPool((*it));
			break;
		}
	}
	emit(cls->componentDelete(cls->eman->getEntity(cls->currEntity), RemovedComp));
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

void ComponentDock::layerChanged(int Value) {
	if (eman != nullptr) {
		auto ent = eman->getEntity(currEntity);
		if (ent) {
			ent->setLayer((unsigned char)Value);
			emit(entityLayerChanged(ent));
		}
	}
}

void ComponentDock::zorderChanged(int Value) {
	if (eman != nullptr) {
		auto ent = eman->getEntity(currEntity);
		if (ent) {
			ent->setZOrder(Value);
			emit(entityZOrderChanged(ent));
		}
	}
}

void ComponentDock::staticChanged(int Value) {
	if (eman != nullptr) {
		auto ent = eman->getEntity(currEntity);
		if (ent) {
			bool state = false;
			if (Value == Qt::CheckState::Checked) state = true;
			ent->setStatic(state);
			emit(entityStaticChanged(ent));
		}
	}
}

void ComponentDock::setupTree() {
	auto mainFrame = new QFrame(this);
	auto vlayout = new QVBoxLayout(mainFrame);
	vlayout->setMargin(0);
	vlayout->setSpacing(0);

	// tree
	comTree = new QTreeWidget(this);
	comTree->setMinimumWidth(230);
	comTree->setHeaderLabel("Components Editor");
	comTree->setHeaderHidden(true);
	comTree->setSelectionMode(QAbstractItemView::SingleSelection);
	comTree->setExpandsOnDoubleClick(false);
	comTree->setRootIsDecorated(false);
	comTree->setIndentation(0);
	comTree->setAnimated(true);
	comTree->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	comTree->setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);
	comTree->setStyleSheet("QTreeView {background-color: rgb(88,88,88);"
						   "padding-right: 3px;"
						   "padding-left: 3px;"
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

	auto hlayoutTitle = new QHBoxLayout();
	hlabel = new QLabel(htools);
	hlabel->setText("Components Editor ");
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

	auto hlayout = new QHBoxLayout(htools);
	hlayout->setMargin(0);

	auto btnAddComps = new QToolButton(htools);
	btnAddComps->setMenu(mtypes);
	btnAddComps->setDefaultAction(addDefComp);
	btnAddComps->setIcon(QIcon(":/icons/add"));
	btnAddComps->setPopupMode(QToolButton::MenuButtonPopup);
	btnAddComps->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnAddComps);

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

	spnLayer = new QSpinBox(this);
	spnLayer->setMinimum(0);
	spnLayer->setMaximum(KENTITY_LAYER_SIZE);
	connect(spnLayer, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ComponentDock::layerChanged);
	hlayout2->addWidget(spnLayer, 1);
	hlayout2->addSpacing(5);

	auto lblOrder = new QLabel(this);
	lblOrder->setText("Z: ");
	hlayout2->addWidget(lblOrder);

	spnZOrder = new QSpinBox(this);
	spnZOrder->setMinimum(0);
	spnZOrder->setMaximum(9999999);
	spnZOrder->setValue(0);
	connect(spnZOrder, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ComponentDock::zorderChanged);
	hlayout2->addWidget(spnZOrder, 1);
	hlayout2->addSpacing(5);

	// static
	chkStatic = new QCheckBox(this);
	chkStatic->setText("Static");
	connect(chkStatic, &QCheckBox::stateChanged, this, &ComponentDock::staticChanged);
	hlayout2->addWidget(chkStatic);
	hlayout2->addSpacing(5);

	vlayout->addLayout(hlayout2);

	auto hlayout3 = new QHBoxLayout(htools);
	hlayout3->setMargin(0);
	hlayout3->setSpacing(0);

	auto btnCollpaseAll = new QToolButton(htools);
	btnCollpaseAll->setIcon(QIcon(":/icons/col"));
	btnCollpaseAll->setToolButtonStyle(Qt::ToolButtonIconOnly);
	connect(btnCollpaseAll, &QToolButton::clicked, comTree, &QTreeWidget::collapseAll);
	hlayout3->addWidget(btnCollpaseAll);

	hlayout3->addSpacing(5);

	ledit = new QLineEdit(htools);
	ledit->setPlaceholderText("Search");
	ledit->addAction(QIcon(":/icons/search"), QLineEdit::ActionPosition::TrailingPosition);
	ledit->setStyleSheet("background-color: gray;");
	connect(ledit, &QLineEdit::textChanged, this, &ComponentDock::actSearch);
	hlayout3->addWidget(ledit, 1);

	vlayout->addLayout(hlayout3);

	htools->setLayout(vlayout);
	setTitleBarWidget(htools);
}

void ComponentDock::actionsControl(ActionsState State) {
	if (State == AS_ON_INITE) {
		addDefComp->setDisabled(true);
		mtypes->setDisabled(true);
		collAll->setDisabled(true);
	} else if (State == AS_ON_LOAD) {
		addDefComp->setDisabled(false);
		mtypes->setDisabled(false);
		collAll->setDisabled(false);
	}
}

void ComponentDock::inite(const QVector<QPair<Kite::CTypes, bool>> &TypeList) {
	types = TypeList;
	mtypes->clear();
	for (auto it = types.begin(); it != types.end(); ++it) {
		// is visible
		if (it->second) {
			auto comtype = new QAction(QIcon(":/icons/comp32"), Kite::getCTypesName(it->first).c_str(), mtypes);
			comtype->setData((size_t)it->first);
			mtypes->addAction(comtype);
		}
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
		auto isExist = Entity->hasComponent(Kite::CTypes::Logic, text.toStdString());
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

Expander *ComponentDock::createCom(Kite::CTypes Type) {
	auto fakeEman = Kite::KEntityManager();
	auto fakeEnt = fakeEman.createEntity("fake");
	auto Comp = fakeEnt->addComponent(Type, "");
	
	auto header = new Expander(Comp, comTree);
	connect(header, &Expander::closeClicked, this, &ComponentDock::actRemove);
	connect(header, &Expander::componentEdited, this, &ComponentDock::actEdit);
	connect(header, &Expander::updateResList, this, &ComponentDock::updateResList);
	connect(header, &Expander::requestPropValue, this, &ComponentDock::getPropValue);
	connect(header, &Expander::requestRes, this, &ComponentDock::requestResource);

	return header;
}

void ComponentDock::addLogicToPool(int Count) {
	if ((treePool.logicPool.size() + Count) > treePool.logicPool.capacity()) {
		treePool.logicPool.reserve(treePool.logicPool.size() + Count);
	}
	while (Count > 0) {
		auto head = createCom(Kite::CTypes::Logic);
		treePool.logicPool.push_back(head);
		--Count;
	}
}

void ComponentDock::initePool(const QVector<QPair<Kite::CTypes, bool>> &TypeList, unsigned int LogicCount) {
	// transform component shuld be at top
	auto trHead = createCom(Kite::CTypes::Transform);
	treePool.fixedPool.insert((size_t)Kite::CTypes::Transform, trHead);
	
	for (auto it = TypeList.begin(); it != TypeList.end(); ++it) {
		// is visible
		if (it->second) {
			if (it->first == Kite::CTypes::Logic) {
				addLogicToPool(LogicCount);
				continue;
			} else if (it->first == Kite::CTypes::Transform) continue;

			auto head = createCom(it->first);
			treePool.fixedPool.insert((size_t)it->first, head);
		}
	}
}

void ComponentDock::fetchFromPool(Kite::KComponent *Comp) {
	Expander *expand = nullptr;
	if (Comp->getType() == Kite::CTypes::Logic) {
		if (treePool.logicPool.empty()) {
			addLogicToPool(5); 
		}
		expand = treePool.logicPool.back();
		expand->getTreeItem()->setExpanded(false);
		treePool.logicPool.pop_back();
	} else {
		// invisible components
		if (treePool.fixedPool.find((size_t)Comp->getType()) == treePool.fixedPool.end()) {
			return;
		}

		// visible components
		expand = treePool.fixedPool[(size_t)Comp->getType()];
	}
	
	expand->reset(Comp);
	expand->getTreeItem()->setHidden(false);
	treeList.push_back(expand);
}

void ComponentDock::putIntoPool(Expander *Exp) {
	for (auto it = treeList.begin(); it != treeList.end(); it) {
		if ((*it) == Exp || Exp == nullptr) {
			if ((*it)->getCType() == Kite::CTypes::Logic) {
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
	this->setDisabled(false);
	eman = Eman;
	actClear();

	// entity info
	currEntity = Entity->getHandle();

	// set callbacks
	Entity->setAddComCallback(addComCallb, this);
	Entity->setRemoveComCallback(remComCallb, this);
	// prefab frame
	showFrame(Entity, isPrefab);

	// components
	for (auto it = types.begin(); it != types.end(); ++it) {
		if (it->first == Kite::CTypes::Logic) {
			// load logic components at the end
			continue;
		} else {
			// is visible
			if (it->second) {
				auto has = Entity->hasComponent(it->first);
				if (has) {
					auto comp = Entity->getComponent(it->first);
					fetchFromPool(comp);
				}
			}
		}
	}

	std::vector<Kite::KHandle> compList;
	Entity->getScriptComponents(compList);
	for (auto it = compList.begin(); it != compList.end(); ++it) {
		auto comp = Entity->getComponentByHandle((*it));
		fetchFromPool(comp);
	}

	actionsControl(AS_ON_LOAD);
	QString name(Entity->getName().c_str());
	//hlabel->setText("Components Editor (" + name + ")");
	// lua table
	llabel->setText(QString("Lua Table: <font color = \"orange\">") + Entity->getLuaTName().c_str() + "</font>");
	chkStatic->setChecked(Entity->getStatic());
	spnLayer->setValue(Entity->getLayer());
	spnZOrder->setValue(Entity->getZOrder());

	actSearch(ledit->text());
}

void ComponentDock::entityDelete(Kite::KEntityManager *Eman, Kite::KEntity *Entity) {
	if (Eman == nullptr || Entity == nullptr) {
		actClear();
		actionsControl(AS_ON_INITE);
		currEntity = Kite::KHandle();
		eman = nullptr;
		llabel->clear();
		this->setDisabled(true);
		return;
	}

	if (Entity->getHandle() == currEntity) {
		actClear();
		actionsControl(AS_ON_INITE);
		currEntity = Kite::KHandle();
		eman = nullptr;
		llabel->clear();
		this->setDisabled(true);
	}
}

Kite::KAny ComponentDock::getPropValue(Kite::CTypes Type, const QString &ComName, const QString &PropName) {
	auto ent = eman->getEntity(currEntity);
	if (ent) {
		auto comp = ent->getComponent(Type, ComName.toStdString());
		if (comp) {
			return comp->getProperty(PropName.toStdString());
		}
	}
	return Kite::KAny();
}

void ComponentDock::updateResList(Kite::RTypes Type, QStringList &List) {
	emit(requestResNames(Type, List));
}

void ComponentDock::actCollAll() {
	comTree->collapseAll();
}

void ComponentDock::actAdd(QAction *Action) {
	auto ent = eman->getEntity(currEntity);
	// logic components need a unique name
	if (Action->text() == "Logic") {
		auto text = getAvailName(ent);
		if (text.isEmpty()) {
			return;
		}

		ent->addComponent(Kite::CTypes::Logic, text.toStdString());
		// fixed components
	} else {
		// available name
		auto isExist = ent->hasComponentType((Kite::CTypes)Action->data().toUInt());
		if (isExist) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("this component is already exist!");
			msg.exec();
			return;
		}
		ent->addComponent((Kite::CTypes)Action->data().toUInt(), Action->text().toStdString());
	}

}

void ComponentDock::actAddDef() {
	auto ent = eman->getEntity(currEntity);
	auto text = getAvailName(ent);
	if (text.isEmpty()) {
		return;
	}

	ent->addComponent(Kite::CTypes::Logic, text.toStdString());
}

void ComponentDock::actRemove(Kite::KHandle CHandle) {
	//auto obj = (Expander *)sender();
	auto ent = eman->getEntity(currEntity);
	auto cptr = ent->getComponentByHandle(CHandle);
	if (cptr->getDepCounter() > 0) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("some components required this component. remove them first!");
		msg.exec();
		return;
	}
	ent->removeComponent((Kite::CTypes) CHandle.type, cptr->getName());
	eman->postWork();
}

void ComponentDock::actEdit(Kite::KHandle Chandle, const QString &Pname, QVariant &Value) {
	Kite::KAny *vptr = (Kite::KAny *)Value.value<void *>();
	auto ent = eman->getEntity(currEntity);
	auto comp = ent->getComponentByHandle(Chandle);
	comp->setProperty(Pname.toStdString(), *vptr);
	emit(componentEdited(ent, comp, Pname));
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
