#include "maintab.h"
#include <QtWidgets>
#include "luaeditor.h"
#include "glsleditor.h"
#include "shprogeditor.h"
#include "textureeditor.h"
#include "atlaseditor.h"
#include <qpropertyanimation.h>
#include <kmeta.khgen.h>

MainTab::MainTab(KiteInfo *KInfo,QWidget *parent) :
	QTabWidget(parent),
	kinfo(KInfo),
	scene(nullptr)
{
	setStyleSheet("QTabBar::tab { height: 23px;}");
	setMovable(true);
	registerTabs();
}

MainTab::~MainTab() {}

void MainTab::setScene(QWidget *Scene) {
	if (scene == nullptr) {
		scene = Scene;
		addTab(Scene, "Scene");
	}
}

bool MainTab::isOpen(Kite::KResource *Res) {
	if (resMap.find(Res) == resMap.end()) {
		return false;
	}
	return true;
}

void MainTab::focusInEvent(QFocusEvent *Event) {
	for (auto it = shortcuts.begin(); it != shortcuts.end(); ++it) {
		(*it)->setEnabled(true);
	}
}

void MainTab::focusOutEvent(QFocusEvent *Event) {
	for (auto it = shortcuts.begin(); it != shortcuts.end(); ++it) {
		(*it)->setEnabled(false);
	}
}

bool MainTab::eventFilter(QObject *Obj, QEvent *Event) {
	// disable ALT+F4 on dock widgets
	if (Event->type() == QEvent::Close) {
		Event->ignore();
		return true;
	}
	return false;
}

void MainTab::registerTabs() {
	tabFactory.clear();
	tabFactory.insert((size_t)Kite::RTypes::Script, LuaEditor::factory);
	tabFactory.insert((size_t)Kite::RTypes::Shader, GLSLEditor::factory);
	tabFactory.insert((size_t)Kite::RTypes::ShaderProgram, ShProgEditor::factory);
	tabFactory.insert((size_t)Kite::RTypes::Texture, TextureEditor::factory);
	tabFactory.insert((size_t)Kite::RTypes::AtlasTexture, AtlasEditor::factory);
}

int MainTab::createTab(QWidget *Widget, Kite::KResource *ResPtr) {
	auto frame = new QFrame();
	auto flayout = new QHBoxLayout(frame);
	flayout->setMargin(0);
	flayout->setSpacing(3);

	auto closeTab = new QAction(QIcon(":/icons/close"), "Close Tab", frame);
	connect(closeTab, &QAction::triggered, this, &MainTab::closeTab);

	auto unpinTab = new QAction(QIcon(":/icons/unpin"), "Unpin Tab", frame);
	connect(unpinTab, &QAction::triggered, this, &MainTab::unpinTab);

	auto btnUnpin = new QToolButton();
	btnUnpin->setIconSize(QSize(8, 8));
	btnUnpin->setDefaultAction(unpinTab);
	btnUnpin->setAutoRaise(true);
	unpinTab->setParent(btnUnpin);
	flayout->addWidget(btnUnpin);

	auto btnClose = new QToolButton();
	btnClose->setDefaultAction(closeTab);
	btnClose->setIconSize(QSize(8, 8));
	btnClose->setAutoRaise(true);
	closeTab->setParent(btnClose);
	flayout->addWidget(btnClose);

	frame->setLayout(flayout);

	closeTab->setData(qVariantFromValue((void *) ResPtr));
	unpinTab->setData(qVariantFromValue((void *)ResPtr));

	auto tid = addTab(Widget, ResPtr->getName().c_str());
	tabBar()->setTabButton(tid, QTabBar::ButtonPosition::RightSide, frame);

	return tid;
}

void MainTab::deleteTab(QWidget *Widget) {
	auto index = indexOf(Widget);
	if (index >= 0) {
		auto tframe = tabBar()->tabButton(index, QTabBar::ButtonPosition::RightSide);
		auto twidget = widget(index);
		removeTab(index);
		delete twidget;
		delete tframe;
	}
}

