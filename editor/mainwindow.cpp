#include "mainwindow.h"
#include <kiteinfo.h>
#include <resourcedock.h>
#include <objectdock.h>
#include <componentdock.h>
#include <outputdock.h>
#include <maintab.h>
#include "executer.h"
#include <QtWidgets>
#include <gridscene.h>
#include <expander.h>
#include <vector>
#include <thread>
#include <string>
#include <climits>
#include "frmnewproj.h"
#include "frmprojsettings.h"
#include "frmabout.h"
#include "comproperty.h"
#include "objectscene.h"
#include <Kite/serialization/kbinaryserial.h>
#include <Kite/serialization/types/kstdstring.h>
#include <Kite/serialization/types/kstdumap.h>
#include <qdir.h>

using namespace Kite;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	curProject(nullptr),
	kinfo(new KiteInfo),
	exec(new Executer),
	undoGroup(new QUndoGroup(this))
{
    this->setMinimumSize(1000, 600);

	setupDocks();
	setupActions();
	setupStatusBar();
    setupToolbar();
	setupMenus();
    setupScene();
	loadDockState();

	connect(exec, &Executer::started, this, &MainWindow::engineStarted);
	connect(exec, &Executer::paused, this, &MainWindow::enginePaused);
	connect(exec, &Executer::unpaused, this, &MainWindow::engineUnpaused);
	connect(exec, &Executer::stoped, this, &MainWindow::engineStoped);
}

MainWindow::~MainWindow()
{}

void MainWindow::toggleFullScreen() {
	if (this->isFullScreen()) {
		this->showMaximized();

	} else {
		this->showFullScreen();
	}
}

void MainWindow::toggleDesignView() {
	static bool dview = (resDock->isHidden() && objDock->isHidden() && prpDock->isHidden());
	if (dview) {
		resDock->show();
		objDock->show();
		prpDock->show();

		dview = false;

	} else {
		resDock->hide();
		objDock->hide();
		prpDock->hide();
		outDock->hide();

		dview = true;
	}
}

void MainWindow::aboutDialogue() {
	static frmAbout *about = new frmAbout(this);
	about->exec();
}

void MainWindow::showSwitchOutput() {
	if (outDock->isHidden()) {
		outDock->show();
	} else {
		outDock->hide();
	}
}

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
    // resource dock
	resDock = new ResourceDock(this);
	resDock->setObjectName("Resources");
    addDockWidget(Qt::LeftDockWidgetArea, resDock);
	
	// output dock
	outDock = new OutputDock(this);
	addDockWidget(Qt::BottomDockWidgetArea, outDock);
	outDock->hide();

    // objects dock
    objDock = new ObjectDock(undoGroup, this);
    addDockWidget(Qt::LeftDockWidgetArea, objDock);	
	/*connect(resDock, &ResourceDock::resEdit, objDock, &ObjectDock::resEdit);
	connect(resDock, &ResourceDock::resDeleted, objDock, &ObjectDock::resDelete);
	connect(resDock, &ResourceDock::resAdded, objDock, &ObjectDock::installEntityCallback);
	connect(objDock, &ObjectDock::requestCreateResource, resDock, &ResourceDock::addNew);
	connect(objDock, &ObjectDock::requestGetResource, resDock, &ResourceDock::load);
	connect(objDock, &ObjectDock::requestResName, resDock, &ResourceDock::filter);*/

    // component/properties dock
    prpDock = new ComponentDock(this);
	prpDock->inite(*kinfo->getComponentTypes());
    addDockWidget(Qt::RightDockWidgetArea, prpDock);
	/*connect(objDock, &ObjectDock::objectSelected, prpDock, &ComponentDock::entityEdit);
	connect(objDock, &ObjectDock::objectDelete, prpDock, &ComponentDock::entityDelete);
	connect(prpDock, &ComponentDock::resSelected, resDock, &ResourceDock::selectResource);
	connect(prpDock, &ComponentDock::requestResNames, resDock, &ResourceDock::filterByType);
	connect(prpDock, &ComponentDock::requestResource, resDock, &ResourceDock::getResource);
	connect(prpDock, &ComponentDock::revertPrefab, objDock, &ObjectDock::revertPrefab);
	connect(prpDock, &ComponentDock::applyPrefab, objDock, &ObjectDock::applyPrefab);*/
	

	// kite class browser
	/*expDock = new QDockWidget(tr("Object Browser"), this);
	expDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, expDock);
	tabifyDockWidget(objDock, expDock);*/

	auto tview = new QTreeView();
	tview->setModel(kinfo->getModel());
	tview->setHeaderHidden(true);
	//expDock->setWidget(tview);
	setDockNestingEnabled(true);
}

