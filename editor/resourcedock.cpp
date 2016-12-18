#include "resourcedock.h"
#include <QtWidgets>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <Kite/core/kscene.h>
#include <Kite/core/kfistream.h>
#include <Kite/core/kfostream.h>
#include <Kite/logic/klogic.h>
#include <Kite/core/kresource.h>
#include <Kite/core/kresourcemanager.h>
#include <Kite/meta/kmetaclass.h>
#include <kmeta.khgen.h>
#include "frmnewres.h"
#include "atlaseditor.h"
#include "shprogeditor.h"
#include "texturearrayeditor.h"
#include "mapeditor.h"

ResourceDock::ResourceDock(QWidget *Parrent) :
	QDockWidget("Resource Explorer", Parrent),
	fwatcher(new QFileSystemWatcher(this))
{
	setObjectName("Resources");
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setStyleSheet("QDockWidget { border: 3px solid; }");
	setMinimumWidth(120);
	setFocusPolicy(Qt::StrongFocus);

	setupTree();
	setupActions();
	setupHTools();
	setupCategories();
	this->setDisabled(true);
}

ResourceDock::~ResourceDock() {}

void ResourceDock::setupTree() {
	resTree = new QTreeWidget(this);
	resTree->setDragEnabled(false);
	resTree->setAcceptDrops(false);
	resTree->setDragDropMode(QAbstractItemView::NoDragDrop);
	resTree->setColumnCount(1);
	resTree->setHeaderHidden(true);
	resTree->setSelectionMode(QAbstractItemView::SingleSelection);
	resTree->setContextMenuPolicy(Qt::CustomContextMenu);
	setWidget(resTree);

	connect(resTree, &QTreeWidget::itemDoubleClicked, this, &ResourceDock::dblClickedAct);
	connect(resTree, &QTreeWidget::itemSelectionChanged, this, &ResourceDock::clickedAct);
	connect(resTree, &QTreeWidget::customContextMenuRequested, this, &ResourceDock::rightClickedAct);
}

void ResourceDock::setupCategories() {
	// create resources tree
	resTree->clear();
	for (size_t i = 0; i < (size_t)Kite::RTypes::maxSize; ++i) {
		// 
		auto cat = new QTreeWidgetItem(resTree);
		cat->setText(0, Kite::getRTypesName((Kite::RTypes)i).c_str());
		cat->setIcon(0, QIcon(":/icons/folder"));

		// formats
		QPair<QString, QString> fpair;
		if (i == (size_t)Kite::RTypes::Scene) {
			fpair.first = ".sce";
			fpair.second = ":/icons/sce";
			resInfoMap[i].resFI.append(fpair);

		} else if (i == (size_t)Kite::RTypes::Script) {
			fpair.first = ".lua";
			fpair.second = ":/icons/scr";
			resInfoMap[i].resFI.append(fpair);

		} else if (i == (size_t)Kite::RTypes::Prefab) {
			fpair.first = ".pre";
			fpair.second = ":/icons/pre";
			resInfoMap[i].resFI.append(fpair);

		} else if (i == (size_t)Kite::RTypes::Texture) {
			fpair.first = ".tex";
			fpair.second = ":/icons/texture";
			resInfoMap[i].resFI.append(fpair);

		} else if (i == (size_t)Kite::RTypes::Shader) {
			fpair.first = ".vert";
			fpair.second = ":/icons/vshader";
			resInfoMap[i].resFI.append(fpair);

			fpair.first = ".frag";
			fpair.second = ":/icons/fshader";
			resInfoMap[i].resFI.append(fpair);

			fpair.first = ".geom";
			fpair.second = ":/icons/gshader";
			resInfoMap[i].resFI.append(fpair);

		} else if (i == (size_t)Kite::RTypes::ShaderProgram) {
			fpair.first = ".shp";
			fpair.second = ":/icons/shaderp";
			resInfoMap[i].onResRemove = SHProgDepChecker::onResRemove;
			resInfoMap[i].resFI.append(fpair);
		} else if (i == (size_t)Kite::RTypes::AtlasTexture) {
			fpair.first = ".atx";
			fpair.second = ":/icons/atlas";
			resInfoMap[i].onResRemove = AtlasDepChecker::onResRemove;
			resInfoMap[i].resFI.append(fpair);
		} else if (i == (size_t)Kite::RTypes::TextureGroup) {
			fpair.first = ".txg";
			fpair.second = ":/icons/layer";
			resInfoMap[i].onResRemove = TextureArrayDepChecker::onResRemove;
			resInfoMap[i].resFI.append(fpair);
		} else if (i == (size_t)Kite::RTypes::OrthogonalMap) {
			fpair.first = ".map";
			fpair.second = ":/icons/ortho";
			resInfoMap[i].onResRemove = OrthoMapDepChecker::onResRemove;
			resInfoMap[i].resFI.append(fpair);
		} else {
			fpair.first = "";
			fpair.second = ":/icons/new";
			resInfoMap[i].resFI.append(fpair);
		}
	}
}

