#include "mainwindow.h"
#include <QtWidgets>
#include <gridscene.h>
#include <expander.h>
#include <vector>
#include <thread>
#include <string>
#include <climits>
#include "frmnewproj.h"
#include "frmprojsettings.h"
#include "comproperty.h"
#include <Kite/serialization/kbinaryserial.h>
#include <Kite/serialization/types/kstdstring.h>
#include <Kite/serialization/types/kstdumap.h>

using namespace Kite;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
	curProject(nullptr),
	kinfo(new KiteInfo),
	exec(new Executer)
{
    this->setMinimumSize(1000, 600);
	setupStatusBar();
	setupActions();
    setupToolbar();
	setupMenus();
    setupDocks();
    setupScene();
	loadDockState();
	disGUI();

	connect(exec, &Executer::started, this, &MainWindow::engineStarted);
	connect(exec, &Executer::paused, this, &MainWindow::enginePaused);
	connect(exec, &Executer::unpaused, this, &MainWindow::engineUnpaused);
	connect(exec, &Executer::stoped, this, &MainWindow::engineStoped);
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
	resTree->setupCategories(*kinfo->getResourceTypes());
	resDock->setWidget(resTree);
	resDock->setTitleBarWidget(resTree->getHeaderTools());

	// output dock
	outDock = new QDockWidget(tr("Engine Output"), this);
	outDock->setObjectName("Resources");
	outDock->setAllowedAreas(Qt::BottomDockWidgetArea);
	outDock->setMinimumWidth(120);
	addDockWidget(Qt::BottomDockWidgetArea, outDock);

	koutput = new QTextEdit(this);
	koutput->setReadOnly(true);
	connect(exec, &Executer::engineOutput, koutput, &QTextEdit::append);
	outDock->setWidget(koutput);

    // objects dock
    objDock = new QDockWidget(tr("Hierarchy"), this);
	objDock->setObjectName("Hierarchy");
    objDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    objDock->setMinimumWidth(120);
    addDockWidget(Qt::LeftDockWidgetArea, objDock);

	objTree = new ObjectTree(this);
	objDock->setWidget(objTree);
	objDock->setTitleBarWidget(objTree->getHeaderTools());

	connect(resTree, &ResourceTree::resourceSelected, objTree, &ObjectTree::sceneEdit);
	connect(resTree, &ResourceTree::resourceDelete, objTree, &ObjectTree::sceneDelete);

    // component/properties dock
    prpDock = new QDockWidget(tr("Components Editor"), this);
	prpDock->setObjectName("Components Editor");
    prpDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    prpDock->setMinimumWidth(120);
    addDockWidget(Qt::RightDockWidgetArea, prpDock);

	propTree = new ComponentTree;
	propTree->setupTypes(*kinfo->getComponentTypes());
	propTree->setResDictionary(resTree->getDictionary());
	prpDock->setWidget(propTree);
	prpDock->setTitleBarWidget(propTree->getHeaderTools());

	connect(objTree, &ObjectTree::objectSelected, propTree, &ComponentTree::entityEdit);
	connect(objTree, &ObjectTree::objectDelete, propTree, &ComponentTree::entityDelete);

	// kite object browser
	expDock = new QDockWidget(tr("Object Browser"), this);
	expDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, expDock);
	tabifyDockWidget(objDock, expDock);

	auto tview = new QTreeView();
	tview->setModel(kinfo->getModel());
	tview->setHeaderHidden(true);
	expDock->setWidget(tview);

	QMainWindow::statusBar()->showMessage("Ready");
}