void MainWindow::setupScene(){
	mainTab = new MainTab(undoGroup, kinfo, this);
	connect(resDock, &ResourceDock::resSelected, mainTab, &MainTab::select);
	connect(resDock, &ResourceDock::resEdit, mainTab, &MainTab::open);
	connect(resDock, &ResourceDock::resDeleted, mainTab, &MainTab::close);
	connect(resDock, &ResourceDock::resDeleted, mainTab, &MainTab::resourceDeleted);
	connect(mainTab, &MainTab::reqResList, resDock, &ResourceDock::filter);
	connect(mainTab, &MainTab::reqResLoad, resDock, &ResourceDock::load);
	connect(mainTab, &MainTab::reqResAdd, resDock, &ResourceDock::addNew);

    sceneView = new QGraphicsView();
	sceneView->scale(1, -1);
	sceneView->setMouseTracking(true);
	sceneView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
	sceneView->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

	setCentralWidget(mainTab);
	
	auto *scene1 = new ObjectScene(this);
	/*connect(resDock, &ResourceDock::resourceSelected, scene1, &ObjectScene::resSelect);
	connect(prpDock, &ComponentDock::componentEdited, scene1, &ObjectScene::componentEdited);
	connect(scene1, &ObjectScene::requestResource, resDock, &ResourceDock::getResource);*/

	//QPixmap plogo(":/icons/logo");
	//QGraphicsPixmapItem *logo = new QGraphicsPixmapItem(plogo);
	//logo->setPos((scene1->width() / 2) - (plogo.width() / 2), (scene1->height() / 2) - (plogo.height() / 2));
	//scene1->addItem(logo);
	sceneView->setScene(scene1);
}

void MainWindow::setupActions() {
	actNew = new QAction(QIcon(":/icons/new"), "New Project", this);
	actNew->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_N));
	actNew->setShortcutContext(Qt::ApplicationShortcut);
	connect(actNew, &QAction::triggered, this, &MainWindow::newProject);

	actOpen = new QAction(QIcon(":/icons/open"), "Open Project", this);
	actOpen->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_O));
	actOpen->setShortcutContext(Qt::ApplicationShortcut);
	connect(actOpen, &QAction::triggered, this, &MainWindow::openProject);

	actSave = new QAction(QIcon(":/icons/save"), "Save Project", this);
	actSave->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
	actSave->setShortcutContext(Qt::ApplicationShortcut);
	actSave->setDisabled(true);
	connect(actSave, &QAction::triggered, this, &MainWindow::applyChanges);

	actClose = new QAction("Close Project", this);
	actClose->setDisabled(true);
	connect(actClose, &QAction::triggered, this, &MainWindow::closeProject);

	actPlay = new QAction(QIcon(":/icons/play"), "Start Debugging", this);
	actPlay->setShortcut(QKeySequence(Qt::Key_F5));
	actPlay->setShortcutContext(Qt::ApplicationShortcut);
	actPlay->setDisabled(true);
	connect(actPlay, &QAction::triggered, this, &MainWindow::startEngine);

	actPause = new QAction(QIcon(":/icons/pause"), "Pause", this);
	actPause->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_Pause));
	actPause->setShortcutContext(Qt::ApplicationShortcut);
	actPause->setDisabled(true);
	connect(actPause, &QAction::triggered, exec, &Executer::pause);

	actStop = new QAction(QIcon(":/icons/stop"), "Stop", this);
	actStop->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F5));
	actStop->setShortcutContext(Qt::ApplicationShortcut);
	actStop->setDisabled(true);
	connect(actStop, &QAction::triggered, exec, &Executer::stop);

	actSetting = new QAction(QIcon(":/icons/set"), "Project Settings", this);
	actSetting->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F7));
	actSetting->setShortcutContext(Qt::ApplicationShortcut);
	actSetting->setDisabled(true);
	connect(actSetting, &QAction::triggered, this, &MainWindow::openProjSetting);

	actShowOut = new QAction("Output", this);
	actShowOut->setShortcut(QKeySequence(Qt::ALT + Qt::Key_2));
	actShowOut->setShortcutContext(Qt::ApplicationShortcut);
	connect(actShowOut, &QAction::triggered, this, &MainWindow::showSwitchOutput);

	actExit = new QAction(QIcon(":/icons/exit"), "Exit", this);
	actExit->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));
	actExit->setShortcutContext(Qt::ApplicationShortcut);
	connect(actExit, &QAction::triggered, this, &MainWindow::exitApp);
}

