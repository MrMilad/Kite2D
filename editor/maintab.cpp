#include "maintab.h"
#include "shprogeditor.h"
#include "luaeditor.h"
#include "glsleditor.h"
#include "textureeditor.h"
#include "mapeditor.h"
#include "texturearrayeditor.h"
#include "atlaseditor.h"
#include <qundostack.h>
#include <QtWidgets>
#include <qmessagebox.h>
#include <qstandarditemmodel.h>
#include <qstandarditemmodel.h>
#include <kmeta.khgen.h>
#include <Kite/core/kfostream.h>
#include <Kite/core/kfistream.h>

FocusCover::FocusCover(QWidget *par) :
	QWidget(par) {
	setAttribute(Qt::WA_TranslucentBackground);
	par->installEventFilter(this);
	hide();
}

void FocusCover::mousePressEvent(QMouseEvent *Event) {
	hide();
}

bool FocusCover::eventFilter(QObject *Obj, QEvent *Event) {
	if (Event->type() == QEvent::Type::Resize) {
		auto rev = static_cast<QResizeEvent *>(Event);
		resize(rev->size());
	}
	return false;
}

MainTab::MainTab(QUndoGroup *UGroup, KiteInfo *KInfo,QWidget *parent) :
	QTabWidget(parent),
	kinfo(KInfo),
	ugroup(UGroup),
	fcover(new FocusCover(this))
{
	registerTabs();
	setStyleSheet("QTabBar::tab { height: 23px;}");
	setMovable(true);

	connect(this, &QTabWidget::currentChanged, this, &MainTab::tabChanged);
	connect(this, &QTabWidget::tabBarClicked, this, &MainTab::tabChanged);
}

MainTab::~MainTab() {}

bool MainTab::isOpen(const QString &Name) {
	if (resMap.find(Name) == resMap.end()) {
		return false;
	}
	return true;
}

bool MainTab::needSave(const QString &Name) {
	auto found = resMap.find(Name);
	if (found != resMap.end()) {
		if (!found->ustack->isClean()) return true;
	}
	return false;
}

bool MainTab::needSave() {
	for (auto it = resMap.begin(); it != resMap.end(); ++it) {
		if (!it->ustack->isClean()) return true;
	}

	return false;
}

bool MainTab::eventFilter(QObject *Obj, QEvent *Event) {
	// disable ALT+F4 on dock widgets
	if (Event->type() == QEvent::Close) {
		Event->ignore();
		return true;
	}

	// focus-in event
	if (Event->type() == QEvent::FocusIn) {
		auto dock = (QDockWidget *)Obj;
		auto widget = (TabWidget *)dock->widget();
		ugroup->setActiveStack(resMap.find(widget->getResource()->getName().c_str())->ustack);
		fcover->show();
	}

	return false;
}

void MainTab::registerTabs() {
	tabFactory.clear();
	//tabFactory.insert((size_t)Kite::RTypes::Script, LuaEditor::factory);
	//tabFactory.insert((size_t)Kite::RTypes::Shader, GLSLEditor::factory);
	tabFactory.insert((size_t)Kite::RTypes::ShaderProgram, ShProgEditor::factory);
	//tabFactory.insert((size_t)Kite::RTypes::Texture, TextureEditor::factory);
	//tabFactory.insert((size_t)Kite::RTypes::AtlasTexture, AtlasEditor::factory);
	//tabFactory.insert((size_t)Kite::RTypes::OrthogonalMap, MapEditor::factory);
	//tabFactory.insert((size_t)Kite::RTypes::TextureGroup, TextureArrayEditor::factory);
}

int MainTab::createTab(QWidget *Widget, Kite::KResource *ResPtr) {
	auto frame = new QFrame();
	auto flayout = new QHBoxLayout(frame);
	flayout->setMargin(0);
	flayout->setSpacing(3);

	auto closeTab = new QAction(QIcon(":/icons/close"), "Close Tab", frame);
	connect(closeTab, &QAction::triggered, this, &MainTab::closeTabAct);

	auto unpinTab = new QAction(QIcon(":/icons/unpin"), "Unpin Tab", frame);
	connect(unpinTab, &QAction::triggered, this, &MainTab::unpinTabAct);

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
	auto twidget = Dock->widget();
	Dock->close();

	delete twidget;
	delete dockHead;
	delete Dock;
}

void MainTab::select(const QString &Name, Kite::RTypes Type) {
	auto found = resMap.find(Name);
	if (found != resMap.end()) {
		// pined (tab)
		if (found->dock == nullptr) {
			auto ind = indexOf(found->widget);
			setCurrentIndex(ind);

		// unpined (dock)
		} else {
			found->dock->raise();
			auto animation = new QPropertyAnimation (found->dock, "windowOpacity");
			animation->setDuration(1000);
			animation->setKeyValueAt(0, 1);
			animation->setKeyValueAt(0.5, 0.5);
			animation->setKeyValueAt(1, 1);
			animation->start(QAbstractAnimation::DeleteWhenStopped);
		}

		// this function will called in tab changed or focus changed
		//ugroup->setActiveStack(found->ustack);
	}
}