void ResourceDock::setupActions() {
	actAddNew = new QAction(QIcon(":/icons/add"), "Add New Resource", this);
	actAddNew->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	actAddNew->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(actAddNew, &QAction::triggered, this, &ResourceDock::addNewAct);
	this->addAction(actAddNew);

	actAddExist = new QAction(QIcon(":/icons/open"), "Add Existing Resource", this);
	actAddExist->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
	actAddExist->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(actAddExist, &QAction::triggered, this, &ResourceDock::addExistAct);
	this->addAction(actAddExist);

	actEdit = new QAction(QIcon(":/icons/resEdit"), "Edit", this);
	actEdit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
	actEdit->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(actEdit, &QAction::triggered, this, &ResourceDock::editAct);
	this->addAction(actEdit);

	actRemove = new QAction(QIcon(":/icons/close"), "Remove", this);
	actRemove->setShortcut(QKeySequence(Qt::Key_Delete));
	actRemove->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(actRemove, &QAction::triggered, this, &ResourceDock::removeAct);
	this->addAction(actRemove);
}

void ResourceDock::setupHTools() {
	auto htools = new QFrame(this);
	auto vlayout = new QVBoxLayout(htools);
	vlayout->setMargin(2);
	vlayout->setSpacing(0);

	auto hlayoutTitle = new QHBoxLayout();
	auto title = new QLabel(htools);
	title->setText("Resource Explorer ");
	title->setStyleSheet("color: lightGray;");
	hlayoutTitle->addWidget(title);

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
	vlayout->addSpacing(2);

	auto hlayoutTools = new QHBoxLayout();
	hlayoutTools->setMargin(0);
	hlayoutTools->setSpacing(0);

	auto btnCollAll = new QToolButton(htools);
	btnCollAll->setIcon(QIcon(":/icons/col"));
	btnCollAll->setToolTip("Collapse All");
	btnCollAll->setToolButtonStyle(Qt::ToolButtonIconOnly);
	connect(btnCollAll, &QToolButton::clicked, resTree, &QTreeWidget::collapseAll);
	hlayoutTools->addWidget(btnCollAll);
	hlayoutTools->addSpacing(5);

	auto ledit = new QLineEdit(htools);
	ledit->setPlaceholderText("Search");
	ledit->addAction(QIcon(":/icons/search"), QLineEdit::ActionPosition::TrailingPosition);
	ledit->setStyleSheet("background-color: gray;");
	connect(ledit, &QLineEdit::textChanged, this, &ResourceDock::searchAct);
	hlayoutTools->addWidget(ledit, 1);
	vlayout->addLayout(hlayoutTools);

	htools->setLayout(vlayout);

	setTitleBarWidget(htools);
}

void ResourceDock::initePanel() {
	resTree->blockSignals(false);
	this->setDisabled(false);
}