void MainWindow::setupMenus(){
	// menu bar
	menuBar()->setStyleSheet("QMenuBar { border: 0px; }\n"
							 "QMenu::separator { height: 1px;\n background: rgb(129, 129, 129);\n margin-left: 25px;}");

	// menuba main frame
	auto mframe = new QFrame(this);
	auto hlayout = new QHBoxLayout(mframe);
	hlayout->setMargin(0);

	// fullscreen exit
	auto fscreenExit = new QToolButton(this);
	fscreenExit->setDefaultAction(actExit);
	fscreenExit->setAutoRaise(true);

	// notifications
	auto btnNotify = new QToolButton(this);
	btnNotify->setIcon(QIcon(":/icons/notifi"));
	btnNotify->setToolTip("Notifications");
	btnNotify->setAutoRaise(true);
	btnNotify->setDisabled(true);

	hlayout->addWidget(btnNotify);
	hlayout->addWidget(fscreenExit);
	menuBar()->setCornerWidget(mframe);

	// file
    fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(actNew);
	fileMenu->addAction(actOpen);
	fileMenu->addSeparator();
	fileMenu->addAction(actClose);
	fileMenu->addSeparator();
	fileMenu->addAction(actSave);
	fileMenu->addSeparator();
	fileMenu->addAction(actExit);

	// edit
	fileMenu = menuBar()->addMenu(tr("&Edit"));
	fileMenu->addAction(actUndo);
	fileMenu->addAction(actRedo);

	// view
	winMenu = menuBar()->addMenu(tr("&View"));
	auto fscreen = winMenu->addAction("Full Screen (Toggle)");
	fscreen->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_F));
	fscreen->setShortcutContext(Qt::ApplicationShortcut);
	this->addAction(fscreen);
	connect(fscreen, &QAction::triggered, this, &MainWindow::toggleFullScreen);

	auto designView = winMenu->addAction("Level Designer (Toggle)");
	designView->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
	designView->setShortcutContext(Qt::ApplicationShortcut);
	this->addAction(designView);
	connect(designView, &QAction::triggered, this, &MainWindow::toggleDesignView);

	// test
	fileMenu = menuBar()->addMenu(tr("&Debug"));
	fileMenu->addAction(actPlay);
	fileMenu->addAction(actPause);
	fileMenu->addAction(actStop);

	// window
	auto showAbout = new QAction("About Kite2D Game Editor", this);
	connect(showAbout, &QAction::triggered, this, &MainWindow::aboutDialogue);

	winMenu = menuBar()->addMenu(tr("&Window"));
	winMenu->addSeparator();
	winMenu->addAction(showAbout);

}

