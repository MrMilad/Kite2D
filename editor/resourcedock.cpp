#include "resourcedock.h"
#include <QtWidgets>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include "frmnewres.h"
#include <Kite/core/kscene.h>
#include <Kite/core/kfistream.h>
#include <Kite/core/kfostream.h>
#include <Kite/logic/klogic.h>
#include <kmeta.khgen.h>

ResourceDock::ResourceDock(QWidget *Parrent) :
	QDockWidget("Resource Explorer", Parrent)
{
	setObjectName("Resources");
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setStyleSheet("QDockWidget { border: 3px solid; }");
	setMinimumWidth(120);
	setFocusPolicy(Qt::StrongFocus);

	setupTree();
	setupActions();
	actionsControl(AS_ON_INITE);
	setupHTools();
	setupCategories();
}

ResourceDock::~ResourceDock() {}

Kite::RecieveTypes ResourceDock::onMessage(Kite::KMessage *Message, Kite::MessageScope Scope) {
	if (Message->getType() == "RES_LOAD") {
		// retrive resource pointer
		auto res = (Kite::KResource *)Message->getData();

		// add it to tree
		auto icons = formats.values((size_t)res->getType());
		auto pitem = resTree->findItems(res->getTypeName().c_str(), Qt::MatchFlag::MatchRecursive).first();
		auto item = new QTreeWidgetItem(pitem);
		item->setText(0, res->getName().c_str());

		// single format
		if (icons.size() == 1) {
			item->setIcon(0, QIcon(icons.first().second));

		// multi format
		} else {
			auto format = res->getName().substr(res->getName().find_last_of("."));
			for (auto it = icons.begin(); it != icons.end(); ++it) {
				if (it->first.toStdString() == format) {
					item->setIcon(0, QIcon(it->second));
					break;
				}
			}
		}
		pitem->setExpanded(true);

		// emit corresponding signal
		emit(resourceAdded(res));
		return Kite::RecieveTypes::RECEIVED;

	} else if (Message->getType() == "RES_UNLOAD") {
		auto res = (Kite::KResource *)Message->getData();
		emit(resourceDelete(res));
		return Kite::RecieveTypes::RECEIVED;
	}
	return Kite::RecieveTypes::IGNORED;
}

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

	connect(resTree, &QTreeWidget::itemDoubleClicked, this, &ResourceDock::actDoubleClicked);
	connect(resTree, &QTreeWidget::itemSelectionChanged, this, &ResourceDock::actClicked);
	connect(resTree, &QTreeWidget::customContextMenuRequested, this, &ResourceDock::actRClicked);
}

void ResourceDock::setupCategories() {
	// register resource types
	registerRTypes(&rman);

	// subscribe to resource load\unload
	rman.subscribe(*this, "RES_LOAD");
	rman.subscribe(*this, "RES_UNLOAD");

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
			formats.insert(i, fpair);

		} else if (i == (size_t)Kite::RTypes::Script) {
			fpair.first = ".lua";
			fpair.second = ":/icons/scr";
			formats.insert(i, fpair);

		} else if (i == (size_t)Kite::RTypes::Prefab) {
			fpair.first = ".pre";
			fpair.second = ":/icons/pre";
			formats.insert(i, fpair);

		} else if (i == (size_t)Kite::RTypes::Texture) {
			fpair.first = ".tex";
			fpair.second = ":/icons/texture";
			formats.insert(i, fpair);

		} else if (i == (size_t)Kite::RTypes::Shader) {
			fpair.first = ".vert";
			fpair.second = ":/icons/vshader";
			formats.insert(i, fpair);

			fpair.first = ".frag";
			fpair.second = ":/icons/fshader";
			formats.insert(i, fpair);

			fpair.first = ".geom";
			fpair.second = ":/icons/gshader";
			formats.insert(i, fpair);

		} else if (i == (size_t)Kite::RTypes::ShaderProgram) {
			fpair.first = ".shp";
			fpair.second = ":/icons/shaderp";
			formats.insert(i, fpair);
		} else if (i == (size_t)Kite::RTypes::AtlasTexture) {
			fpair.first = ".atx";
			fpair.second = ":/icons/atlas";
			formats.insert(i, fpair);
		} else {
			fpair.first = "";
			fpair.second = ":/icons/new";
			formats.insert(i, fpair);
		}
	}
}