void MainTab::deleteDock(QDockWidget *Dock) {
	auto dockHead = (QFrame *)Dock->titleBarWidget();
	auto editor = (CodeEditor *)Dock->widget();
	Dock->close();

	delete editor;
	delete dockHead;
	delete Dock;
}

void MainTab::selectResource(Kite::KResource *Res) {
	auto found = resMap.find(Res);

	if (found == resMap.end()) {
		if (Res->getType() == Kite::RTypes::Scene) {
			auto ind = indexOf(scene);
			setTabText(ind, Res->getName().c_str());
			setCurrentIndex(ind);
		}

	} else {
		// pined (tab)
		if ((*found).second == nullptr) {
			auto ind = indexOf((*found).first);
			setCurrentIndex(ind);

		// unpined (dock)
		} else {
			(*found).second->raise();
			auto animation = new QPropertyAnimation ((*found).second, "windowOpacity");
			animation->setDuration(1000);
			animation->setKeyValueAt(0, 1);
			animation->setKeyValueAt(0.5, 0.5);
			animation->setKeyValueAt(1, 1);
			animation->start(QAbstractAnimation::DeleteWhenStopped);
		}
	}
}

void MainTab::saveAll() {
	for (auto it = resMap.begin(); it != resMap.end(); ++it) {
		it->first->saveChanges();
	}
}

void MainTab::saveRes(Kite::KResource *Res) {
	auto found = resMap.find(Res);
	if (found != resMap.end()) {
		found->first->saveChanges();
	}
}

void MainTab::openTabs(Kite::KResource *Res) {
	auto found = resMap.find(Res);

	if (found == resMap.end()) {
		// scene 
		if (Res->getType() == Kite::RTypes::Scene) {
			auto ind = indexOf(scene);
			setTabText(ind, Res->getName().c_str());
			setCurrentIndex(ind);
		
		// other resources
		} else {
			// serach tab factory
			auto tab = tabFactory.find((size_t)Res->getType());
			if (tab != tabFactory.end()) {

				// create prefred widget
				auto twidget = tab.value()(Res, kinfo, this);
				connect(twidget, &TabWidget::requestResList, this, &MainTab::requestResList);
				connect(twidget, &TabWidget::requestRes, this, &MainTab::requestRes);
				connect(twidget, &TabWidget::requestAddRes, this, &MainTab::requestAddRes);
				connect(twidget, &TabWidget::requestReloadTab, this, &MainTab::reloadRes);
				twidget->inite();
				auto tid = createTab(twidget, Res);
				setCurrentIndex(tid);
				resMap.insert(Res, { twidget, nullptr });
			}
		}
	} else {
		// pined (tab)
		if ((*found).second == nullptr) {
			if (Res->getType() == Kite::RTypes::Scene) { 
				setCurrentIndex(indexOf((*found).first));
			}

		// unpined (dock)
		} else {
			(*found).second->raise();
			auto animation = new QPropertyAnimation((*found).second, "windowOpacity");
			animation->setDuration(1000);
			animation->setKeyValueAt(0, 1);
			animation->setKeyValueAt(0.5, 0.5);
			animation->setKeyValueAt(1, 1);
			animation->start(QAbstractAnimation::DeleteWhenStopped);
		}
	}
}

void MainTab::closeResource(Kite::KResource *Res) {
	if (Res->getType() == Kite::RTypes::Scene) {
		auto ind = indexOf(scene);
		setTabText(ind, "Scene");
		return;
	}

	auto found = resMap.find(Res);
	if (found != resMap.end()) {
		found->first->saveChanges();

		// pined (tab)
		if ((*found).second == nullptr) {
			deleteTab((*found).first);

		// unpined (dock)
		} else {
			deleteDock(found->second);
		}

		resMap.erase(found);
	}
}

void MainTab::reloadRes(Kite::KResource *Res) {
	auto found = resMap.find(Res);
	if (found != resMap.end()) {
		found->first->reload();
	}
}

