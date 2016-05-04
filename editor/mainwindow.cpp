#include "mainwindow.h"
#include <QtWidgets>
#include <gridscene.h>
#include <expander.h>
#include <vector>
#include <thread>
#include <string>
#include <climits>
#include "frmnewproj.h"
#include "comproperty.h"

#include <Kite/meta/kmetamanager.h>
#include <Kite/core/kresourcemanager.h>
#include <kmeta.khgen.h>

using namespace Kite;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
	kcurScene(nullptr), curEnt(nullptr),
	curProject(nullptr)
{
    this->setMinimumSize(1000, 600);
	setupStatusBar();
	scanKiteMeta();
	setupActions();
    setupToolbar();
	setupMenus();
    setupDocks();
    setupScene();
	setupResources();
	loadDockState();
	registerResCallbacks();
	disGUI();

}

MainWindow::~MainWindow()
{}

void MainWindow::closeEvent(QCloseEvent *event) {
	//! Ignore the event by default.. otherwise the window will be closed always.
	event->ignore();

	closeProject();
	if (curProject != nullptr) {
		return;
	}

	saveDockState();
	event->accept();
}

void MainWindow::entityChecked(QTreeWidgetItem *Item, int Col) {
	if (kcurScene != nullptr) {
		auto eman = kcurScene->getEManager();
		auto entity = eman->getEntityByName(Item->text(0).toStdString());
		if (Item->checkState(0) == Qt::Checked) {
			entity->setActive(true);
		} else {
			entity->setActive(false);
		}
	}
}

void MainWindow::entityClicked() {
	componentClear();
	curEnt = objTree->currentItem();
	if (kcurScene != nullptr) {
		auto eman = kcurScene->getEManager();
		auto entity = eman->getEntityByName(curEnt->text(0).toStdString());
		loadComponents(entity->getHandle());
	}
}

void MainWindow::entityRClicked(const QPoint & pos) {
	curEnt = objTree->itemAt(pos);
	QMenu cmenu(this);

	// checking user rclicked on tree itself or entity??
	if (curEnt == nullptr) {
		QPoint pt(pos);
		addObj->setText("Add New Entity");
		cmenu.addAction(addObj);
		cmenu.exec(objTree->mapToGlobal(pos));
	} else {
		addObj->setText("Add New Child");
		cmenu.addMenu(compMenu);
		cmenu.addSeparator();
		cmenu.addAction(addObj);
		cmenu.addSeparator();
		cmenu.addAction(remObj);
		cmenu.addAction(renObj);
		cmenu.exec(objTree->mapToGlobal(pos));
	}
}