void ResourceDock::setupActions() {
	addRes = new QAction(QIcon(":/icons/add"), "Add New Resource", this);
	addRes->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	addRes->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(addRes, &QAction::triggered, this, &ResourceDock::actAdd);
	this->addAction(addRes);

	openRes = new QAction(QIcon(":/icons/open"), "Add Existing Resource", this);
	openRes->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
	openRes->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(openRes, &QAction::triggered, this, &ResourceDock::actOpen);
	this->addAction(openRes);

	saveRes = new QAction(QIcon(":/icons/save"), "Save Resource", this);
	saveRes->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
	saveRes->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(saveRes, &QAction::triggered, this, &ResourceDock::actSave);
	this->addAction(saveRes);

	saveAsRes = new QAction(QIcon(":/icons/saveAs"), "Save Resource As", this);
	saveAsRes->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
	saveAsRes->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(saveAsRes, &QAction::triggered, this, &ResourceDock::actSaveAs);
	this->addAction(saveAsRes);

	editRes = new QAction(QIcon(":/icons/resEdit"), "Edit", this);
	editRes->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
	editRes->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(editRes, &QAction::triggered, this, &ResourceDock::actEdit);
	this->addAction(editRes);

	remRes = new QAction(QIcon(":/icons/close"), "Remove", this);
	remRes->setShortcut(QKeySequence(Qt::Key_Delete));
	remRes->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(remRes, &QAction::triggered, this, &ResourceDock::actRemove);
	this->addAction(remRes);
}

void ResourceDock::setupHTools() {
	htools = new QFrame(this);
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

	auto hlayout = new QHBoxLayout(htools);
	hlayout->setMargin(3);
	//hlayout->setSizeConstraint(QLayout::SetFixedSize);

	auto btnAdd = new QToolButton(htools);
	btnAdd->setDefaultAction(addRes);
	btnAdd->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnAdd);

	auto btnOpen = new QToolButton(htools);
	btnOpen->setDefaultAction(openRes);
	btnOpen->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnOpen);

	hlayout->addSpacing(10);

	auto btnEdit = new QToolButton(htools);
	btnEdit->setDefaultAction(editRes);
	btnEdit->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnEdit);

	auto btnRemove = new QToolButton(htools);
	btnRemove->setDefaultAction(remRes);
	btnRemove->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnRemove);

	hlayout->addStretch(1);
	vlayout->addLayout(hlayout);

	ledit = new QLineEdit(htools);
	ledit->setPlaceholderText("Search");
	ledit->addAction(QIcon(":/icons/search"), QLineEdit::ActionPosition::TrailingPosition);
	ledit->setStyleSheet("background-color: gray;");
	connect(ledit, &QLineEdit::textChanged, this, &ResourceDock::actSearch);
	
	vlayout->addWidget(ledit);

	htools->setLayout(vlayout);

	setTitleBarWidget(htools);
}

void ResourceDock::actionsControl(ActionsState State) {
	if (State == AS_ON_CAT) {
		addRes->setDisabled(false);
		openRes->setDisabled(false);
		saveRes->setDisabled(true);
		saveAsRes->setDisabled(true);
		editRes->setDisabled(true);
		remRes->setDisabled(true);
	} else if (State == AS_ON_ITEM) {
		addRes->setDisabled(true);
		openRes->setDisabled(true);
		saveRes->setDisabled(false);
		saveAsRes->setDisabled(false);
		editRes->setDisabled(false);
		remRes->setDisabled(false);
	} else if (State == AS_ON_INITE) {
		addRes->setDisabled(true);
		openRes->setDisabled(true);
		saveRes->setDisabled(true);
		saveAsRes->setDisabled(true);
		editRes->setDisabled(true);
		remRes->setDisabled(true);
	}
}

void ResourceDock::clearResources() {
	resTree->clear();
	rman.clear();

	// reset tree
	for (size_t i = 0; i < (size_t)Kite::RTypes::maxSize; ++i) {
		auto cat = new QTreeWidgetItem(resTree);
		cat->setText(0, Kite::getRTypesName((Kite::RTypes)i).c_str());
		cat->setIcon(0, QIcon(":/icons/folder"));
	}
}

void ResourceDock::resetModify() {
	auto rlist = rman.dump();
	for (auto it = rlist.begin(); it != rlist.end(); ++it) {
		(*it)->isModified();
	}
}

const std::vector<Kite::KResource *> &ResourceDock::dumpResource() {
	return rman.dump();
}

void ResourceDock::filterByType(Kite::RTypes Type, QStringList &List) {
	List.clear();
	auto dumpList = rman.dump();
	for (auto it = dumpList.begin(); it != dumpList.end(); ++it) {
		if ((*it)->getType() == Type) {
			List.append((*it)->getName().c_str());
		}
	}
}

void ResourceDock::filterByTypeRes(Kite::RTypes Type, QList<const Kite::KResource *> &List) {
	List.clear();
	auto dumpList = rman.dump();
	for (auto it = dumpList.begin(); it != dumpList.end(); ++it) {
		if ((*it)->getType() == Type) {
			List.push_back((*it));
		}
	}
}

