#include "mainwindow.h"
#include <QtWidgets>
#include <gridscene.h>
#include <expander.h>
#include <vector>
#include <thread>
#include <string>
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
	saveDockState();
	QMainWindow::closeEvent(event);
}

void MainWindow::resourceClicked() {

}

void MainWindow::resourceRClicked(const QPoint & pos) {
	QTreeWidgetItem *item = resTree->itemAt(pos);
	QMenu cmenu(this);
	
	// checking user rclicked on category or items??
	if (item != nullptr) {
		auto found = kresMap.find(item);
		if (found != kresMap.end()) {
			QPoint pt(pos);
			addRes->setText("Add New " + item->text(0));
			openRes->setText("Add Existing " + item->text(0));
			cmenu.addAction(addRes);
			cmenu.addAction(openRes);
			cmenu.exec(resTree->mapToGlobal(pos));
		} else {
			QPoint pt(pos);
			editRes->setText("Edit " + item->text(0));
			remRes->setText("Remove " + item->text(0));
			saveRes->setText("Save " + item->text(0));
			cmenu.addAction(editRes);
			cmenu.addSeparator();
			cmenu.addAction(remRes);
			cmenu.addSeparator();
			cmenu.addAction(saveRes);
			cmenu.exec(resTree->mapToGlobal(pos));
		}
	}
}

void MainWindow::resourceAdd() {
	auto parrent = resTree->currentItem();
	auto restype = parrent->text(0);

	// create new resource and add it to res tree
	auto resmap = kresMap.find(parrent);

	bool ok = false;
	QString text;
	do {
		text = QInputDialog::getText(this, "New " + restype,
										restype + " name:", QLineEdit::Normal,
										"", &ok);

		// cancel pressed
		if (!ok) {
			return;
		}

		// empty name
		if (text.isEmpty()) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("resource name is empty!");
			msg.exec();
			continue;
		}

		// available name
		auto avail = resmap->find(text);
		if (avail != resmap->end()) {
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

	auto newres = krman->create(restype.toStdString(), text.toStdString());
	if (newres != nullptr) {
		resmap->insert(text, newres);

		// register ctypes
		if (restype == "KScene") {
			auto scene = (KScene *)newres;
			Kite::registerCTypes(scene->getEManager());
		}

		auto item = new QTreeWidgetItem(parrent);
		item->setText(0, text);
		parrent->setExpanded(true);
	} else {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("resource add module return nullptr!\nresource type: " + restype);
		msg.exec();
	}
}

void MainWindow::resourceOpen() {
	auto parrent = resTree->currentItem();
	auto restype = parrent->text(0);

	QString fileName = QFileDialog::getOpenFileName(this,
													"Open " + restype, "", "Kite2D Resource File (*.kres)");

	// chck name is available
	if (!fileName.isEmpty()) {
		QFileInfo finfo(fileName);
		auto resmap = kresMap.find(parrent);
		auto avail = resmap->find(finfo.baseName());
		if (avail != resmap->end()) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("this name is already exist!");
			msg.exec();
			return;
		}

		// cretae new resource 
		auto newres = krman->create(restype.toStdString(), finfo.baseName().toStdString());
		if (newres != nullptr) {
			// load resource
			Kite::KFIStream istream;
			if (!istream.open(fileName.toStdString(), Kite::KIOTypes::KRT_BIN)) {
				QMessageBox msg;
				msg.setWindowTitle("Message");
				msg.setText("cant open file stream with the given address.\nfile address: " + fileName);
				msg.exec();
				return;
			}

			if (!newres->loadStream(&istream)) {
				QMessageBox msg;
				msg.setWindowTitle("Message");
				msg.setText("cant load resource data from file.\nfile address: " + fileName);
				msg.exec();

				delete newres;
				return;
			}

			resmap->insert(finfo.baseName(), newres);
			auto item = new QTreeWidgetItem(parrent);
			item->setText(0, finfo.baseName());
			parrent->setExpanded(true);
		} else {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("resource add module return nullptr!\nresource type: " + restype);
			msg.exec();
		}
	}
}