void ResourceDock::clearPanel() {
	resMap.clear();
	resTree->clear();
	for (size_t i = 0; i < (size_t)Kite::RTypes::maxSize; ++i) {
		auto cat = new QTreeWidgetItem(resTree);
		cat->setText(0, Kite::getRTypesName((Kite::RTypes)i).c_str());
		cat->setIcon(0, QIcon(":/icons/folder"));
	}

	// reset file watcher
	//fwatcher->removePaths(fpaths);
	//fpaths.clear();

	resTree->blockSignals(true);
	this->setDisabled(true);
}

void ResourceDock::dump(QList<DumpItem> &List){
	for (auto it = resMap.begin(); it != resMap.end(); ++it) {
		List.append(DumpItem(it.key(), it->first, it->second));
	}
}

void ResourceDock::filter(Kite::RTypes Type, QStringList &List) {
	List.clear();
	for (auto it = resMap.begin(); it != resMap.end(); ++it) {
		if (it.value().second == Type) {
			List.append(it.key());
		}
	}
}

void ResourceDock::select(const QString &Name) {
	auto found = resTree->findItems(Name, Qt::MatchFlag::MatchRecursive);
	if (!found.isEmpty()) {
		resTree->setCurrentItem(found.first());
	}
}

void ResourceDock::manageUsedResource(const QHash<size_t, QVector<Kite::KMetaProperty>> *ResComponents) {
	if (ResComponents->empty()) {
		return;
	}

	for (auto it = resMap.begin(); it != resMap.end(); ++it) {
		if (it.value().second == Kite::RTypes::Scene) {
			Kite::KFIStream stream;
			auto scene = (Kite::KScene *)load(&stream, it.key());
			auto eman = scene->getEManager();

			scene->clearResources();

			auto econtiner = eman->getEntityStorage();
			for (auto eit = econtiner->begin(); eit != econtiner->end(); ++eit) {

				// first we collect scripts from logic components
				std::vector<Kite::KHandle> lcomps;
				eit->getScriptComponents(lcomps);
				for (auto sit = lcomps.begin(); sit != lcomps.end(); ++sit) {
					auto script = (Kite::KLogicCom *)eit->getComponentByHandle((*sit));
					if (!script->getScript().str.empty()) {
						scene->addResource(script->getScript().str, Kite::RTypes::Script);
					}
				}

				// then we collect resources from other components
				for (auto cit = ResComponents->begin(); cit != ResComponents->end(); ++cit) {
					if (cit.key() == (size_t)Kite::CTypes::Logic) {
						continue;
					}

					if (eit->hasComponent((Kite::CTypes)cit.key())) {
						for (auto pit = cit->begin(); pit != cit->end(); ++pit) {
							auto com = eit->getComponent((Kite::CTypes)cit.key());
							auto res = com->getProperty(pit->name);
							std::string resName;
							if (pit->typeName == "std::string") {
								resName = res.as<std::string>();
							} else if (pit->typeName == "KStringID") {
								resName = res.as<Kite::KStringID>().str;
							}
							
							if (!resName.empty()) {
								scene->addResource(resName, pit->resType);
							}
						}
					}
				}
			}
		}
	}
}

bool ResourceDock::addExisting(const QString &Address, Kite::RTypes Type, bool WarnExist) {
	// is exist?
	QFileInfo file(Address);
	if (resMap.find(file.fileName()) != resMap.end()) {
		if (WarnExist) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("This resource is already exist.\naddress: " + Address +
						"\ntype: " + QString(Kite::getRTypesName(Type).c_str()));
			msg.exec();
			return false;
		} else {
			return true;
		}
	}

	// check file 
	if (!file.isReadable()) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("couldn't add resource. this file is not readable.\naddress: " + Address +
					"\ntype: " + QString(Kite::getRTypesName(Type).c_str()));
		msg.exec();
		return false;
	}

	// add to tree
	auto pitem = resTree->findItems(Kite::getRTypesName(Type).c_str(), Qt::MatchFlag::MatchRecursive).first();
	auto item = new QTreeWidgetItem(pitem);
	item->setText(0, file.fileName());

	// add to map
	resMap.insert(file.fileName(), qMakePair(Address, Type));

	emit(resAdded(file.fileName(), Type));
	return true;
}