void ResourceDock::selectResource(const QString &Name) {
	auto found = resTree->findItems(Name, Qt::MatchFlag::MatchRecursive);
	if (!found.isEmpty()) {
		resTree->setCurrentItem(found.first());
	}
}

void ResourceDock::manageUsedResource(const QHash<size_t, QVector<Kite::KMetaProperty>> *ResComponents) {
	if (ResComponents->empty()) {
		return;
	}

	auto dumpList = rman.dump();
	for (auto it = dumpList.begin(); it != dumpList.end(); ++it) {
		if ((*it)->getType() == Kite::RTypes::Scene) {
			auto scene = (Kite::KScene *)(*it);
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

bool ResourceDock::openResource(const QString &Address, Kite::RTypes Type, bool WarnExist) {
	// check name
	QFileInfo file(Address);
	if (rman.get(Address.toStdString()) != nullptr) {
		if (WarnExist) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("This file is already exist.\nType: " + QString(Kite::getRTypesName(Type).c_str()) + "\nAddress: " + Address);
			msg.exec();
			return false;
		} else {
			return true;
		}
	}

	// load resource 
	auto res = rman.load(Kite::IStreamTypes::FIStream, Type, Address.toStdString());
	if (res == nullptr) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("Resource add module return nullptr!\nresource type: " + QString(Kite::getRTypesName(Type).c_str()));
		msg.exec();
		return false;
	}
	res->setModified(true);
	return true;
}

Kite::KResource *ResourceDock::getResource(const QString &Name) {
	return rman.get(Name.toStdString());
}

Kite::KResource *ResourceDock::addResource(Kite::RTypes Type) {
	// find category and format
	auto pitem = resTree->findItems(Kite::getRTypesName(Type).c_str(), Qt::MatchFlag::MatchRecursive);
	auto frmt = formats.find((size_t)Type)->first;
	
	QString text;
	frmnewres *form;
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

	do {
		// show form
		form->exec();

		// cancel pressed
		if (!form->isOk()) {
			return nullptr;
		}

		text = form->getName();

		// shader types
		if (Type == Kite::RTypes::Shader) {
			if (form->getType() == "Vertex") { frmt = ".vert"; }
			if (form->getType() == "Fragment") { frmt = ".frag"; }
			if (form->getType() == "Geometry") { frmt = ".geom"; }
		}

		// available name
		if (!resTree->findItems(text + frmt, Qt::MatchFlag::MatchRecursive).isEmpty()) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("this name is already exist!");
			msg.exec();
			continue;
		}

		// file validation
		QFile file(currDirectory + "/" + text + frmt);
		if (file.exists()) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("this name is exists in the current resource directory.");
			msg.exec();
			file.close();
			continue;
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
	} while (true);

	delete form;

	// create new res
	auto newres = rman.create(Type, text.toStdString());
	if (newres == nullptr) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("resource add module return nullptr!\nresource type: " + QString(Kite::getRTypesName(Type).c_str()));
		msg.exec();
		return nullptr;
	}

	// save it to the hard disk
	Kite::KFOStream fstream;
	if (!newres->saveStream(fstream, currDirectory.toStdString() + "/" + text.toStdString())) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("cant create resource file.!\nresource type: " + QString(Kite::getRTypesName(Type).c_str()));
		msg.exec();
		return nullptr;
	}
	delete newres;

	// reload it from hard disk
	auto res = rman.load(Kite::IStreamTypes::FIStream, Type, currDirectory.toStdString() + "/" + text.toStdString());
	if (res == nullptr) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("resource add module return nullptr!\nresource type: " + QString(Kite::getRTypesName(Type).c_str()));
		msg.exec();
		return nullptr;
	}

	return res;
}

Kite::KResource *ResourceDock::addResourceInternal(Kite::RTypes Type, const QString &Name) {
	// check name
	// available name
	if (!resTree->findItems(Name, Qt::MatchFlag::MatchRecursive).isEmpty()) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("an resource with this name \"" + Name + "\" is already exist! use another name.");
		msg.exec();
		return nullptr;
	}

	// create new res
	auto newres = rman.create(Type, Name.toStdString());
	if (newres == nullptr) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("resource add module return nullptr!\nresource type: " + QString(Kite::getRTypesName(Type).c_str()));
		msg.exec();
		return nullptr;
	}

	// save it to the hard disk
	Kite::KFOStream fstream;
	if (!newres->saveStream(fstream, currDirectory.toStdString() + "/" + Name.toStdString())) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("cant create resource file.!\nresource type: " + QString(Kite::getRTypesName(Type).c_str()));
		msg.exec();
		return nullptr;
	}
	delete newres;

	// reload it from hard disk
	auto res = rman.load(Kite::IStreamTypes::FIStream, Type, currDirectory.toStdString() + "/" + Name.toStdString());
	if (res == nullptr) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("resource add module return nullptr!\nresource type: " + QString(Kite::getRTypesName(Type).c_str()));
		msg.exec();
		return nullptr;
	}

	return res;
}