void MainWindow::setupScene(){
	mainTab = new MainTab(this);
	mainTab->setCompleterModel(kinfo->getModel());
	connect(resTree, &ResourceTree::resourceSelected, mainTab, &MainTab::selectResource);
	connect(resTree, &ResourceTree::resourceEdit, mainTab, &MainTab::openTabs);
	connect(resTree, &ResourceTree::resourceDelete, mainTab, &MainTab::closeResource);

    sceneView = new QGraphicsView();
	mainTab->addTab(sceneView, "Scene");

	setCentralWidget(mainTab);
	
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
	newProj->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_N));
	newProj->setShortcutContext(Qt::ApplicationShortcut);
	connect(newProj, &QAction::triggered, this, &MainWindow::newProject);

	openProj = new QAction(QIcon(":/icons/open"), "Open Project", this);
	openProj->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_O));
	openProj->setShortcutContext(Qt::ApplicationShortcut);
	connect(openProj, &QAction::triggered, this, &MainWindow::openProject);

	saveProj = new QAction(QIcon(":/icons/save"), "Save Project", this);
	saveProj->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
	saveProj->setShortcutContext(Qt::ApplicationShortcut);
	connect(saveProj, &QAction::triggered, this, &MainWindow::saveProject);

	closeProj = new QAction("Close Project", this);
	connect(closeProj, &QAction::triggered, this, &MainWindow::closeProject);

	playScene = new QAction(QIcon(":/icons/play"), "Play", this);
	playScene->setShortcut(QKeySequence(Qt::Key_F5));
	playScene->setShortcutContext(Qt::ApplicationShortcut);
	connect(playScene, &QAction::triggered, this, &MainWindow::startEngine);

	pauseScene = new QAction(QIcon(":/icons/pause"), "Pause", this);
	pauseScene->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_Pause));
	pauseScene->setShortcutContext(Qt::ApplicationShortcut);
	connect(pauseScene, &QAction::triggered, exec, &Executer::pause);

	stopScene = new QAction(QIcon(":/icons/stop"), "Stop", this);
	stopScene->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F5));
	stopScene->setShortcutContext(Qt::ApplicationShortcut);
	connect(stopScene, &QAction::triggered, exec, &Executer::stop);

	projSettings = new QAction(QIcon(":/icons/set"), "Project Settings", this);
	projSettings->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F7));
	projSettings->setShortcutContext(Qt::ApplicationShortcut);
	connect(projSettings, &QAction::triggered, this, &MainWindow::openProjSetting);

	exit = new QAction(QIcon(":/icons/exit"), "Exit", this);
	exit->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));
	exit->setShortcutContext(Qt::ApplicationShortcut);
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

	// test
	fileMenu = menuBar()->addMenu(tr("&Test"));
	fileMenu->addAction(playScene);
	fileMenu->addAction(pauseScene);
	fileMenu->addAction(stopScene);

	// window
	winMenu = menuBar()->addMenu(tr("&Window"));

}

void MainWindow::setupToolbar(){
    fileTolb = addToolBar(tr("Toolbar"));
	fileTolb->setObjectName("Toolbar");
	fileTolb->addAction(newProj);
	fileTolb->addAction(openProj);
	fileTolb->addSeparator();
	fileTolb->addAction(saveProj);
	fileTolb->addSeparator();
	fileTolb->addAction(projSettings);
	fileTolb->addSeparator();
	fileTolb->addAction(playScene);
	fileTolb->addAction(pauseScene);
	fileTolb->addAction(stopScene);
}

void MainWindow::setupStatusBar() {
	auto klabel = new QLabel(this);
	klabel->setText("Kite2D Editor ver 0.1");
	klabel->setStyleSheet("color: orange;");
	QMainWindow::statusBar()->addPermanentWidget(klabel);
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
	pauseScene->setDisabled(true);
	stopScene->setDisabled(true);
	saveProj->setDisabled(true);
	closeProj->setDisabled(true);
	playScene->setDisabled(true);
	resDock->setDisabled(true);
	objDock->setDisabled(true);
	prpDock->setDisabled(true);
	projSettings->setDisabled(true);
}

