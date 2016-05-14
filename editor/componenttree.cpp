#include "componenttree.h"
#include "expander.h"
#include <QtWidgets>
#include <qinputdialog.h>
#include "comproperty.h"
#include "kmeta.khgen.h"

ComponentTree::ComponentTree(QWidget *Par) :
	QTreeWidget(Par), currEntity(nullptr), mtypes(new QMenu(this)), resDict(nullptr)
{
	setMinimumWidth(330);
	setHeaderLabel("Components Editor");
	setHeaderHidden(true);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setRootIsDecorated(false);
	setIndentation(0);
	setAnimated(true);
	connect(this, &QTreeWidget::customContextMenuRequested, this, &ComponentTree::actRClicked);
	connect(mtypes, &QMenu::triggered, this, &ComponentTree::actAdd);

	setupActions();
	actionsControl(AS_ON_INITE);
	setupHTools();

	Kite::registerKiteMeta(&mman);
}

ComponentTree::~ComponentTree() {}

void ComponentTree::setupActions() {
	remComp = new QAction(QIcon(":/icons/remove"), "Remove Component", this);
	connect(remComp, &QAction::triggered, this, &ComponentTree::actRemove);

	addDefComp = new QAction(QIcon(":/icons/add"), "Add Logic Component", this);
	addDefComp->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	addDefComp->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(addDefComp, &QAction::triggered, this, &ComponentTree::actAddDef);
	this->addAction(addDefComp);
}

void ComponentTree::setupHTools() {
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

	ledit = new QLineEdit(htools);
	ledit->addAction(QIcon(":/icons/search"), QLineEdit::ActionPosition::TrailingPosition);
	ledit->setStyleSheet("background-color: gray;");
	connect(ledit, &QLineEdit::textChanged, this, &ComponentTree::actSearch);

	hlayout->addWidget(ledit);

	vlayout->addLayout(hlayout);

	htools->setLayout(vlayout);
}

void ComponentTree::actionsControl(ActionsState State) {
	if (State == AS_ON_INITE) {
		addDefComp->setDisabled(true);
		remComp->setDisabled(true);
		mtypes->setDisabled(true);
	} else if (State == AS_ON_LOAD) {
		addDefComp->setDisabled(false);
		remComp->setDisabled(false);
		mtypes->setDisabled(false);
	}
}

void ComponentTree::setupTypes(const QStringList &TypeList) {
	types = TypeList;
	
	mtypes->clear();
	for (auto it = types.begin(); it != types.end(); ++it) {
		auto comtype = new QAction(QIcon(":/icons/comp32"), (*it), mtypes);
		mtypes->addAction(comtype);
	}
}