void MainWindow::resourceSave() {
	auto item = resTree->currentItem();
	auto resmap = kresMap.find(item->parent());
	
	QString fileName = QFileDialog::getSaveFileName(this, "Save " + item->text(0),
													"", "Kite2D Resource File (*.kres)");

	if (!fileName.isEmpty()) {
		auto res = (*resmap->find(item->text(0)));
		Kite::KFOStream ostream;
		if (!ostream.open(fileName.toStdString(), Kite::KIOTypes::KRT_BIN)) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("cant open file stream with the given address.\nfile address: " + fileName);
			msg.exec();
			return;
		}

		res->saveStream(&ostream);
	}
}

void MainWindow::resourceEdit() {
	auto item = resTree->currentItem();
	auto restype = item->parent()->text(0);

	// checking edit callback
	auto callb = kresCallbackMap.find(restype);
	if (callb == kresCallbackMap.end()) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("Edit module for this type does not exist.\nResource type: " + restype);
		msg.exec();
		return;
	}

	if (callb->editCallback == nullptr) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("Edit module for this type does not exist.\nResource type: " + restype);
		msg.exec();
		return;
	}

	// retrieve resource pointer from map and pass it to its edit function
	auto resmap = kresMap.find(item->parent());
	(this->*callb->editCallback)((*resmap->find(item->text(0))));
}

void MainWindow::resourceRemove() {
	auto item = resTree->currentItem();
	auto restype = item->parent()->text(0);

	// checking remove callback
	auto callb = kresCallbackMap.find(restype);
	if (callb == kresCallbackMap.end()) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("Remove module for this type does not exist.\nResource type: " + restype);
		msg.exec();
		return;
	}

	if (callb->removeCallb == nullptr) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("Remove module for this type does not exist.\nResource type: " + restype);
		msg.exec();
		return;
	}

	// retrieve resource pointer from map and pass it to its remove function
	auto resmap = kresMap.find(item->parent());
	(this->*callb->removeCallb)((*resmap->find(item->text(0))));
	resmap->erase(resmap->find(item->text(0)));
	delete item;
}

void MainWindow::entityClicked() {

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
			node->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
			objTree->setCurrentItem(node);

		// child
		} else {
			auto parrent = eman->getEntityByName(curEnt->text(0).toStdString());
			parrent->addChild(ent);
			auto node = new QTreeWidgetItem(curEnt);
			node->setText(0, text);
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
			entity->addComponent("Logic", text.toStdString());
			createComponent(entity->getHandle(), text, "Logic");

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
			entity->addComponent(Action->text().toStdString(), Action->text().toStdString());
			createComponent(entity->getHandle(), Action->text(), Action->text());
		}
	} else {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("There is no current scene.");
		msg.exec();
	}
}