void MainWindow::enGUI() {
	playScene->setDisabled(false);
	saveProj->setDisabled(false);
	closeProj->setDisabled(false);
	playScene->setDisabled(false);
	resDock->setDisabled(false);
	objDock->setDisabled(false);
	prpDock->setDisabled(false);
	projSettings->setDisabled(false);
}

void MainWindow::saveXML(QIODevice *device, const QString &Address){
	QXmlStreamWriter stream(device);
	stream.setAutoFormatting(true);
	stream.writeStartDocument();

	stream.writeStartElement("k2dproject");
	stream.writeAttribute("name", curProject->name);
	stream.writeAttribute("version", "1.0");

	// config
	stream.writeStartElement("config");
	stream.writeStartElement("window");
	stream.writeAttribute("title", curProject->config.window.title.c_str());
	stream.writeAttribute("width", QString::number(curProject->config.window.width));
	stream.writeAttribute("height", QString::number(curProject->config.window.height));
	stream.writeAttribute("xpos", QString::number(curProject->config.window.xpos));
	stream.writeAttribute("ypos", QString::number(curProject->config.window.ypos));
	stream.writeAttribute("startupScene", curProject->config.startUpScene.c_str());
	if (curProject->config.window.fullscreen) {
		stream.writeAttribute("fullscreen", "true");
	} else {
		stream.writeAttribute("fullscreen", "false");
	}
	if (curProject->config.window.showCursor) {
		stream.writeAttribute("showcursor", "true");
	} else {
		stream.writeAttribute("showcursor", "false");
	}
	if (curProject->config.window.resizable) {
		stream.writeAttribute("resizable", "true");
	} else {
		stream.writeAttribute("resizable", "false");
	}
	stream.writeEndElement(); // config.window
	stream.writeEndElement(); // config

	// resources
	stream.writeStartElement("resources");
	auto resDict = resTree->getDictionary();
	for (auto it = resDict->cbegin(); it != resDict->cend(); ++it) {
		KFOStream fstream;
		fstream.open(Address.toStdString() + "/resources/" + it.key().toStdString() + ".kres", Kite::IOMode::BIN);
		(*it)->saveStream(&fstream);
		fstream.close();

		fstream.open(Address.toStdString() + "/dict.kdict", Kite::IOMode::BIN);
		Kite::KBinarySerial bserial;
		bserial << *resTree->getKiteDictionary(Address + "/");
		bserial.saveStream(&fstream);
		fstream.close();

		stream.writeStartElement("item");
		stream.writeAttribute("name", it.key());
		stream.writeAttribute("type", (*it)->getResourceType().c_str());
		stream.writeEndElement();
	}

	stream.writeEndElement(); // resources
	stream.writeEndElement(); // k2dproject
	stream.writeEndDocument();
}

bool MainWindow::loadXML(QIODevice *device, const QString &Address) {
	QXmlStreamReader xml(device);
	bool head = false;
	bool config = false;
	bool res = false;

	if (xml.readNextStartElement()) {
		if (xml.name() == "k2dproject" && xml.attributes().value("version") == "1.0") {
			QFileInfo finfo(Address);
			curProject = new Project;
			curProject->name = xml.attributes().value("name").toString();
			curProject->Path = finfo.path();
			curProject->resPath = finfo.path() + "/resources";
			curProject->config.dictionary = finfo.path().toStdString() + "/dict.kdict";
			head = true;
		}

		while (!xml.atEnd()) {
			xml.readNext();

			// config
			if (xml.isStartElement() && xml.name() == "config") {
				config = true;
				while (!xml.isEndElement()) {
					xml.readNext();

					// window
					if (xml.isStartElement() && xml.name() == "window") {
						curProject->config.window.title = xml.attributes().value("title").toString().toStdString();
						curProject->config.window.width = xml.attributes().value("width").toInt();
						curProject->config.window.height = xml.attributes().value("height").toInt();
						curProject->config.window.xpos = xml.attributes().value("xpos").toInt();
						curProject->config.window.ypos = xml.attributes().value("ypos").toInt();
						curProject->config.startUpScene = xml.attributes().value("startupScene").toString().toStdString();
						if (xml.attributes().value("fullscreen").toString() == "true") {
							curProject->config.window.fullscreen = true;
						} else {
							curProject->config.window.fullscreen = false;
						}

						if (xml.attributes().value("showcursor").toString() == "true") {
							curProject->config.window.showCursor = true;
						} else {
							curProject->config.window.showCursor = false;
						}

						if (xml.attributes().value("resizable").toString() == "true") {
							curProject->config.window.resizable = true;
						} else {
							curProject->config.window.resizable = false;
						}

					}
				}
			}
				
			// resources
			if (xml.isStartElement() && xml.name() == "resources") {
				res = true;
				while (!xml.atEnd()) {
					xml.readNext();
					if (xml.isStartElement() && xml.name() == "item") {
						if (!resTree->openResource(curProject->resPath + "/" + xml.attributes().value("name").toString() + ".kres",
												  xml.attributes().value("type").toString())) {
							return false;
						}
					}
				}
			}
		}
	}

	if (xml.hasError()) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("cant load project file.\nfile address: " + Address 
					+ "\nXML Error: " + xml.errorString());
		msg.exec();
		return false;
	}
	return (head && res && config);
}