void ResourceDock::actDoubleClicked(QTreeWidgetItem * item, int column) {
	actEdit();
}

void ResourceDock::actClicked() {
	auto item = resTree->currentItem();

	if (item != nullptr) {
		if (item->parent() == nullptr) {
			actionsControl(AS_ON_CAT);
			addRes->setText("Add New " + item->text(0));
			openRes->setText("Add Existing " + item->text(0));
		} else {
			actionsControl(AS_ON_ITEM);
			editRes->setText("Edit " + item->text(0));
			remRes->setText("Remove " + item->text(0));
			emit(resourceSelected(rman.get(item->text(0).toStdString())));
		}
	}
}

void ResourceDock::actRClicked(const QPoint & pos) {
	QTreeWidgetItem *item = resTree->itemAt(pos);
	QMenu cmenu(this);
	QPoint pt(pos);
	pt.setY(pt.y() + 50);
	
	if (item != nullptr) {
		// checking user rclicked on category or items?
		if (item->parent() == nullptr) {
			addRes->setText("Add New " + item->text(0));
			openRes->setText("Add Existing " + item->text(0));
			cmenu.addAction(addRes);
			cmenu.addAction(openRes);
			cmenu.exec(mapToGlobal(pt));
		} else {
			editRes->setText("Edit " + item->text(0));
			remRes->setText("Remove " + item->text(0));
			saveRes->setText("Save " + item->text(0));
			saveAsRes->setText("Save " + item->text(0) + " As...");
			cmenu.addAction(editRes);
			cmenu.addSeparator();
			cmenu.addAction(remRes);
			cmenu.addSeparator();
			cmenu.addAction(saveRes);
			cmenu.addAction(saveAsRes);
			cmenu.exec(mapToGlobal(pt));
		}
	}
}

void ResourceDock::actAdd() {
	auto pitem = resTree->currentItem();
	auto restype = Kite::getRTypesByName(pitem->text(0).toStdString());
	
	if (pitem->parent() != nullptr) {
		return;
	}

	addResource(restype);
}

void ResourceDock::actOpen() {
	auto pitem = resTree->currentItem();
	auto restype = Kite::getRTypesByName(pitem->text(0).toStdString());
	auto frm = formats.values((size_t)restype);

	if (pitem->parent() != nullptr) {
		return;
	}

	QString formats;
	for (auto it = frm.begin(); it != frm.end(); ++it) {
		formats += " *" + it->first;
	}
	QString fileName = QFileDialog::getOpenFileName(this, "Open " + QString(Kite::getRTypesName(restype).c_str()),
													currDirectory, "Kite2D Resource File (" + formats + ")");

	if (!fileName.isEmpty()) {
		openResource(fileName, restype);
	}
}

void ResourceDock::actSave() {
	auto item = resTree->currentItem();
	auto res = rman.get(item->text(0).toStdString());

	if (res->getAddress().empty()) {
		return actSaveAs();
	}

	emit(resourceSave(res));
	Kite::KFOStream ostream;
	if (!res->saveStream(ostream, res->getAddress() + "/" + res->getName())) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("cant save resource.\nfile address: " + QString(res->getName().c_str()));
		msg.exec();
	}
}

void ResourceDock::actSaveAs() {
	auto item = resTree->currentItem();
	auto res = rman.get(item->text(0).toStdString());
	auto type = Kite::getRTypesByName(item->parent()->text(0).toStdString());
	auto frm = formats.find((size_t)type);

	QString fileName = QFileDialog::getSaveFileName(this, "Save " + item->text(0),
													"", "Kite2D Resource File (*" + frm->first +" )");

	if (!fileName.isEmpty()) {
		emit(resourceSave(res));
		Kite::KFOStream ostream;
		if (!res->saveStream(ostream, fileName.toStdString(), true)) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("cant open file stream with the given address.\nfile address: " + fileName);
			msg.exec();
			return;
		}
	}
}

void ResourceDock::actEdit() {
	auto item = resTree->currentItem();
	if (item->parent() != nullptr) {
		auto res = rman.get(item->text(0).toStdString());

		// emit edit signal
		emit(resourceEdit(res));
	}
}

void ResourceDock::actRemove() {
	if (resTree->currentItem()->parent() == nullptr) {
		return;
	}

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "Message", "Are you sure you want to remove this resource?",
								  QMessageBox::Yes | QMessageBox::No);
	if (reply != QMessageBox::Yes) {
		return;
	}

	auto item = resTree->currentItem();
	rman.unload(item->text(0).toStdString(), true);

	delete item;
}

void ResourceDock::actSearch(const QString &Pharase) {
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