void MainWindow::componentRemove() {
	auto item = propTree->currentItem();
	auto child = item->child(0);
	auto expander = propTree->itemWidget(item, 0);
	auto cframe = propTree->itemWidget(child, 0);
	delete child;
	delete item;
	delete expander;
	delete cframe;
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
    resDock = new QDockWidget(tr("Resources"), this);
	resDock->setObjectName("Resources");
    resDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    resDock->setMinimumWidth(120);
    addDockWidget(Qt::LeftDockWidgetArea, resDock);

	/*QStringList headers;
	headers.clear();
	headers << "Name";*/
	resTree = new QTreeWidget;
	resTree->setColumnCount(1);
	resTree->setHeaderHidden(true);
	resTree->setSelectionMode(QAbstractItemView::SingleSelection);
	resTree->setContextMenuPolicy(Qt::CustomContextMenu);
	resDock->setWidget(resTree);
	connect(resTree, &QTreeWidget::itemSelectionChanged, this, &MainWindow::resourceClicked);
	connect(resTree, &QTreeWidget::customContextMenuRequested, this, &MainWindow::resourceRClicked);

    // objects dock
    objDock = new QDockWidget(tr("Objects"), this);
	objDock->setObjectName("Objects");
    objDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    objDock->setMinimumWidth(120);
    addDockWidget(Qt::LeftDockWidgetArea, objDock);

	objTree = new QTreeWidget;
	objTree->setColumnCount(1);
	objTree->setHeaderHidden(true);
	objTree->setContextMenuPolicy(Qt::CustomContextMenu);
	objTree->setSelectionMode(QAbstractItemView::SingleSelection);

	objDock->setWidget(objTree);
	connect(objTree, &QTreeWidget::itemSelectionChanged, this, &MainWindow::entityClicked);
	connect(objTree, &QTreeWidget::customContextMenuRequested, this, &MainWindow::entityRClicked);

    // component/properties dock
    prpDock = new QDockWidget(tr("Components"), this);
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
	
	GridScene *scene1 = new GridScene(25, 15, 32, 32);
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

	closeProj = new QAction(QIcon(":/icons/save"), "Close Project", this);
	connect(closeProj, &QAction::triggered, this, &MainWindow::closeProject);

	playScene = new QAction(QIcon(":/icons/play"), "Play", this);

	addRes = new QAction(QIcon(":/icons/save"), "Add New Resource", this);
	connect(addRes, &QAction::triggered, this, &MainWindow::resourceAdd);

	openRes = new QAction(QIcon(":/icons/save"), "Add Existing Resource", this);
	connect(openRes, &QAction::triggered, this, &MainWindow::resourceOpen);

	saveRes = new QAction(QIcon(":/icons/save"), "Save Resource", this);
	connect(saveRes, &QAction::triggered, this, &MainWindow::resourceSave);

	editRes = new QAction(QIcon(":/icons/save"), "Edit", this);
	connect(editRes, &QAction::triggered, this, &MainWindow::resourceEdit);

	remRes = new QAction(QIcon(":/icons/save"), "Remove", this);
	connect(remRes, &QAction::triggered, this, &MainWindow::resourceRemove);

	addObj = new QAction(QIcon(":/icons/save"), "Add New Entity", this);
	connect(addObj, &QAction::triggered, this, &MainWindow::entityAdd);

	remObj = new QAction(QIcon(":/icons/save"), "Remove", this);
	connect(remObj, &QAction::triggered, this, &MainWindow::entityRemove);

	renObj = new QAction(QIcon(":/icons/save"), "Rename", this);
	connect(renObj, &QAction::triggered, this, &MainWindow::entityRename);

	remCom = new QAction(QIcon(":/icons/save"), "Remove", this);
	connect(remCom, &QAction::triggered, this, &MainWindow::componentRemove);

	exit = new QAction(this->style()->standardIcon(QStyle::SP_DockWidgetCloseButton), "Exit", this);
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
		compMenu->addAction((*it));
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
	addRes->setDisabled(true);
	openRes->setDisabled(true);
	editRes->setDisabled(true);
	remRes->setDisabled(true);
	addObj->setDisabled(true);
	remObj->setDisabled(true);
	renObj->setDisabled(true);
}

void MainWindow::enGUI() {
	playScene->setDisabled(false);
	saveProj->setDisabled(false);
	closeProj->setDisabled(false);
	playScene->setDisabled(false);
	addRes->setDisabled(false);
	openRes->setDisabled(false);
	editRes->setDisabled(false);
	remRes->setDisabled(false);
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
				if (ilit->first == "CType") {
					kcompNameMap.insert(ilit->second.c_str(), (*it)->getName().c_str());
					kcompList.push_back(ilit->second.c_str());
				}
			}
			continue;
		}

	}

	QMainWindow::statusBar()->showMessage("Ready");
}