void MainWindow::setupToolbar(){
	actUndo = undoGroup->createUndoAction(this);
	actUndo->setIcon(QIcon(":/icons/undo"));
	actUndo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
	actUndo->setShortcutContext(Qt::ApplicationShortcut);

	actRedo = undoGroup->createRedoAction(this);
	actRedo->setIcon(QIcon(":/icons/redo"));
	actRedo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
	actRedo->setShortcutContext(Qt::ApplicationShortcut);

    auto fileTolb = new QToolBar("Project");
	addToolBar(Qt::ToolBarArea::LeftToolBarArea, fileTolb);
	fileTolb->setObjectName("Project");
	fileTolb->setStyleSheet("QToolBar { border: 0px }");
	fileTolb->addAction(actNew);
	fileTolb->addAction(actOpen);
	fileTolb->addAction(actSave);
	fileTolb->addSeparator();
	fileTolb->addAction(actUndo);
	fileTolb->addAction(actRedo);
	fileTolb->addSeparator();
	fileTolb->addAction(actSetting);
	
	auto debugTolb = new QToolBar("Debug");
	addToolBar(Qt::ToolBarArea::LeftToolBarArea, debugTolb);
	debugTolb->setObjectName("Debug");
	debugTolb->setStyleSheet("QToolBar { border: 0px }");
	debugTolb->addAction(actPlay);
	debugTolb->addAction(actPause);
	debugTolb->addAction(actStop);
}

