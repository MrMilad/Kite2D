#include "maintab.h"
#include <QtWidgets>
#include <codeeditor.h>
#include <qpropertyanimation.h>
#include <kmeta.khgen.h>

MainTab::MainTab(QWidget *parent) :
	QTabWidget(parent) , cmodel(nullptr)
{
	setStyleSheet("QTabBar::tab { height: 23px; }");
	setMovable(true);
}

MainTab::~MainTab() {}

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
	btnUnpin->setFixedSize(15, 15);
	btnUnpin->setAutoRaise(true);
	btnUnpin->setDefaultAction(unpinTab);
	unpinTab->setParent(btnUnpin);
	flayout->addWidget(btnUnpin);

	auto btnClose = new QToolButton();
	btnClose->setFixedSize(15, 15);
	btnClose->setAutoRaise(true);
	btnClose->setDefaultAction(closeTab);
	closeTab->setParent(btnClose);
	flayout->addWidget(btnClose);

	frame->setLayout(flayout);

	closeTab->setData(qVariantFromValue((void *) ResPtr));
	unpinTab->setData(qVariantFromValue((void *)ResPtr));

	auto tid = addTab(Widget, ResPtr->getResourceName().c_str());
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
		if (Res->getResourceType() == "KScene") {
			setTabText(0, Res->getResourceName().c_str());
			setCurrentIndex(0);
		}

	} else {
		if (Res->getResourceType() == "KScript") {
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
}

void MainTab::saveAll() {
	for (auto it = resMap.begin(); it != resMap.end(); ++it) {
		// KScript
		if (it.key()->getResourceType() == "KScript") {
			auto sc = (Kite::KScript *)it.key();
			auto ceditor = (CodeEditor *)it->first;
			sc->setCode(ceditor->document()->toPlainText().toStdString());
		}
	}
}

void MainTab::openTabs(Kite::KResource *Res) {
	auto found = resMap.find(Res);

	if (found == resMap.end()) {
		if (Res->getResourceType() == "KScript") {
			auto script = (Kite::KScript *)Res;
			auto ceditor = new CodeEditor(this);
			ceditor->setPlainText(script->getCode().c_str());
			static bool ini = false;
			if (!ini && cmodel != nullptr) {
				CodeEditor::setCompleterModel(cmodel);
				ini = true;
			}

			auto tid = createTab(ceditor, Res);
			setCurrentIndex(tid);
			resMap.insert(Res, { ceditor, nullptr });
		}

		if (Res->getResourceType() == "KScene") {
			setTabText(0, Res->getResourceName().c_str());
			setCurrentIndex(0);
		}

	} else {
		// pined (tab)
		if ((*found).second == nullptr) {
			if (Res->getResourceType() == "KScript") {
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
	auto found = resMap.find(Res);

	if (found != resMap.end()) {
		// pined (tab)
		if ((*found).second == nullptr) {
			if (Res->getResourceType() == "KScript") {
				deleteTab((*found).first);
			}

		// unpined (dock)
		} else {
			deleteDock(found->second);
		}

		resMap.erase(found);
	}
}

void MainTab::closeTab() {
	auto act = (QAction *)sender();
	auto resPtr = (Kite::KResource *)act->data().value<void *>();
	auto found = resMap.find(resPtr);

	if (found != resMap.end()) {
		// save code to KScript object
		if (resPtr->getResourceType() == "KScript") {
			auto sc = (Kite::KScript *)resPtr;
			auto ceditor = (CodeEditor *)found->first;
			sc->setCode(ceditor->document()->toPlainText().toStdString());
		}

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
		auto editor = widget(index);
		removeTab(index);
		delete frame;

		// and add editor into a dock widget
		auto dock = new QDockWidget(this->parentWidget());
		dock->resize(400, 300);
		dock->setFloating(true);
		dock->setWidget(editor);
		dock->setAllowedAreas(Qt::DockWidgetArea::NoDockWidgetArea);
		(*found).second = dock;

		// dock header widget
		auto dframe = new QFrame(dock);
		auto flayout = new QHBoxLayout(dframe);
		flayout->setMargin(3);

		flayout->addWidget(new QLabel(found.key()->getResourceName().c_str()));

		// pin button
		auto pinAction = new QAction(QIcon(":/icons/pin"), "Pin to Tabbar", dock);
		pinAction->setData(qVariantFromValue((void *)found.key()));
		connect(pinAction, &QAction::triggered, this, &MainTab::pinDock);
		auto btnPin = new QToolButton();
		btnPin->setFixedSize(15, 15);
		btnPin->setAutoRaise(true);
		btnPin->setDefaultAction(pinAction);
		flayout->addWidget(btnPin);

		// close button
		auto closeAction = new QAction(QIcon(":/icons/close"), "Close", dock);
		closeAction->setData(qVariantFromValue((void *)found.key()));
		connect(closeAction, &QAction::triggered, this, &MainTab::closeDock);
		auto btnClose = new QToolButton();
		btnClose->setFixedSize(15, 15);
		btnClose->setAutoRaise(true);
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
	auto editor = (CodeEditor *)dock->widget();

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
		// save code to KScript object
		if (resPtr->getResourceType() == "KScript") {
			auto sc = (Kite::KScript *)resPtr;
			auto ceditor = (CodeEditor *)found->first;
			sc->setCode(ceditor->document()->toPlainText().toStdString());
		}
		deleteDock(found->second);
		resMap.erase(found);
	}
}