bool ResourceDock::addNew(Kite::RTypes Type, const QString &Name, bool AskName){
	// check name
	if (!AskName) {
		if (resMap.find(Name) != resMap.end()) {
			return false;
		}
	}

	// find category and format
	auto pitem = resTree->findItems(Kite::getRTypesName(Type).c_str(), Qt::MatchFlag::MatchRecursive);
	auto frmt = resInfoMap.find((size_t)Type)->resFI.first().first;
	
	QString text = Name;
	frmnewres *form = nullptr;
	QStringList slist;

	// shaders have severeal types(vert, frag, geo)
	if (Type == Kite::RTypes::Shader) {
		slist.push_back("Vertex");
		slist.push_back("Fragment");
		slist.push_back("Geometry");
		form = new frmnewres(slist, this);

	// single types resources
	} else {
		form = new frmnewres(slist, this);
	}

	while(AskName) {
		// show form
		form->exec();

		// cancel pressed
		if (!form->isOk()) {
			return false;
		}

		text = form->getName();

		// shader types
		if (Type == Kite::RTypes::Shader) {
			if (form->getType() == "Vertex") { frmt = ".vert"; }
			if (form->getType() == "Fragment") { frmt = ".frag"; }
			if (form->getType() == "Geometry") { frmt = ".geom"; }
		}

		// available name
		if (resMap.find(text + frmt) != resMap.end()) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("this name is already exist!");
			msg.exec();
			continue;
		}

		// file validation
		QFile file(resDir + text + frmt);
		if (file.exists()) {
			auto ret = QMessageBox::question(this, "Message",
												"this name is exists in the current resources directory.\ndo you want to replace it?",
												QMessageBox::Yes | QMessageBox::No,
												QMessageBox::No);

			if (ret == QMessageBox::StandardButton::No) {
				file.close();
				continue;
			}
		}

		if (!file.open(QIODevice::ReadWrite)) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("cant create file in the resource directory.");
			msg.exec();
			file.close();
			continue;
		}
		file.close();

		// ok pressed
		if (form->isOk()) {
			text = QFileInfo(file.fileName()).fileName();
			break;
		}
	}

	delete form;

	// create new res
	Kite::KResourceManager rman;
	auto newres = rman.createOnFly(Type, text.toStdString());
	if (newres == nullptr) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("resource create module return nullptr!\nresource type: " + QString(Kite::getRTypesName(Type).c_str()));
		msg.exec();
		return false;
	}

	// save it to the hard disk
	Kite::KFOStream fstream;
	if (!newres->saveStream(fstream, resDir.toStdString() + text.toStdString())) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("cant create resource file.!\nresource type: " + QString(Kite::getRTypesName(Type).c_str()));
		msg.exec();
		delete newres;
		return false;
	}
	delete newres;

	// add to tree
	auto item = new QTreeWidgetItem(pitem.first());
	item->setText(0, text);

	// add to map
	resMap.insert(text, qMakePair(resDir + text, Type));

	emit(resAdded(text, Type));
	return true;
}

Kite::KResource *ResourceDock::load(Kite::KIStream *Stream, const QString &Name) {
	auto found = resMap.find(Name);
	if (found == resMap.end()) return nullptr;

	Kite::KResourceManager rman;
	auto res = rman.loadOnFly(Stream, found->second, found->first.toStdString());
	return res;
}

void ResourceDock::dblClickedAct(QTreeWidgetItem *item, int column) {
	editAct();
}

void ResourceDock::clickedAct() {
	auto item = resTree->currentItem();

	if (item != nullptr) {
		if (item->parent() == nullptr) {
			actAddNew->setText("Add New " + item->text(0));
			actAddExist->setText("Add Existing " + item->text(0));
		} else {
			actEdit->setText("Edit " + item->text(0));
			actRemove->setText("Remove " + item->text(0));
			emit(resSelected(item->text(0), resMap.find(item->text(0))->second));
		}
	}
}