void MainWindow::setupStatusBar() {
	auto klabel = new QLabel(this);
	klabel->setText("Kite2D Game Editor ver " + QString::number(K2D_VER_MAJ) +
					"." + QString::number(K2D_VER_MIN) +
					"." + QString::number(K2D_VER_BUILD));
	klabel->setStyleSheet("color: orange;");
	QMainWindow::statusBar()->addPermanentWidget(klabel);

	auto btnShowOutputPan = new QToolButton(this);
	btnShowOutputPan->setDefaultAction(actShowOut);
	btnShowOutputPan->setToolButtonStyle(Qt::ToolButtonTextOnly);
	QMainWindow::statusBar()->addWidget(btnShowOutputPan);
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

void MainWindow::saveXML(){
	QFile file(curProject->Path + "/" + curProject->name + ".k2d");
	if (!file.open(QIODevice::WriteOnly)) {
		file.close();
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("cant save changes to project files.");
		msg.exec();
		return;
	}

	QXmlStreamWriter stream(&file);
	KFOStream fstream;
	QDir dir(curProject->Path);
	Kite::KBinarySerial bserial;

	stream.setAutoFormatting(true);
	stream.writeStartDocument();

	stream.writeStartElement("k2dproject");
	stream.writeAttribute("name", curProject->name);
	stream.writeAttribute("version", "1.0");

	// resources
	std::unordered_map<std::string, std::string> dict;
	stream.writeStartElement("resources");
	QList<DumpItem> rlist;
	resDock->dump(rlist);
	for (auto it = rlist.cbegin(); it != rlist.cend(); ++it) {
		dict.insert({ it->name.toStdString(), it->address.toStdString() + "/" + it->name.toStdString() });

		stream.writeStartElement("item");
		stream.writeAttribute("address", dir.relativeFilePath(QString(it->address) + "/" + it->name));
		stream.writeAttribute("type", Kite::getRTypesName(it->type).c_str());
		if (mainTab->isOpen(it->name)) {
			stream.writeAttribute("open", "true");
		} else {
			stream.writeAttribute("open", "false");
		}
		stream.writeEndElement();
	}

	stream.writeEndElement(); // resources

	stream.writeEndElement(); // k2dproject
	stream.writeEndDocument();

	// save dictinary
	bserial.clearCatch();
	bserial << dict;
	bserial.saveStream(fstream, curProject->Path.toStdString() + "/dict.kdict", 0);
	curProject->config.dictionary = curProject->Path.toStdString() + "/dict.kdict";

	// save config
	bserial.clearCatch();
	bserial << curProject->config;
	bserial.saveStream(fstream, curProject->Path.toStdString() + "/config.kconf", 0);

	file.close();
}

bool MainWindow::loadXML(QIODevice *device, const QString &Address) {
	QXmlStreamReader xml(device);
	bool head = false;
	bool res = false;

	if (xml.readNextStartElement()) {
		if (xml.name() == "k2dproject" && xml.attributes().value("version") == "1.0") {
			QFileInfo finfo(Address);
			curProject->name = xml.attributes().value("name").toString();
			curProject->Path = finfo.path();
			resDock->setResDirectory(curProject->Path + "/resources");

			// config
			KFIStream fstream;
			Kite::KBinarySerial bserial;
			if (!bserial.loadStream(fstream, finfo.path().toStdString() + "/config.kconf")) {
				QMessageBox msg;
				msg.setWindowTitle("Message");
				msg.setText("cant load project file.\nfile address: " + Address
							+ "\nmissing config file!");
				msg.exec();
				return false;
			}
			bserial >> curProject->config;
			//curProject->config.dictionary = finfo.path().toStdString() + "/dict.kdict";
			head = true;
		}

		while (!xml.atEnd()) {
			xml.readNext();				
			// resources
			if (xml.isStartElement() && xml.name() == "resources") {
				res = true;
				while (!xml.atEnd()) {
					xml.readNext();
					if (xml.isStartElement() && xml.name() == "item") {
						if (!resDock->addExisting(curProject->Path + "/" + xml.attributes().value("address").toString(),
												  Kite::getRTypesByName(xml.attributes().value("type").toString().toStdString()), false)) {
							return false;
						}

						// open tab
						if (xml.attributes().value("open").toString() == "true") {
							QFileInfo finfo(curProject->Path + "/" + xml.attributes().value("address").toString());
							mainTab->open(finfo.fileName(), Kite::getRTypesByName(xml.attributes().value("type").toString().toStdString()));
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
	return (head && res);
}

void MainWindow::initProject() {
	this->setWindowTitle("Kite2D Editor - " + curProject->Path + "/" + curProject->name);

	resDock->setResDirectory(curProject->Path + "/resources");
	resDock->initePanel();
	objDock->initePanel();

	actPlay->setDisabled(false);
	actClose->setDisabled(false);
	actSave->setDisabled(false);
	actSetting->setDisabled(false);
}

void MainWindow::clearProject() {
	this->setWindowTitle("Kite2D Editor");

	// clear all panels in order
	objDock->clearPanel();
	resDock->clearPanel();

	actPlay->setDisabled(true);
	actClose->setDisabled(true);
	actSave->setDisabled(true);
	actSetting->setDisabled(true);

	delete curProject;
	curProject = nullptr;
}

void MainWindow::startEngine() {
	applyChanges();
	outDock->getEditor()->clear();
	outDock->autoShow();
	connect(exec, &Executer::engineOutput, this, &MainWindow::getEngineOutput);
	static Kite::KConfig tempConfig;
	tempConfig = curProject->config;

	// we have problem with fullscreen and i dont know what is this shit!
	// so we try to simulate fullscreen mode with window mode
	if (curProject->config.window.fullscreen) {
		tempConfig.window.fullscreen = false;
		tempConfig.window.resizable = false;
		QRect rec = QApplication::desktop()->screenGeometry();
		tempConfig.window.width = rec.width();	
		tempConfig.window.height = rec.height();
		tempConfig.window.xpos = 0;
		tempConfig.window.ypos = 1; // this must be 1 otherwise we have unknowen problem!
	}
	exec->run(&tempConfig);
}

void MainWindow::getEngineOutput(const QString &Text, int MType) {
	switch ((Kite::msgType)MType) {
	case Kite::msgType::MSG_DEBUG:
		outDock->getEditor()->appendHtml("<font color=\"red\"><b>DEBUG: </b></font>" + Text);
		break;
	case Kite::msgType::MSG_BREAK:
		outDock->getEditor()->appendHtml("<font color=\"red\"><b>BREAK: </b></font>" + Text);
		break;
	case Kite::msgType::MSG_ASSERT:
		outDock->getEditor()->appendHtml("<font color=\"red\"><b>ASSERT: </b></font>" + Text);
		break;
	case Kite::msgType::MSG_LUA:
		outDock->getEditor()->appendPlainText("Output: " + Text);
		break;
	default:
		outDock->getEditor()->appendHtml("<font color=\"yellow\"><b>Other: </b></font>" + Text);
		break;
	}
}

void MainWindow::engineStarted() {
	//outDock->getEditor()->appendHtml("<font color = \"Aqua\">---- Engine Started ----</font>");
	outDock->getEditor()->appendPlainText("----[ Engine Started ]----");
	actPlay->setDisabled(true);
	actPause->setDisabled(false);
	actStop->setDisabled(false);
}

void MainWindow::enginePaused() {
	//outDock->getEditor()->appendHtml("<font color = \"Aqua\">---- Engine Paused ----</font>");
	outDock->getEditor()->appendPlainText("----[ Engine Paused ]----");
	actPlay->setDisabled(false);
	actPause->setDisabled(true);
	actStop->setDisabled(false);
	disconnect(actPlay, &QAction::triggered, 0, 0);
	connect(actPlay, &QAction::triggered, exec, &Executer::unpause);
}

void MainWindow::engineUnpaused() {
	//outDock->getEditor()->appendHtml("<font color = \"Aqua\">---- Engine Unpaused ----</font>");
	outDock->getEditor()->appendPlainText("----[ Engine Unpaused ]----");
	actPlay->setDisabled(true);
	actPause->setDisabled(false);
	actStop->setDisabled(false);
	disconnect(actPlay, &QAction::triggered, 0, 0);
	connect(actPlay, &QAction::triggered, this, &MainWindow::startEngine);
}

void MainWindow::engineStoped() {
	disconnect(exec, &Executer::engineOutput, this, &MainWindow::getEngineOutput);
	//outDock->getEditor()->appendHtml("<font color = \"Aqua\">---- Engine Stoped ----</font>");
	outDock->getEditor()->appendPlainText("----[ Engine Stoped ]----");
	outDock->autoHide();
	actPlay->setDisabled(false);
	actPause->setDisabled(true);
	actStop->setDisabled(true);
	disconnect(actPlay, &QAction::triggered, 0, 0);
	connect(actPlay, &QAction::triggered, this, &MainWindow::startEngine);
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
			curProject->config.window.title = curProject->name.toStdString();
			curProject->config.window.width = 800;
			curProject->config.window.height = 600;
			curProject->config.window.xpos = 100;
			curProject->config.window.ypos = 100;
			curProject->config.window.fullscreen = false;
			curProject->config.window.showCursor = true;
			curProject->config.window.resizable = false;
			curProject->config.dictionary = curProject->Path.toStdString() + "/dict.kdict";

			// activate panels and actions
			initProject();
		}
	}
}

void MainWindow::openProject() {
		QString fileName = QFileDialog::getOpenFileName(this,
												"Open Project", "", "Kite2D Project File (*.k2d)");

	if (!fileName.isEmpty()) {
		// close current project if any
		closeProject();

		if (curProject == nullptr) {
			QFile file(fileName);
			if (file.open(QIODevice::ReadOnly)) {
				curProject = new Project;
				resDock->blockSignals(true);
				if (loadXML(&file, fileName)) {
					initProject();
				} else {
					closeProject(true);
				}
				file.close();
				resDock->blockSignals(false);
			}
		}
	}
}

void MainWindow::applyChanges() {
	if (curProject != nullptr) {
		resDock->manageUsedResource(kinfo->getResourceComponentsTypes());
		mainTab->saveAll();
	}
}

void MainWindow::closeProject(bool Silent) {
	if (curProject != nullptr) {
		if (!Silent && mainTab->needSave()) {
			int ret = QMessageBox::warning(this, "Kite2D Editor",
										   "Do you want to save changes to " + curProject->name + "?",
										   QMessageBox::Save | QMessageBox::Discard
										   | QMessageBox::Cancel,
										   QMessageBox::Save);

			// apply all current (unsaved) changes befor saving
			if (ret == QMessageBox::Save) {
				applyChanges();

			} else if (ret == QMessageBox::Cancel) {
				return;
			}
		}

		saveXML();
		clearProject();
	}
}

void MainWindow::openProjSetting() {
	QStringList list;
	resDock->filter(Kite::RTypes::Scene, list);
	list.push_front("<default>"); // default scene
	frmProjSettings frm(this, &curProject->config, list);
	frm.exec();
}