QString ComponentTree::getAvailName(Kite::KEntity *Entity) {
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

bool ComponentTree::eventFilter(QObject *object, QEvent *event) {
	if (event->type() == QEvent::MouseButtonPress) {
		auto combo = (QComboBox *)object;

		auto text = combo->currentText();
		combo->clear();
		auto type = combo->objectName();

		if (resDict != nullptr) {
			QStringList items;
			for (auto it = resDict->cbegin(); it != resDict->cend(); ++it) {
				if ((*it)->getResourceType().c_str() == type) {
					items.push_back(it.key());
				}
			}

			combo->addItems(items);
			combo->setCurrentText(text);
		}
	}
	return false;
}

void ComponentTree::removeComponentGUI() {
	auto item = currentItem();
	if (item != nullptr) {
		auto child = item->child(0);
		auto expander = (Expander *)itemWidget(item, 0);
		auto cframe = (QFrame *)itemWidget(child, 0);
		delete child;
		delete item;
		delete expander;
		delete cframe;
	}
}

void ComponentTree::createComponent(const Kite::KEntity *Entity, const Kite::KComponent *Comp) {
	// header
	auto category = new QTreeWidgetItem(this);
	QString name;
	if (Comp->getClassName() == "KLogicCom") {
		QString lname(Comp->getName().c_str());
		name = "Logic\t" + lname;
	} else {
		name = Comp->getName().c_str();
		name.append("\t");
	}
	category->setText(0, name);
	auto btnExpander = new Expander(name, QIcon(":/icons/com"), this, category);
	btnExpander->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	btnExpander->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(btnExpander, &Expander::customContextMenuRequested, this, &ComponentTree::actRClicked);
	this->setItemWidget(category, 0, btnExpander);

	// contetnts
	auto cframe = new QFrame(this);
	bindProperties(Entity, Comp, cframe);

	auto pContainer = new QTreeWidgetItem(category);
	pContainer->setDisabled(true);
	this->setItemWidget(pContainer, 0, cframe);
}

void ComponentTree::bindProperties(const Kite::KEntity *Entity, const Kite::KComponent *Comp, QFrame *Frame) {
	auto compMeta = (Kite::KMetaClass *)mman.getMeta(Comp->getClassName().c_str());
	auto propList = compMeta->getProperties();
	auto flayout = new QFormLayout(Frame);

	flayout->setContentsMargins(10, 0, 10, 0);
	flayout->setHorizontalSpacing(5);
	flayout->setVerticalSpacing(3);
	for (auto it = propList->begin(); it != propList->end(); ++it) {
		auto propMeta = mman.getMeta(it->typeName);
		if (propMeta != nullptr) {
			addGUIItem(flayout, propMeta, Entity, Comp, &(*it));
		} else {
			QString val("Unregistred Property! Name: ");
			auto invalidType = new QLabel(val + it->name.c_str() + " Type: " + it->typeName.c_str());
			invalidType->setStyleSheet("QLabel { color : red; }");
			flayout->addRow(invalidType);
		}
	}

	Frame->setLayout(flayout);
}

void ComponentTree::addGUIItem(QFormLayout *Layout, const Kite::KMetaBase *Meta, const Kite::KEntity *Entity,
							   const Kite::KComponent *Comp, const Kite::KMetaProperty *PropMeta) {
	if (Meta->getMetaType() == Kite::KMetaTypes::KMT_POD) {
		// std string
		if (PropMeta->typeName == "std::string") {
			// create property binder
			auto pholder = new KSTR(Layout, Comp->getName().c_str(), Comp->getName().c_str(),
									PropMeta->name.c_str(), PropMeta->typeName.c_str(), Entity->getHandle());
			if (Comp->getClassName() == "KLogicCom") { pholder->setCType("Logic"); }
			connect(pholder, &KSTR::finishEdit, this, &ComponentTree::actEdit);

			bool ronly = false;
			if (PropMeta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;

			// getting def value
			auto defval = Comp->getProperty(PropMeta->name).as<std::string>();

			// non-resource string
			if (PropMeta->resType.empty()) {
				// create gui element and bind to property
				auto line = lineEdit(Layout, PropMeta->name.c_str(), defval.c_str(), ronly);
				connect(line, &QLineEdit::textChanged, pholder, &KSTR::editedStr);

				// resource string
			} else {
				QStringList items;
				auto combo = comboEdit(Layout, PropMeta->name.c_str(), items, true);
				combo->setObjectName(PropMeta->resType.c_str());
				combo->installEventFilter(this);
				eventFilter(combo, &QEvent(QEvent::MouseButtonPress));
				combo->setCurrentText(defval.c_str());
				connect(combo, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
						pholder, &KSTR::editedStr);
			}

			// bool
		} else if (PropMeta->typeName == "bool") {
			// create property binder
			auto pholder = new KBOOL(Layout, Comp->getName().c_str(), Comp->getName().c_str(),
									 PropMeta->name.c_str(), PropMeta->typeName.c_str(), Entity->getHandle());
			if (Comp->getClassName() == "KLogicCom") { pholder->setCType("Logic"); }
			connect(pholder, &KBOOL::finishEdit, this, &ComponentTree::actEdit);

			bool ronly = false;
			if (PropMeta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;

			// getting def value
			auto defval = Comp->getProperty(PropMeta->name).as<bool>();

			// create gui element and bind to property
			auto check = checkEdit(Layout, PropMeta->name.c_str(), defval, ronly);
			connect(check, &QCheckBox::stateChanged, pholder, &KBOOL::editedBool);
		} else if (PropMeta->typeName == "F32") {
			// create property binder
			auto pholder = new KFLT(Layout, Comp->getName().c_str(), Comp->getName().c_str(),
									PropMeta->name.c_str(), PropMeta->typeName.c_str(), Entity->getHandle());
			if (Comp->getClassName() == "KLogicCom") { pholder->setCType("Logic"); }
			connect(pholder, &KFLT::finishEdit, this, &ComponentTree::actEdit);

			bool ronly = false;
			if (PropMeta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;

			// getting def value
			auto defval = Comp->getProperty(PropMeta->name).as<float>();

			// create gui element and bind to property
			auto spin = singleSpin<QDoubleSpinBox>(Layout, PropMeta->name.c_str(), defval, ronly);
			spin->setDecimals(4);
			connect(spin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), pholder, &KFLT::editedFloat);
		}

	} else if (Meta->getFlag() & POD && Meta->getMetaType() == Kite::KMetaTypes::KMT_CLASS) {
		if (PropMeta->typeName == "KVector2F32") {
			// create property binder
			auto pholder = new KV2F32(Layout, Comp->getName().c_str(), Comp->getName().c_str(),
									  PropMeta->name.c_str(), PropMeta->typeName.c_str(), Entity->getHandle());
			if (Comp->getClassName() == "KLogicCom") { pholder->setCType("Logic"); }
			connect(pholder, &KV2F32::finishEdit, this, &ComponentTree::actEdit);

			// read only
			bool ronly = false;
			if (PropMeta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;

			// getting def value
			auto defval = Comp->getProperty(PropMeta->name).as<Kite::KVector2F32>();

			// create gui element and bind to property
			pholder->editedX(defval.x);
			auto spinPair = doubleSpin<QDoubleSpinBox>(Layout, PropMeta->name.c_str(), "X", "Y", defval.x, defval.y, ronly, PropMeta->max, PropMeta->max);
			spinPair.first->setDecimals(4);
			connect(spinPair.first, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), pholder, &KV2F32::editedX);

			pholder->editedY(defval.y);
			spinPair.second->setDecimals(4);
			connect(spinPair.second, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), pholder, &KV2F32::editedY);
		}

	} else {
		QString val("Invalid Property! Name: ");
		auto invalidType = new QLabel(val + PropMeta->name.c_str() + " Type: " + PropMeta->typeName.c_str());
		invalidType->setStyleSheet("QLabel { color : red; }");
		Layout->addRow(invalidType);
	}
}

void ComponentTree::entityEdit(Kite::KEntity *Entity) {
	if (Entity == currEntity) {
		return;
	}
	actClear();

	currEntity = Entity;
	for (auto it = types.begin(); it != types.end(); ++it) {
		if ((*it) == "Logic") {
			// load logic components at the end
			continue;
		} else {
			auto comp = Entity->getComponentByName(it->toStdString(), it->toStdString());
			if (comp != nullptr) {
				createComponent(Entity, comp);
			}
		}
	}

	std::vector<Kite::KComponent *> compList;
	Entity->getScriptComponents(compList);
	for (auto it = compList.begin(); it != compList.end(); ++it) {
		createComponent(Entity, (*it));
	}

	actionsControl(AS_ON_LOAD);
	QString name(Entity->getName().c_str());
	hlabel->setText("Components Editor (" + name + ")");
}

void ComponentTree::entityDelete(Kite::KEntity *Entity) {
	if (Entity == currEntity || Entity == nullptr) {
		actClear();
		actionsControl(AS_ON_INITE);
		currEntity = nullptr;
		hlabel->setText("Components Editor");
	}
}

void ComponentTree::actRClicked(const QPoint & pos) {
	auto obj = (Expander *)sender();
	auto item = itemAt(pos);
	QMenu cmenu(this);

	// checking user rclicked on tree itself or entity??
	if (item != nullptr) {
		remComp->setData(obj->text());
		cmenu.addAction(remComp);
		cmenu.exec(obj->mapToGlobal(pos));
	}
}

void ComponentTree::actAdd(QAction *Action) {
	// logic components need a unique name
	if (Action->text() == "Logic") {
		auto text = getAvailName(currEntity);
		if (text.isEmpty()) {
			return;
		}

		auto chandle = currEntity->addComponent("Logic", text.toStdString());
		createComponent(currEntity, currEntity->getComponent("Logic", chandle));
		emit(componentAdded(currEntity, currEntity->getComponentByName("Logic", text.toStdString())));
		// fixed components
	} else {
		// available name
		auto isExist = currEntity->hasComponentType(Action->text().toStdString());
		if (isExist) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("this component is already exist!");
			msg.exec();
			return;
		}
		auto chandle = currEntity->addComponent(Action->text().toStdString(), Action->text().toStdString());
		createComponent(currEntity, currEntity->getComponent(Action->text().toStdString(), chandle));

		emit(componentAdded(currEntity, currEntity->getComponentByName(Action->text().toStdString(), Action->text().toStdString())));
	}

}

void ComponentTree::actAddDef() {
	auto text = getAvailName(currEntity);
	if (text.isEmpty()) {
		return;
	}

	auto chandle = currEntity->addComponent("Logic", text.toStdString());
	createComponent(currEntity, currEntity->getComponent("Logic", chandle));
	emit(componentAdded(currEntity, currEntity->getComponentByName("Logic", text.toStdString())));
}

void ComponentTree::actRemove() {
	auto act = (QAction *)sender();
	auto ctype = act->data().toString().section("\t", 0, 0);
	auto cname = act->data().toString().section("\t", 1, 1);
	emit(componentDelete(currEntity, currEntity->getComponentByName(ctype.toStdString(), cname.toStdString())));
	currEntity->removeComponent(ctype.toStdString(), cname.toStdString());
	removeComponentGUI();
}

void ComponentTree::actEdit() {
	auto pholder = (ComProperty *)sender();
	auto comp = currEntity->getComponentByName(pholder->getCType().toStdString(), pholder->getCName().toStdString());
	comp->setProperty(pholder->getPName().toStdString(), pholder->getValue());
	emit(componentEdited(currEntity, comp));
}

void ComponentTree::actClear() {
	auto number = topLevelItemCount();
	for (auto i = 0; i < number; ++i) {
		auto item = topLevelItem(0);
		setCurrentItem(item);
		removeComponentGUI();
	}
}

void ComponentTree::actSearch(const QString &Pharase) {
	auto allItems = QTreeWidgetItemIterator(this);
	while (*allItems) {
		if ((*allItems)->text(0).contains(Pharase) && !Pharase.isEmpty()) {
			itemWidget((*allItems), 0)->setStyleSheet("background-color: green;");
		} else {
			itemWidget((*allItems), 0)->setStyleSheet("");
		}
		++allItems;
	}
}