void MainTab::saveAll() {
	bool succes = true;
	for (auto it = resMap.begin(); it != resMap.end(); ++it) {
		if (!it->ustack->isClean()) {
			it->widget->saveChanges();
			Kite::KFOStream fstream;
			if (!it->widget->getResource()->saveStream(fstream,
													   it->widget->getResource()->getAddress()
														  + "/" + it->widget->getResource()->getName())) {
				succes = false;
			}
			it->ustack->setClean();
		}
	}

	if (!succes) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("save operation was not successful.");
		msg.exec();
	}
}

void MainTab::save(const QString &Name) {
	auto found = resMap.find(Name);
	if (found != resMap.end()) {
		if (!found->ustack->isClean()) {
			found->widget->saveChanges();
			Kite::KFOStream fstream;
			if (!found->widget->getResource()->saveStream(fstream,
														  found->widget->getResource()->getAddress()
													   + "/" + found->widget->getResource()->getName())) {
				QMessageBox msg;
				msg.setWindowTitle("Message");
				msg.setText("save operation was not successful.");
				msg.exec();
			}
			found->ustack->setClean();
		}
	}
}

void MainTab::open(const QString &Name, Kite::RTypes Type) {
	auto found = resMap.find(Name);

	if (found == resMap.end()) {
		// serach tab factory
		auto found = tabFactory.find((size_t)Type);
		if (found != tabFactory.end()) {
			auto stream = new Kite::KFIStream;
			auto res = reqResLoad(stream, Name);

			// create undo stack
			auto ustack = new QUndoStack(ugroup);
			ustack->setObjectName(Name);
			connect(ustack, &QUndoStack::cleanChanged, this, &MainTab::cleanChanged);

			// create prefred widget
			auto twidget = found.value()(res, stream, ustack, kinfo, this);
			connect(twidget, &TabWidget::reqResList, this, &MainTab::reqResList);
			connect(twidget, &TabWidget::reqResLoad, this, &MainTab::reqResLoad);
			connect(twidget, &TabWidget::reqReloadName, this, &MainTab::reload);
			connect(twidget, &TabWidget::reqReloadType, this, &MainTab::reloadType);
			twidget->inite();

			// create tab
			auto tid = createTab(twidget, res);
			setCurrentIndex(tid);

			// insert to map
			resMap.insert(Name, TabInfo(twidget, nullptr, ustack));

			// set as active undo stack
			ugroup->setActiveStack(ustack);

			emit(tabSelected(Name));
		}
	}
}

void MainTab::close(const QString &Name, Kite::RTypes Type) {
	// close and remove resource editor
	// we dont try to saving changes here
	auto found = resMap.find(Name);
	if (found != resMap.end()) {
		// pined (tab)
		if (found->dock == nullptr) {
			deleteTab(found->widget);

		// unpined (dock)
		} else {
			deleteDock(found->dock);
		}

		found->ustack->clear();
		delete found->ustack;
		resMap.erase(found);
	}
}

void MainTab::resourceAdded(const QString &Name, Kite::RTypes Type) {
	// update all open tabs
	for (auto it = resMap.begin(); it != resMap.end(); ++it) {
		it->widget->onAddRes(Name, Type);
	}
}

void MainTab::resourceDeleted(const QString &Name, Kite::RTypes Type) {
	// update all open editors
	for (auto it = resMap.begin(); it != resMap.end(); ++it) {
		it->widget->onRemoveRes(Name, Type);
	}
}

void MainTab::reload(const QString &Name) {
	auto found = resMap.find(Name);
	if (found != resMap.end()) {
		found->widget->reload();
		found->ustack->clear();
	}
}

void MainTab::reloadType(Kite::RTypes Type) {
	for (auto it = resMap.begin(); it != resMap.end(); ++it) {
		if (it->widget->getResource()->getType() == Type) {
			it->widget->reload();
			it->ustack->clear();
		}
	}
}

void MainTab::closeTabAct() {
	auto act = (QAction *)sender();
	auto resPtr = (Kite::KResource *)act->data().value<void *>();
	auto found = resMap.find(resPtr->getName().c_str());

	if (found != resMap.end()) {
		// check if need save
		if (!found->ustack->isClean()) {
			int ret = QMessageBox::warning(this, "Kite2D Editor",
										   "Do you want to save changes to " + QString(found->widget->getResource()->getName().c_str()) + "?",
										   QMessageBox::Save | QMessageBox::Discard
										   | QMessageBox::Cancel,
										   QMessageBox::Save);

			// save all changes to resource file
			if (ret == QMessageBox::Save) {
				found->widget->saveChanges();
				Kite::KFOStream fstream;
				if (!found->widget->getResource()->saveStream(fstream,
															  found->widget->getResource()->getAddress() 
															  + "/" + found->widget->getResource()->getName())) {
					QMessageBox msg;
					msg.setWindowTitle("Message");
					msg.setText("save operation was not successful");
					msg.exec();
				}

			} else if (ret == QMessageBox::Cancel) {
				return;
			}
		}

		deleteTab(found->widget);

		delete found->ustack;
		resMap.erase(found);
	}
}