void MainWindow::entityAdd() {
	if (kcurScene != nullptr) {
		auto eman = kcurScene->getEManager();
		bool ok = false;
		QString text;
		do {
			text = QInputDialog::getText(this, tr("New Entity"),
										 tr("Entity name:"), QLineEdit::Normal,
										 "", &ok);

			// cancel pressed
			if (!ok) {
				return;
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

		auto ent = eman->createEntity(text.toStdString());

		// root
		if (curEnt == nullptr) {
			auto node = new QTreeWidgetItem(objTree);
			node->setText(0, text);
			node->setFlags(node->flags() | Qt::ItemIsUserCheckable);
			node->setCheckState(0, Qt::Checked);
			node->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
			objTree->setCurrentItem(node);

		// child
		} else {
			auto parrent = eman->getEntityByName(curEnt->text(0).toStdString());
			parrent->addChild(ent);
			auto node = new QTreeWidgetItem(curEnt);
			node->setText(0, text);
			node->setFlags(node->flags() | Qt::ItemIsUserCheckable);
			node->setCheckState(0, Qt::Checked);
			node->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
			curEnt->setExpanded(true);
			objTree->setCurrentItem(node);
		}

	} else {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("There is no current scene.");
		msg.exec();
	}
}

void MainWindow::entityRemove() {
	if (kcurScene != nullptr) {
		auto eman = kcurScene->getEManager();

		if (curEnt != nullptr) {
			eman->removeEntityByName(curEnt->text(0).toStdString());
			eman->postWork();
			delete curEnt;
			curEnt = nullptr;
		} else {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("Select an entity first.");
			msg.exec();
		}
	} else {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("There is no current scene.");
		msg.exec();
	}
}

void MainWindow::entityRename() {
	if (kcurScene != nullptr) {
		auto eman = kcurScene->getEManager();

		if (curEnt != nullptr) {
			bool ok = false;
			QString text;
			do {
				text = QInputDialog::getText(this, tr("New Entity"),
											 tr("Entity name:"), QLineEdit::Normal,
											 "", &ok);

				// cancel pressed
				if (!ok) {
					return;
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

			auto ent = eman->getEntityByName(curEnt->text(0).toStdString());
			eman->renameEntity(ent->getHandle(), text.toStdString());
			curEnt->setText(0, text);
		} else {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("Select an entity first.");
			msg.exec();
		}
	} else {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("There is no current scene.");
		msg.exec();
	}
}


void MainWindow::componentClicked() {
	QMessageBox msg;
	msg.setWindowTitle("Message");
	msg.setText("this name is already exist!");
	msg.exec();
}

void MainWindow::componentRClicked(const QPoint & pos) {
	auto obj = (Expander *)sender();
	auto item = propTree->itemAt(pos);
	QMenu cmenu(this);

	// checking user rclicked on tree itself or entity??
	if (item != nullptr) {
		remCom->setData(obj->text());
		cmenu.addAction(remCom);
		cmenu.exec(obj->mapToGlobal(pos));
	}
}

void MainWindow::componentAdd(QAction *Action) {
	if (kcurScene != nullptr) {
		auto eman = kcurScene->getEManager();
		auto entity = eman->getEntityByName(curEnt->text(0).toStdString());

		// logic components need a unique name
		if (Action->text() == "Logic") {
			QString text;
			bool ok;
			do {
				text = QInputDialog::getText(this, tr("New Logic Component"),
											 tr("Component name:"), QLineEdit::Normal,
											 "", &ok);
				// cancel pressed
				if (!ok) {
					return;
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
				auto isExist = entity->hasComponent("Logic", text.toStdString());
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
			auto chandle = entity->addComponent("Logic", text.toStdString());
			createComponent(entity, entity->getComponent("Logic", chandle));

		// fixed components
		} else {
			// available name
			auto isExist = entity->hasComponentType(Action->text().toStdString());
			if (isExist) {
				QMessageBox msg;
				msg.setWindowTitle("Message");
				msg.setText("this component is already exist!");
				msg.exec();
				return;
			}
			auto chandle = entity->addComponent(Action->text().toStdString(), Action->text().toStdString());
			createComponent(entity, entity->getComponent(Action->text().toStdString(), chandle));
		}
	} else {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("There is no current scene.");
		msg.exec();
	}
}

void MainWindow::componentEdited() {
	auto pholder = (ComProperty *)sender();

	auto eman = kcurScene->getEManager();
	auto entity = eman->getEntity(pholder->getEntity());
	if (entity != nullptr) {
		auto comp = entity->getComponentByName(pholder->getCType().toStdString(), pholder->getCName().toStdString());
		comp->setProperty(pholder->getPName().toStdString(), pholder->getValue());
	} else {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("There is no entity for this component.");
		msg.exec();
	}
}

void MainWindow::componentRemove() {
	auto act = (QAction *)sender();
	auto ctype = act->data().toString().section("\t",0 , 0);
	auto cname = act->data().toString().section("\t", 1, 1);
	if (kcurScene != nullptr) {
		auto eman = kcurScene->getEManager();
		auto entity = eman->getEntityByName(curEnt->text(0).toStdString());
		entity->removeComponent(ctype.toStdString(), cname.toStdString());
	}
	removeComponentGUI();
}

void MainWindow::componentClear() {
	auto number = propTree->topLevelItemCount();
	for (auto i = 0; i < number; ++i) {
		auto item = propTree->topLevelItem(0);
		propTree->setCurrentItem(item);
		removeComponentGUI();
	}
	//propTree->clear();
}

void MainWindow::exitApp() {
	closeProject();
	if (curProject == nullptr) {
		this->close();
	}
}

void MainWindow::setupDocks(){
	QMainWindow::statusBar()->showMessage("Initializing GUI (Docks) ...");

    // resource dock
    resDock = new QDockWidget(tr("Resource Explorer"), this);
	resDock->setObjectName("Resources");
    resDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    resDock->setMinimumWidth(120);
    addDockWidget(Qt::LeftDockWidgetArea, resDock);

	resTree = new ResourceTree(this);
	resTree->setupCategories(kresCatList);
	resDock->setWidget(resTree);
	resDock->setTitleBarWidget(resTree->getHeaderTools());

    // objects dock
    objDock = new QDockWidget(tr("Hierarchy"), this);
	objDock->setObjectName("Hierarchy");
    objDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    objDock->setMinimumWidth(120);
    addDockWidget(Qt::LeftDockWidgetArea, objDock);

	objTree = new ObjectTree(this);
	objDock->setWidget(objTree);
	objDock->setTitleBarWidget(objTree->getHeaderTools());

    // component/properties dock
    prpDock = new QDockWidget(tr("Component Editor"), this);
	prpDock->setObjectName("Components");
    prpDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    prpDock->setMinimumWidth(120);
    addDockWidget(Qt::RightDockWidgetArea, prpDock);

	propTree = new QTreeWidget;
	propTree->setHeaderLabel("Components");
	propTree->setHeaderHidden(true);
	propTree->setSelectionMode(QAbstractItemView::SingleSelection);
	propTree->setRootIsDecorated(false);
	propTree->setIndentation(0);
	propTree->setAnimated(true);
	prpDock->setWidget(propTree);

	QMainWindow::statusBar()->showMessage("Ready");
}

void MainWindow::setupScene(){
    sceneView = new QGraphicsView();
	setCentralWidget(sceneView);
	
	GridScene *scene1 = new GridScene(25, 20, 32, 32);
	QPixmap plogo("F:\\MyImage\\Kite\\Logo-2-tr.png");
	QGraphicsPixmapItem *logo = new QGraphicsPixmapItem(plogo);
	logo->setPos((scene1->width() / 2) - (plogo.width() / 2), (scene1->height() / 2) - (plogo.height() / 2));
	scene1->addItem(logo);
	sceneView->setScene(scene1);
}

void MainWindow::setupActions() {
	QMainWindow::statusBar()->showMessage("setup actions ...");

	newProj = new QAction(QIcon(":/icons/new"), "New Project", this);
	connect(newProj, &QAction::triggered, this, &MainWindow::newProject);

	openProj = new QAction(QIcon(":/icons/open"), "Open Project", this);
	connect(openProj, &QAction::triggered, this, &MainWindow::openProject);

	saveProj = new QAction(QIcon(":/icons/save"), "Save Project", this);
	connect(saveProj, &QAction::triggered, this, &MainWindow::saveProject);

	closeProj = new QAction("Close Project", this);
	connect(closeProj, &QAction::triggered, this, &MainWindow::closeProject);

	playScene = new QAction(QIcon(":/icons/play"), "Play", this);

	addObj = new QAction(QIcon(":/icons/add"), "Add New Entity", this);
	connect(addObj, &QAction::triggered, this, &MainWindow::entityAdd);

	remObj = new QAction(QIcon(":/icons/remove"), "Remove", this);
	connect(remObj, &QAction::triggered, this, &MainWindow::entityRemove);

	renObj = new QAction(QIcon(":/icons/edit"), "Rename", this);
	connect(renObj, &QAction::triggered, this, &MainWindow::entityRename);

	remCom = new QAction(QIcon(":/icons/remove"), "Remove", this);
	connect(remCom, &QAction::triggered, this, &MainWindow::componentRemove);

	exit = new QAction(QIcon(":/icons/exit"), "Exit", this);
	connect(exit, &QAction::triggered, this, &MainWindow::exitApp);

	QMainWindow::statusBar()->showMessage("Ready");
}

void MainWindow::setupMenus(){
	// menu bar
	menuBar()->setStyleSheet("QMenu::separator {\n"
							 "height: 1px;\n"
							 "background: rgb(129, 129, 129);\n"
							 "margin-left: 25px;}");

	// file
    fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newProj);
	fileMenu->addAction(openProj);
	fileMenu->addSeparator();
	fileMenu->addAction(closeProj);
	fileMenu->addSeparator();
	fileMenu->addAction(saveProj);
	fileMenu->addSeparator();
	fileMenu->addAction(exit);

	// window
	winMenu = menuBar()->addMenu(tr("&Window"));

	// components
	compMenu = new QMenu(this);
	compMenu->setTitle("Add New Component...");
	connect(compMenu, &QMenu::triggered, this, &MainWindow::componentAdd);
	for (auto it = kcompList.begin(); it != kcompList.end(); ++it) {
		compMenu->addAction((*it))->setIcon(QIcon(":/icons/com"));
	}
}

void MainWindow::run() {

}

void MainWindow::setupToolbar(){
    fileTolb = addToolBar(tr("Toolbar"));
	fileTolb->setObjectName("Toolbar");
	fileTolb->addAction(newProj);
	fileTolb->addAction(openProj);
	fileTolb->addSeparator();
	fileTolb->addAction(saveProj);
	fileTolb->addSeparator();
	fileTolb->addAction(playScene);
}

void MainWindow::setupStatusBar() {
	QMainWindow::statusBar()->showMessage("Ready");
}

void MainWindow::loadDockState() {
	QSettings settings;
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray());
}

void MainWindow::saveDockState() {
	QSettings settings;
	settings.setValue("geometry", saveGeometry());
	settings.setValue("state", saveState());
}

void MainWindow::disGUI() {
	playScene->setDisabled(true);
	saveProj->setDisabled(true);
	closeProj->setDisabled(true);
	playScene->setDisabled(true);
	addObj->setDisabled(true);
	remObj->setDisabled(true);
	renObj->setDisabled(true);
}

void MainWindow::enGUI() {
	playScene->setDisabled(false);
	saveProj->setDisabled(false);
	closeProj->setDisabled(false);
	playScene->setDisabled(false);
	addObj->setDisabled(false);
	remObj->setDisabled(false);
	renObj->setDisabled(false);
}

void MainWindow::registerResCallbacks() {
	ResourceCallbacks sceneCallb;
	sceneCallb.editCallback = &MainWindow::editScene;
	sceneCallb.removeCallb = &MainWindow::removeScene;
	kresCallbackMap.insert("KScene", sceneCallb);
}

void MainWindow::saveXML(QIODevice *device){
	QXmlStreamWriter stream(device);
	stream.setAutoFormatting(true);
	stream.writeStartDocument();
	stream.writeStartElement("bookmark");
	stream.writeAttribute("href", "http://qt-project.org/");
	stream.writeTextElement("title", "Qt Project");
	stream.writeEndElement(); // bookmark
	stream.writeEndDocument();
}

void MainWindow::loadXML(QIODevice *device) {
	QXmlStreamReader xml(device);
	while (!xml.atEnd()) {
		xml.readNext();
		// do processing
	}
	if (xml.hasError()) {
		// do error handling
	}
}

void MainWindow::scanKiteMeta() {
	QMainWindow::statusBar()->showMessage("Scaning Kite2D ...");

	// register kiet meta system
	kmman = new Kite::KMetaManager();
	Kite::registerKiteMeta(kmman);

	// register resource types
	krman = new Kite::KResourceManager();
	Kite::registerRTypes(krman);

	// dump all mete information
	std::vector<const KMetaBase *> meta;
	kmman->dump(meta);

	// searching for resource categories
	kresCatList.clear();
	kcompList.clear();
	for (auto it = meta.begin(); it != meta.end(); ++it) {
		if (((*it)->getFlag() & RESOURCE) && !((*it)->getFlag() & ABSTRACT)) {
			kresCatList.push_back((*it)->getName().c_str());
			continue;
		}

		if (((*it)->getFlag() & COMPONENT) && !((*it)->getFlag() & ABSTRACT)) {
			auto infoList = (*it)->getInfo();
			for (auto ilit = infoList->begin(); ilit != infoList->end(); ++ilit) {
				if (ilit->first == "KI_CTYPE") {
					kcompList.push_back(ilit->second.c_str());
				}
			}
			continue;
		}

	}

	QMainWindow::statusBar()->showMessage("Ready");
}

void MainWindow::setupResources() {
	/*resTree->clear();
	kresMap.clear();

	for (auto it = kresCatList.begin(); it != kresCatList.end(); ++it) {
		auto category = new QTreeWidgetItem(resTree);
		category->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
		category->setText(0, (*it));
		category->setIcon(0, this->style()->standardIcon(QStyle::SP_DirOpenIcon));
		kresMap.insert((*it), QHash<QString, Kite::KResource *>());
	}*/
}

void MainWindow::newProject() {

	// close current project if any
	closeProject();

	if (curProject == nullptr) {
		frmNewProj frm(this);
		frm.exec();

		if (frm.isOk()) {
			curProject = new Project;
			curProject->name = frm.getName();
			curProject->Path = frm.getPath() + "/" + curProject->name;
			curProject->resPath = curProject->Path  + "/resources";

			this->setWindowTitle("Kite2D Editor - " + frm.getName());

			// reset resources
			setupResources();

			enGUI();
		}
	}
}

void MainWindow::openProject() {
	// close current project if any
	closeProject();

	if (curProject == nullptr) {
		QString fileName = QFileDialog::getOpenFileName(this,
												"Open Project", "", "Kite2D Project File (*.k2d)");
	}
}

void MainWindow::saveProject() {
	if (curProject != nullptr) {
		QFile file(curProject->Path + "/" + curProject->name + ".k2d");
		if (file.open(QIODevice::WriteOnly)) {
			saveXML(&file);
			file.close();
		}
	}
}

void MainWindow::closeProject() {
	if (curProject != nullptr) {
		int ret = QMessageBox::warning(this, "Kite2D Editor",
									   "Do you want to save changes to " + curProject->name + "?",
									   QMessageBox::Save | QMessageBox::Discard
									   | QMessageBox::Cancel,
									   QMessageBox::Save);

		if (ret == QMessageBox::Save) {
			saveProject();
		} else if (ret == QMessageBox::Cancel) {
			return;
		}

		this->setWindowTitle("Kite2D Editor");

		// clear resource tree
		for (auto it = kresMap.begin(); it != kresMap.end(); ++it) {
			auto callb = kresCallbackMap.find(it.key());
			if (callb != kresCallbackMap.end()) {
				if (callb->removeCallb != nullptr) {
					for (auto rit = it->begin(); rit != it->end(); ++rit) {
						(this->*callb->removeCallb)((*rit));
					}
				}
			}
		}
		setupResources();

		delete curProject;
		curProject = nullptr;

		disGUI();
	}
}

void MainWindow::loadChilds(Kite::KEntityManager *Eman, const Kite::KHandle &Entity, QTreeWidgetItem *Parrent) {
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

void MainWindow::loadComponents(Kite::KHandle EHandle) {
	if (kcurScene != nullptr) {
		auto eman = kcurScene->getEManager();
		auto entity = eman->getEntity(EHandle);

		if (entity == nullptr) {
			return;
		}

		for (auto it = kcompList.begin(); it != kcompList.end(); ++it) {
			if ((*it) == "Logic") {
				// load logic components at the end
				continue;
			} else {
				auto comp = entity->getComponentByName(it->toStdString(), it->toStdString());
				if (comp != nullptr) {
					createComponent(entity, comp);
				}
			}
		}

		std::vector<Kite::KComponent *> compList;
		entity->getScriptComponents(compList);
		for (auto it = compList.begin(); it != compList.end(); ++it) {
			createComponent(entity, (*it));
		}
	}
}

void MainWindow::removeComponentGUI() {
	auto item = propTree->currentItem();
	auto child = item->child(0);
	auto expander = (Expander *)propTree->itemWidget(item, 0);
	auto cframe = (QFrame *) propTree->itemWidget(child, 0);
	delete child;
	delete item;
	delete expander;
	delete cframe;
}

void MainWindow::createComponent(const Kite::KEntity *Entity, const Kite::KComponent *Comp) {
	// header
	auto category = new QTreeWidgetItem(propTree);
	QString name;
	if (Comp->getClassName() == "KLogicCom") {
		QString lname(Comp->getName().c_str());
		name = "Logic\t" + lname;
	} else {
		name = Comp->getName().c_str();
		name.append("\t");
	}
	auto btnExpander = new Expander(name, QIcon(":/icons/com"), propTree, category);
	btnExpander->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	btnExpander->setAutoRaise(true);
	btnExpander->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(btnExpander, &Expander::customContextMenuRequested, this, &MainWindow::componentRClicked);
	propTree->setItemWidget(category, 0, btnExpander);

	// contetnts
	auto cframe = new QFrame(propTree);
	bindProperties(Entity, Comp, cframe);

	auto pContainer = new QTreeWidgetItem(category);
	pContainer->setDisabled(true);
	propTree->setItemWidget(pContainer, 0, cframe);
}

void MainWindow::bindProperties(const Kite::KEntity *Entity, const Kite::KComponent *Comp, QFrame *Frame) {

	auto compMeta = (Kite::KMetaClass *)kmman->getMeta(Comp->getClassName().c_str());
	auto propList = compMeta->getProperties();
	auto flayout = new QFormLayout();

	flayout->setContentsMargins(10, 0, 10, 0);
	flayout->setHorizontalSpacing(5);
	flayout->setVerticalSpacing(3);
	for (auto it = propList->begin(); it != propList->end(); ++it) {
		auto propMeta = kmman->getMeta(it->typeName);
		if (propMeta != nullptr) {
			addGUIItem(flayout, propMeta, Entity, Comp, &(*it));
		} else {
			QString val("Unregistred Property! Name: ");
			auto invalidType = new QLabel(val + it->name.c_str() + "Type: " + it->typeName.c_str());
			invalidType->setStyleSheet("QLabel { color : red; }");
			flayout->addRow(invalidType);
		}
	}

	Frame->setLayout(flayout);
}

void MainWindow::addGUIItem(QFormLayout *Layout, const Kite::KMetaBase *Meta, const Kite::KEntity *Entity,
							const Kite::KComponent *Comp, const Kite::KMetaProperty *PropMeta) {
	if (Meta->getMetaType() == KMetaTypes::KMT_POD ) {
		// std string
		if (PropMeta->typeName == "std::string") {
			// create property binder
			auto pholder = new KSTR(Layout, Comp->getName().c_str(), Comp->getName().c_str(),
									PropMeta->name.c_str(), PropMeta->typeName.c_str(), Entity->getHandle());
			if (Comp->getClassName() == "KLogicCom") { pholder->setCType("Logic");}
			connect(pholder, &KSTR::finishEdit, this, &MainWindow::componentEdited);

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
				auto resCat = kresMap.find(PropMeta->resType.c_str());
				// getting all available resources with the given category
				if (resCat != kresMap.end()) {
					QStringList items;
					for (auto it = resCat->begin(); it != resCat->end(); ++it) {
						items.push_back(it.key());
					}
					items.push_back("--EMPTY--");
					auto combo = comboEdit(Layout, PropMeta->name.c_str(), items, true);
					combo->setObjectName(PropMeta->resType.c_str());
					combo->setCurrentText(defval.c_str());
					if (combo->currentText() != defval.c_str()) {
						combo->setCurrentText("--EMPTY--");
					}
					
					connect(combo, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
							pholder, &KSTR::editedStr);
				}
			}

		// bool
		} else if (PropMeta->typeName == "bool") {
			// create property binder
			auto pholder = new KBOOL(Layout, Comp->getName().c_str(), Comp->getName().c_str(),
									PropMeta->name.c_str(), PropMeta->typeName.c_str(), Entity->getHandle());
			if (Comp->getClassName() == "KLogicCom") { pholder->setCType("Logic"); }
			connect(pholder, &KBOOL::finishEdit, this, &MainWindow::componentEdited);

			bool ronly = false;
			if (PropMeta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;

			// getting def value
			auto defval = Comp->getProperty(PropMeta->name).as<bool>();

			// create gui element and bind to property
			auto check = checkEdit(Layout, PropMeta->name.c_str(), defval,ronly);
			connect(check, &QCheckBox::stateChanged, pholder, &KBOOL::editedBool);
		} else if (PropMeta->typeName == "F32") {
			// create property binder
			auto pholder = new KFLT(Layout, Comp->getName().c_str(), Comp->getName().c_str(),
									  PropMeta->name.c_str(), PropMeta->typeName.c_str(), Entity->getHandle());
			if (Comp->getClassName() == "KLogicCom") { pholder->setCType("Logic"); }
			connect(pholder, &KFLT::finishEdit, this, &MainWindow::componentEdited);

			bool ronly = false;
			if (PropMeta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;

			// getting def value
			auto defval = Comp->getProperty(PropMeta->name).as<float>();

			// create gui element and bind to property
			auto spin = singleSpin<QDoubleSpinBox>(Layout, PropMeta->name.c_str(), defval, ronly);
			connect(spin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), pholder, &KFLT::editedFloat);
		}

	} else if (Meta->getFlag() & POD && Meta->getMetaType() == KMetaTypes::KMT_CLASS) {
		if (PropMeta->typeName == "KVector2F32") {
			// create property binder
			auto pholder = new KV2F32(Layout, Comp->getName().c_str(), Comp->getName().c_str(),
									  PropMeta->name.c_str(), PropMeta->typeName.c_str(), Entity->getHandle());
			if (Comp->getClassName() == "KLogicCom") { pholder->setCType("Logic");}
			connect(pholder, &KV2F32::finishEdit, this, &MainWindow::componentEdited);

			// read only
			bool ronly = false;
			if (PropMeta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;

			// getting def value
			auto defval = Comp->getProperty(PropMeta->name).as<Kite::KVector2F32>();

			// create gui element and bind to property
			pholder->editedX(defval.x);
			auto spinPair = doubleSpin<QDoubleSpinBox>(Layout, PropMeta->name.c_str(), "X", "Y", defval.x, defval.y, ronly, PropMeta->max, PropMeta->max);
			connect(spinPair.first, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged) , pholder, &KV2F32::editedX);

			pholder->editedY(defval.y);	
			connect(spinPair.second, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), pholder, &KV2F32::editedY);
		}

	}else{
		QString val("Invalid Property! Name: ");
		auto invalidType = new QLabel(val + PropMeta->name.c_str() + " Type: " + PropMeta->typeName.c_str());
		invalidType->setStyleSheet("QLabel { color : red; }");
		Layout->addRow(invalidType);
	}
}

void MainWindow::editScene(Kite::KResource *Scene) {
	QMainWindow::statusBar()->showMessage("Loading scene ...");
	objTree->clear();

	auto scene = (Kite::KScene *)Scene;
	auto eman = scene->getEManager();
	auto root = eman->getEntity(eman->getRoot());

	// loading tree recursively
	for (auto it = root->beginChild(); it != root->endChild(); ++it) {
		auto child = eman->getEntity((*it));
		auto node = new QTreeWidgetItem(objTree);
		node->setFlags(node->flags() | Qt::ItemIsUserCheckable);
		if (child->getActive()) {
			node->setCheckState(0, Qt::Checked);
		} else {
			node->setCheckState(0, Qt::Unchecked);
		}
		node->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
		node->setText(0, child->getName().c_str());
		loadChilds(eman, (*it), node);
	}

	// set current scene
	kcurScene = scene;

	QMainWindow::statusBar()->showMessage("Ready");
}

void MainWindow::removeScene(Kite::KResource *Scene) {
	auto scene = (Kite::KScene *)Scene;
	if (kcurScene == scene) {
		objTree->clear();
		componentClear();
		kcurScene = nullptr;
	}

	delete scene;
}