void MainTab::closeTab() {
	auto act = (QAction *)sender();
	auto resPtr = (Kite::KResource *)act->data().value<void *>();
	auto found = resMap.find(resPtr);

	if (found != resMap.end()) {
		found->first->saveChanges();
		deleteTab((*found).first);
		resMap.erase(found);
	}
}

void MainTab::unpinTab() {
	auto act = (QAction *)sender();
	auto found = resMap.find((Kite::KResource *)act->data().value<void *>());

	if (found != resMap.end()) {
		// remove tab
		auto index = indexOf((*found).first);
		auto frame = tabBar()->tabButton(index, QTabBar::ButtonPosition::RightSide);
		auto editor = (QFrame *)widget(index);
		removeTab(index);
		delete frame;

		// and add editor into a dock widget
		auto dock = new QDockWidget(this->parentWidget());
		dock->installEventFilter(this);
		dock->setStyleSheet("QDockWidget { border: 3px solid; }\n"
							"QDockWidget > QWidget  { border: 1px solid gray; }");
		dock->resize(400, 300);
		dock->setFloating(true);
		dock->setWidget(editor);
		dock->setAllowedAreas(Qt::DockWidgetArea::NoDockWidgetArea);
		(*found).second = dock;

		// dock header widget
		auto dframe = new QFrame(dock);
		dframe->setObjectName("baseFrame");
		dframe->setStyleSheet("#baseFrame { background: rgb(0, 109, 163); }\n");
		auto flayout = new QHBoxLayout(dframe);
		flayout->setMargin(3);

		flayout->addWidget(new QLabel(found.key()->getName().c_str()));

		auto sepBrush = QBrush(Qt::gray, Qt::BrushStyle::Dense6Pattern);
		QPalette sepPalette;
		sepPalette.setBrush(QPalette::Background, sepBrush);

		auto seprator = new QLabel(dock);
		seprator->setAutoFillBackground(true);
		seprator->setPalette(sepPalette);
		seprator->setMaximumHeight(10);
		seprator->setMinimumHeight(10);
		flayout->addWidget(seprator, 1, Qt::AlignVCenter);

		// pin button
		auto pinAction = new QAction(QIcon(":/icons/pin"), "Pin to Tabbar", dock);
		pinAction->setData(qVariantFromValue((void *)found.key()));
		connect(pinAction, &QAction::triggered, this, &MainTab::pinDock);
		auto btnPin = new QToolButton();
		btnPin->setDefaultAction(pinAction);
		btnPin->setIconSize(QSize(8, 8));
		flayout->addWidget(btnPin);

		// close button
		auto closeAction = new QAction(QIcon(":/icons/close"), "Close", dock);
		closeAction->setData(qVariantFromValue((void *)found.key()));
		connect(closeAction, &QAction::triggered, this, &MainTab::closeDock);
		auto btnClose = new QToolButton();
		btnClose->setIconSize(QSize(8, 8));
		btnClose->setDefaultAction(closeAction);
		flayout->addWidget(btnClose);

		dock->setTitleBarWidget(dframe);
		dock->show();
	}
}

void MainTab::pinDock() {
	auto act = (QAction *)sender();
	auto found = resMap.find((Kite::KResource *)act->data().value<void *>());
	auto dock = (QDockWidget *)found->second;
	auto dockHead = (QFrame *)dock->titleBarWidget();
	auto editor = (QFrame *)dock->widget();

	createTab(editor, found.key());
	
	delete dockHead;
	delete dock;
	found->second = nullptr;
}

void MainTab::closeDock() {
	auto act = (QAction *)sender();
	auto resPtr = (Kite::KResource *)act->data().value<void *>();
	auto found = resMap.find(resPtr);
	if (found != resMap.end()) {
		found->first->saveChanges();
		// save code to KScript object
		deleteDock(found->second);
		resMap.erase(found);
	}
}