void MainWindow::setupResources() {
	resTree->clear();
	kresMap.clear();

	for (auto it = kresCatList.begin(); it != kresCatList.end(); ++it) {
		auto category = new QTreeWidgetItem(resTree);
		category->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
		category->setText(0, (*it));
		category->setIcon(0, this->style()->standardIcon(QStyle::SP_DirOpenIcon));
		kresMap.insert(category, QHash<QString, Kite::KResource *>());
	}
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
			curProject->Path = frm.getPath();

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
			auto callb = kresCallbackMap.find(it.key()->text(0));
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
		chnode->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
		chnode->setText(0, child->getName().c_str());

		if (child->hasChild()) {
			loadChilds(Eman, (*it), chnode);
		}
	}
}

void MainWindow::createComponent(Kite::KHandle Entity, QString CName, QString CType) {
	// header
	auto category = new QTreeWidgetItem(propTree);
	QString name;
	if (CType == "Logic") {
		name = CType + "\t(" + CName + ")";
	} else {
		name = CType;
	}
	auto btnExpander = new Expander(name, QIcon(":/icons/save"), propTree, category);
	btnExpander->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	btnExpander->setAutoRaise(true);
	btnExpander->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(btnExpander, &Expander::customContextMenuRequested, this, &MainWindow::componentRClicked);
	propTree->setItemWidget(category, 0, btnExpander);

	// contetnts
	auto cframe = new QFrame(propTree);
	bindProperties(Entity, CName, CType, cframe);

	auto pContainer = new QTreeWidgetItem(category);
	pContainer->setDisabled(true);
	propTree->setItemWidget(pContainer, 0, cframe);
}

void MainWindow::bindProperties(Kite::KHandle EHandle, QString CName, QString CType, QFrame *Frame) {
	auto found = kcompNameMap.find(CType);
	if (found == kcompNameMap.end()) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("unregistred component name.\ncname: " + CName);
		msg.exec();
		return;
	}

	auto compMeta = (Kite::KMetaClass *)kmman->getMeta(found->toStdString());
	auto propList = compMeta->getProperties();
	auto flayout = new QFormLayout(Frame);
	auto prop = new KV2F32(Frame);

	prop->setCName(CName);
	prop->setCType(CType);
	prop->setEntity(EHandle);

	flayout->setContentsMargins(10, 0, 10, 0);
	flayout->setHorizontalSpacing(20);
	flayout->setVerticalSpacing(0);
	for (auto it = propList->begin(); it != propList->end(); ++it) {
		auto propMeta = kmman->getMeta(it->typeName);
		if (propMeta != nullptr) {
			addGUIItem(flayout, &(*it), propMeta);
		} else {
			QString val("Unregistred Property! Name: ");
			auto invalidType = new QLabel(val + it->name.c_str() + "Type: " + it->typeName.c_str());
			invalidType->setStyleSheet("QLabel { color : red; }");
			flayout->addRow(invalidType);
		}
	}

	Frame->setLayout(flayout);
}

void MainWindow::addGUIItem(QFormLayout *Layout, const Kite::KMetaProperty *Prop, const Kite::KMetaBase *Meta) {
	if (Meta->getMetaType() == KMetaTypes::KMT_POD) {
		//Layout->addRow(new QLabel(Prop->name.c_str()));

	} else if (Meta->getFlag() & POD && Meta->getMetaType() == KMetaTypes::KMT_CLASS) {
		if (Prop->typeName == "KVector2F32" || Prop->typeName == "KVector2I32") {
			QString pname(Prop->name.c_str());
			auto ledit = new QLineEdit();
			ledit->setValidator(new QIntValidator(0, 100, this));
			ledit->setStyleSheet("border: 1px solid gray; border-radius: 4px;");
			Layout->addRow(pname + ".x: ", ledit);

			ledit = new QLineEdit();
			ledit->setValidator(new QDoubleValidator(0, 100, 2, this));
			ledit->setStyleSheet("border: 1px solid gray; border-radius: 4px;");
			Layout->addRow(pname + ".y: ", ledit);

			Layout->addItem(new QSpacerItem(0, 5));
		}

	}else{
		QString val("Invalid Property! Name: ");
		auto invalidType = new QLabel(val + Prop->name.c_str() + " Type: " + Prop->typeName.c_str());
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
		kcurScene = nullptr;
	}

	delete scene;
}
