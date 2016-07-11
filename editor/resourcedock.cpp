#include "resourcedock.h"
#include <QtWidgets>
#include <qinputdialog.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <Kite/core/kscene.h>
#include <Kite/core/kfistream.h>
#include <Kite/core/kfostream.h>
#include <Kite/logic/klogic.h>
#include <kmeta.khgen.h>

ResourceDock::ResourceDock(QWidget *Parrent) :
	QDockWidget("Resource Explorer", Parrent),
	kiteDictionary(new std::unordered_map<std::string, std::string>)
{
	setObjectName("Resources");
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setMinimumWidth(120);

	setupTree();
	setupActions();
	actionsControl(AS_ON_INITE);
	setupHTools();
	registerRTypes(&rman);
}

ResourceDock::~ResourceDock() {
	delete kiteDictionary;
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

void ResourceDock::setupCategories(const QStringList &CatList) {
	resTree->clear();
	for (auto it = CatList.begin(); it != CatList.end(); ++it) {
		// 
		auto cat = new QTreeWidgetItem(resTree);
		cat->setText(0, (*it));
		cat->setIcon(0, QIcon(":/icons/open"));

		// formats
		QPair<QString, QString> fpair;
		if ((*it) == "KScene") {
			fpair.first = ".sce";
			fpair.second = ":/icons/sce";
		} else if ((*it) == "KScript") {
			fpair.first = ".lua";
			fpair.second = ":/icons/scr";
		} else if ((*it) == "KPrefab") {
			fpair.first = ".pre";
			fpair.second = ":/icons/new";
		} else {
			fpair.first = "";
			fpair.second = ":/icons/new";
		}
		formats.insert((*it), fpair);
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

	remRes = new QAction(QIcon(":/icons/remove"), "Remove", this);
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

	auto name = new QLabel(htools);
	name->setText("Resource Explorer");
	name->setStyleSheet("color: DodgerBlue;");
	vlayout->addWidget(name);

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
	for (auto it = dictinary.begin(); it != dictinary.end(); ++it) {
		emit(resourceDelete((*it)));
	}
	dictinary.clear();
	resTree->clear();
}

void ResourceDock::filterByType(const QString &Type, QStringList &List) {
	List.clear();
	for (auto it = dictinary.begin(); it != dictinary.end(); ++it) {
		if (it.value()->getType() == Type.toStdString()) {
			List.append(it.key());
		}
	}
}

void ResourceDock::selectResource(const QString &Name) {
	auto found = resTree->findItems(Name, Qt::MatchFlag::MatchRecursive);
	if (!found.isEmpty()) {
		resTree->setCurrentItem(found.first());
	}
}

void ResourceDock::manageUsedResource(const QHash<QString, QVector<Kite::KMetaProperty>> *ResComponents) {
	if (ResComponents->empty()) {
		return;
	}
	for (auto it = dictinary.begin(); it != dictinary.end(); ++it) {
		if (it.value()->getType() == "KScene") {
			auto scene = (Kite::KScene *)it.value();
			auto eman = scene->getEManager();

			scene->clearResources();

			for (auto eit = eman->beginEntity(); eit != eman->endEntity(); ++eit) {

				// first we collect scripts from logic components
				std::vector<Kite::KHandle> lcomps;
				eit->getScriptComponents(lcomps);
				for (auto sit = lcomps.begin(); sit != lcomps.end(); ++sit) {
					auto script = (Kite::KLogicCom *)eit->getComponent("Logic", (*sit));
					if (!script->getScript().empty()) {
						scene->addResource(script->getScript(), "KScript");
					}
				}

				// then we collect another resources from components
				for (auto cit = ResComponents->begin(); cit != ResComponents->end(); ++cit) {
					if (cit.key() == "Logic") {
						continue;
					}

					if (eit->hasComponent(cit.key().toStdString(), "")) {
						for (auto pit = cit->begin(); pit != cit->end(); ++pit) {
							auto com = eit->getComponentByName(cit.key().toStdString(), "");
							auto res = com->getProperty(pit->name);
							auto resName = res.as<std::string>();
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

const std::unordered_map<std::string, std::string> *ResourceDock::getKiteDictionary(const QString &AddressPrefix) const {
	kiteDictionary->clear();
	for (auto it = dictinary.begin(); it != dictinary.end(); ++it) {
		kiteDictionary->insert({ it.key().toStdString(), AddressPrefix.toStdString() + "resources/" + it.key().toStdString()});
	}

	return kiteDictionary;
}

bool ResourceDock::openResource(const QString &Address, const QString &Type) {
	// check name
	QFileInfo file(Address);
	if (dictinary.find(file.fileName()) != dictinary.end()) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("This file is already exist.\nType: " + Type + "\nAddress: " + Address);
		msg.exec();
		return false;
	}

	// check type
	auto count = resTree->topLevelItemCount() - 1;
	bool registered = false;
	for (count; count >= 0; --count) {
		if (resTree->topLevelItem(count)->text(0) == Type) {
			registered = true;
			break;
		}
	}
	if (!registered) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("Unregistered resource type.\nType: " + Type + "\nAddress: " + Address);
		msg.exec();
		return false;
	}

	// cretae new resource 
	auto newres = rman.create(Type.toStdString(), file.fileName().toStdString());
	if (newres != nullptr) {
		// load resource
		Kite::KFIStream istream;
		if (!newres->loadStream(&istream, Address.toStdString())) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("cant load resource data from file.\nfile address: " + Address);
			msg.exec();
			delete newres;
			return false;
		}

		dictinary.insert(newres->getName().c_str(), newres);
		auto found = formats.find(Type);
		auto pitem = resTree->topLevelItem(count);
		auto item = new QTreeWidgetItem(pitem);
		item->setText(0, newres->getName().c_str());
		item->setIcon(0, QIcon(found->second));
		pitem->setExpanded(true);
		emit(resourceOpen(newres));

		return true;
	} else {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("Resource add module return nullptr!\nresource type: " + Type);
		msg.exec();
	}
	return false;
}

Kite::KResource *ResourceDock::getResource(const QString &Name) {
	return dictinary[Name];
}

Kite::KResource *ResourceDock::addResource(const QString &Type) {
	auto frmt = formats.find(Type);

	// find category
	QTreeWidgetItem * pitem = nullptr;
	for (auto i = 0; i < resTree->topLevelItemCount(); ++i) {
		if (resTree->topLevelItem(i)->text(0) == Type) {
			pitem = resTree->topLevelItem(i);
			break;
		}
	}
	if (pitem == nullptr) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("this type is not registered!\nresource type: " + Type);
		msg.exec();
		return nullptr;
	}
	
	// create new resource and add it to res tree
	bool ok = false;
	QString text;
	do {
		text = QInputDialog::getText(this, "New " + Type,
									 Type + " name:", QLineEdit::Normal,
									 "", &ok);

		// cancel pressed
		if (!ok) {
			return nullptr;
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
		if (dictinary.find(text + frmt->first) != dictinary.end()) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("this name is already exist!");
			msg.exec();
			continue;
		}

		// file validation
		QFile file(currDirectory + "\\" + text + frmt->first);
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
		if (ok) {
			text = QFileInfo(file.fileName()).fileName();
			break;
		}
	} while (true);

	auto newres = rman.create(Type.toStdString(), text.toStdString());
	if (newres != nullptr) {
		// add to resource dictionary map
		dictinary.insert(text, newres);

		// tree item
		auto item = new QTreeWidgetItem(pitem);
		item->setText(0, text);
		item->setIcon(0, QIcon(frmt->second));
		pitem->setExpanded(true);
		emit(resourceAdded(newres));
		return newres;

	} else {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("resource add module return nullptr!\nresource type: " + Type);
		msg.exec();
	}
	return nullptr;
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
		} else if (!dictinary.isEmpty()) {
			actionsControl(AS_ON_ITEM);
			editRes->setText("Edit " + item->text(0));
			remRes->setText("Remove " + item->text(0));
			emit(resourceSelected((*dictinary.find(item->text(0)))));
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
	auto restype = pitem->text(0);
	
	if (pitem->parent() != nullptr) {
		return;
	}

	addResource(restype);
}

void ResourceDock::actOpen() {
	auto pitem = resTree->currentItem();
	auto restype = pitem->text(0);
	auto frm = formats.find(restype);

	if (pitem->parent() != nullptr) {
		return;
	}

	QString fileName = QFileDialog::getOpenFileName(this, "Open " + restype, "", "Kite2D Resource File (*" + frm->first + ")");

	if (!fileName.isEmpty()) {
		openResource(fileName, restype);
	}
}

void ResourceDock::actSave() {
	auto item = resTree->currentItem();
	auto res = (*dictinary.find(item->text(0)));

	if (res->getAddress().empty()) {
		return actSaveAs();
	}

	Kite::KFOStream ostream;
	if (!res->saveStream(&ostream, res->getAddress() + "\\" + res->getName())) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("cant save resource.\nfile address: " + QString(res->getName().c_str()));
		msg.exec();
	}
}

void ResourceDock::actSaveAs() {
	auto item = resTree->currentItem();
	auto res = (*dictinary.find(item->text(0)));
	auto type = item->parent()->text(0);
	auto frm = formats.find(type);

	QString fileName = QFileDialog::getSaveFileName(this, "Save " + item->text(0),
													"", "Kite2D Resource File (*" + frm->first +" )");

	if (!fileName.isEmpty()) {
		Kite::KFOStream ostream;
		if (!res->saveStream(&ostream, fileName.toStdString())) {
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
		auto res = (*dictinary.find(item->text(0)));

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
	auto res = (*dictinary.find(item->text(0)));

	// first we emit the corresponding signal
	emit(resourceDelete(res));

	// then we erase it from gui
	dictinary.erase(dictinary.find(item->text(0)));
	delete item;
	delete res;
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