void MainWindow::startEngine() {
	saveProject();
	koutput->clear();
	exec->run(&curProject->config);
}

void MainWindow::engineStarted() {
	playScene->setDisabled(true);
	pauseScene->setDisabled(false);
	stopScene->setDisabled(false);
}

void MainWindow::enginePaused() {
	playScene->setDisabled(false);
	pauseScene->setDisabled(true);
	stopScene->setDisabled(false);
	disconnect(playScene, &QAction::triggered, 0, 0);
	connect(playScene, &QAction::triggered, exec, &Executer::unpause);
}

void MainWindow::engineUnpaused() {
	playScene->setDisabled(true);
	pauseScene->setDisabled(false);
	stopScene->setDisabled(false);
	disconnect(playScene, &QAction::triggered, 0, 0);
	connect(playScene, &QAction::triggered, this, &MainWindow::startEngine);
}

void MainWindow::engineStoped() {
	playScene->setDisabled(false);
	pauseScene->setDisabled(true);
	stopScene->setDisabled(true);
	disconnect(playScene, &QAction::triggered, 0, 0);
	connect(playScene, &QAction::triggered, this, &MainWindow::startEngine);
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
			curProject->config.window.title = curProject->name.toStdString();
			curProject->config.window.width = 800;
			curProject->config.window.height = 600;
			curProject->config.window.xpos = 100;
			curProject->config.window.ypos = 100;
			curProject->config.window.fullscreen = false;
			curProject->config.window.showCursor = true;
			curProject->config.window.resizable = false;
			curProject->config.dictionary = curProject->Path.toStdString() + "/dict.kdict";

			this->setWindowTitle("Kite2D Editor - " + frm.getName());

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

		if (!fileName.isEmpty()) {
			QFile file(fileName);
			if (file.open(QIODevice::ReadOnly)) {
				if (loadXML(&file, fileName)) {
					enGUI();
				}
				file.close();
			}
		}
	}
}

void MainWindow::saveProject() {
	if (curProject != nullptr) {
		resTree->manageUsedResource(kinfo->getResourceComponentsTypes());
		mainTab->saveAll();
		QFile file(curProject->Path + "/" + curProject->name + ".k2d");
		if (file.open(QIODevice::WriteOnly)) {
			saveXML(&file, curProject->Path);
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
		resTree->clearResources();
		resTree->setupCategories(*kinfo->getResourceTypes());

		delete curProject;
		curProject = nullptr;

		disGUI();
	}
}

void MainWindow::openProjSetting() {
	QStringList items;
	resTree->filterByType("KScene", items);
	frmProjSettings frm(this, &curProject->config, items);
	frm.exec();
}