void MainTab::unpinTabAct() {
	auto act = (QAction *)sender();
	auto res = (Kite::KResource *)act->data().value<void *>();
	auto found = resMap.find(res->getName().c_str());

	if (found != resMap.end()) {
		// remove tab
		auto index = indexOf(found->widget);
		auto frame = tabBar()->tabButton(index, QTabBar::ButtonPosition::RightSide);
		auto editor = (QFrame *)widget(index);
		removeTab(index);
		delete frame;

		// and add editor into a dock widget
		auto dock = new QDockWidget(this->parentWidget());
		dock->setObjectName("dock");
		dock->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
		dock->installEventFilter(this);
		dock->setStyleSheet("QDockWidget { border: 3px solid; }\n"
							"QDockWidget > QWidget  { border: 1px solid gray; }");
		dock->resize(400, 300);
		dock->setFloating(true);
		dock->setWidget(editor);
		dock->setAllowedAreas(Qt::DockWidgetArea::NoDockWidgetArea);
		found->dock = dock;

		// dock header widget
		auto dframe = new QFrame(dock);
		dframe->setObjectName("baseFrame");
		dframe->setStyleSheet("#baseFrame { background: rgb(0, 109, 163); }\n");
		auto flayout = new QHBoxLayout(dframe);
		flayout->setMargin(3);

		auto title = new QLabel(found->widget->getResource()->getName().c_str());
		title->setObjectName("title");
		flayout->addWidget(title);

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
		pinAction->setData(qVariantFromValue((void *)found->widget->getResource()));
		connect(pinAction, &QAction::triggered, this, &MainTab::pinDockAct);
		auto btnPin = new QToolButton();
		btnPin->setDefaultAction(pinAction);
		btnPin->setIconSize(QSize(8, 8));
		flayout->addWidget(btnPin);

		// close button
		auto closeAction = new QAction(QIcon(":/icons/close"), "Close", dock);
		closeAction->setData(qVariantFromValue((void *)found->widget->getResource()));
		connect(closeAction, &QAction::triggered, this, &MainTab::closeDockAct);
		auto btnClose = new QToolButton();
		btnClose->setIconSize(QSize(8, 8));
		btnClose->setDefaultAction(closeAction);
		flayout->addWidget(btnClose);

		dock->setTitleBarWidget(dframe);
		dock->show();
	}
}

void MainTab::tabChanged(int index) {
	if (index >= 0) {
		auto widget = (TabWidget *)this->widget(index);
		auto found = resMap.find(widget->getResource()->getName().c_str());
		if (found != resMap.end()) {
			ugroup->setActiveStack(found->ustack);
		}

	// empty tabbar
	} else {
		ugroup->setActiveStack(nullptr);
	}
}

void MainTab::cleanChanged(bool clean) {
	auto resName = sender()->objectName();
	auto found = resMap.find(resName);
	if (clean) {
		if (found->dock) {
			auto title = found->dock->findChild<QLabel *>("title");
			title->setText(resName);
		} else {
			setTabText(indexOf(found->widget), resName);
		}
	} else {
		if (found->dock) {
			auto title = found->dock->findChild<QLabel *>("title");
			title->setText(resName + "*");
		} else {
			setTabText(indexOf(found->widget), resName + "*");
		}
	}
}

void MainTab::pinDockAct() {
	auto act = (QAction *)sender();
	auto res = (Kite::KResource *)act->data().value<void *>();
	auto found = resMap.find(res->getName().c_str());
	auto dock = (QDockWidget *)found->dock;
	auto dockHead = (QFrame *)dock->titleBarWidget();
	auto editor = (QFrame *)dock->widget();

	createTab(editor, (Kite::KResource *)act->data().value<void *>());
	
	delete dockHead;
	delete dock;
	found->dock = nullptr;
}

void MainTab::closeDockAct() {
	auto act = (QAction *)sender();
	auto resPtr = (Kite::KResource *)act->data().value<void *>();
	auto found = resMap.find(resPtr->getName().c_str());
	if (found != resMap.end()) {
		// check if need save
		if (!found->ustack->isClean()) {
			int ret = QMessageBox::warning(this, "Kite2D Editor",
										   "Do you want to save changes to " + QString(found->widget->getResource()->getName().c_str()) + "?",
										   QMessageBox::Save | QMessageBox::Discard
										   | QMessageBox::Cancel,
										   QMessageBox::Save);

			// save all changes to resource
			if (ret == QMessageBox::Save) {
				found->widget->saveChanges();
				Kite::KFOStream fstream;
				if (!found->widget->getResource()->saveStream(fstream,
															  found->widget->getResource()->getAddress()
															  + "/" + found->widget->getResource()->getName())) {
					QMessageBox msg;
					msg.setWindowTitle("Message");
					msg.setText("save operation was not successful");
					msg.exec();
				}
			} else if (ret == QMessageBox::Cancel) {
				return;
			}
		}

		deleteDock(found->dock);
		delete found->ustack;
		resMap.erase(found);
	}
}