void ResourceDock::rightClickedAct(const QPoint & pos) {
	QTreeWidgetItem *item = resTree->currentItem();
	QMenu cmenu(this);
	QPoint pt(pos);
	pt.setY(pt.y() + 50);
	
	if (item != nullptr) {
		// checking user rclicked on category or items?
		if (item->parent() == nullptr) {
			actAddNew->setText("Add New " + item->text(0));
			actAddExist->setText("Add Existing " + item->text(0));
			cmenu.addAction(actAddNew);
			cmenu.addAction(actAddExist);
			cmenu.exec(mapToGlobal(pt));
		} else {
			actEdit->setText("Edit " + item->text(0));
			actRemove->setText("Remove " + item->text(0));
			cmenu.addAction(actEdit);
			cmenu.addSeparator();
			cmenu.addAction(actRemove);
			cmenu.exec(mapToGlobal(pt));
		}
	}
}

void ResourceDock::addNewAct() {
	auto pitem = resTree->currentItem();
	auto restype = Kite::getRTypesByName(pitem->text(0).toStdString());
	
	// is it type?
	if (pitem->parent() == nullptr) {
		addNew(restype, "", true);
	}
}

void ResourceDock::addExistAct() {
	auto pitem = resTree->currentItem();
	auto restype = Kite::getRTypesByName(pitem->text(0).toStdString());
	auto frmt = resInfoMap.find((size_t)restype);

	if (pitem->parent() == nullptr) {
		QString formats;
		for (auto it = frmt->resFI.begin(); it != frmt->resFI.end(); ++it) {
			formats += " *" + it->first;
		}
		QString fileName = QFileDialog::getOpenFileName(this, "Open " + QString(Kite::getRTypesName(restype).c_str()),
														resDir, "Kite2D Resource File (" + formats + ")");

		if (!fileName.isEmpty()) {
			addExisting(fileName, restype);
		}
	}
}

void ResourceDock::editAct() {
	auto item = resTree->currentItem();
	if (item->parent() != nullptr) {
		// emit edit signal
		emit(resEdit(item->text(0), resMap.find(item->text(0))->second));
	}
}

void ResourceDock::removeAct() {
	auto item = resTree->currentItem();
	if (item->parent() != nullptr) {
		auto found = resMap.find(item->text(0));
		QMessageBox::StandardButton reply;
		QMessageBox msgBox;
		msgBox.setText("Choose Remove to remove \'" + item->text(0) + "\' from project.\n"
					   "Choose Delete to permanently delete \'" + item->text(0) + "\' from project.");
		QAbstractButton *btnRemove = msgBox.addButton("Remove", QMessageBox::YesRole);
		QAbstractButton *btnDelete = msgBox.addButton("Delete", QMessageBox::DestructiveRole);
		QAbstractButton *btnCancel = msgBox.addButton("Cancel", QMessageBox::NoRole);

		msgBox.setIcon(QMessageBox::Question);
		msgBox.exec();

		if (msgBox.clickedButton() == btnCancel) {
			return;
		} else if (msgBox.clickedButton() == btnRemove) {
			emit(resDeleted(found.key(), found->second));
		} else if (msgBox.clickedButton() == btnDelete) {
			emit(resDeleted(found.key(), found->second));
			QFile file(resDir + item->text(0));
			file.remove();
		}
		delete item;
		resMap.erase(found);
	}
}

void ResourceDock::searchAct(const QString &Pharase) {
	auto allItems = QTreeWidgetItemIterator(resTree);
	while (*allItems) {
		if ((*allItems)->parent() != nullptr) {
			if (!Pharase.isEmpty()) {
				if ((*allItems)->text(0).contains(Pharase)) {
					(*allItems)->setHidden(false);
				} else {
					(*allItems)->setHidden(true);
				}
			} else {
				(*allItems)->setHidden(false);
			}
		}
		++allItems;
	}
}