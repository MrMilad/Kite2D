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
		auto cat = new QTreeWidgetItem(resTree);
		cat->setText(0, (*it));
		cat->setIcon(0, QIcon(":/icons/open"));
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
	btnAdd->setAutoRaise(true);
	btnAdd->setDefaultAction(addRes);
	btnAdd->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnAdd);

	auto btnOpen = new QToolButton(htools);
	btnOpen->setAutoRaise(true);
	btnOpen->setDefaultAction(openRes);
	btnOpen->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnOpen);

	hlayout->addSpacing(10);

	auto btnEdit = new QToolButton(htools);
	btnEdit->setAutoRaise(true);
	btnEdit->setDefaultAction(editRes);
	btnEdit->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnEdit);

	auto btnRemove = new QToolButton(htools);
	btnRemove->setAutoRaise(true);
	btnRemove->setDefaultAction(remRes);
	btnRemove->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnRemove);

	hlayout->addSpacing(10);

	ledit = new QLineEdit(htools);
	ledit->setPlaceholderText("Search");
	ledit->addAction(QIcon(":/icons/search"), QLineEdit::ActionPosition::TrailingPosition);
	ledit->setStyleSheet("background-color: gray;");
	connect(ledit, &QLineEdit::textChanged, this, &ResourceDock::actSearch);
	
	hlayout->addWidget(ledit);

	vlayout->addLayout(hlayout);

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
		if (it.value()->getResourceType() == Type.toStdString()) {
			List.append(it.key());
		}
	}

}

void ResourceDock::manageUsedResource(const QHash<QString, QVector<Kite::KMetaProperty>> *ResComponents) {
	if (ResComponents->empty()) {
		return;
	}
	for (auto it = dictinary.begin(); it != dictinary.end(); ++it) {
		if (it.value()->getResourceType() == "KScene") {
			auto scene = (Kite::KScene *)it.value();
			auto eman = scene->getEManager();

			scene->clearResources();

			for (auto eit = eman->beginEntity(); eit != eman->endEntity(); ++eit) {

				// first we collect scripts from logic components
				std::vector<Kite::KComponent *> lcomps;
				eit->getScriptComponents(lcomps);
				for (auto sit = lcomps.begin(); sit != lcomps.end(); ++sit) {
					auto script = (Kite::KLogicCom *)(*sit);
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
		kiteDictionary->insert({ it.key().toStdString(), AddressPrefix.toStdString() + "resources/" + it.key().toStdString() + ".kres" });
	}

	return kiteDictionary;
}

bool ResourceDock::openResource(const QString &Address, const QString &Type) {
	auto pitem = resTree->findItems(Type, Qt::MatchFlag::MatchExactly);
	if (pitem.isEmpty()) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("Unregistered resource type.\nType: " + Type + "\nAddress: " + Address);
		msg.exec();
		return false;
	}

	// cretae new resource 
	auto newres = rman.create(Type.toStdString(), "");
	if (newres != nullptr) {
		// load resource
		Kite::KFIStream istream;
		if (!istream.open(Address.toStdString(), Kite::IOMode::BIN)) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("cant open file stream with the given address.\nfile address: " + Address);
			msg.exec();
			return false;
		}

		if (!newres->loadStream(&istream)) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("cant load resource data from file.\nfile address: " + Address);
			msg.exec();

			delete newres;
			return false;
		}

		newres->setResourceAddress(Address.toStdString());
		dictinary.insert(newres->getResourceName().c_str(), newres);

		auto item = new QTreeWidgetItem(pitem.first());
		item->setText(0, newres->getResourceName().c_str());
		if (Type == "KScript") {
			item->setIcon(0, QIcon(":/icons/script"));
		} else if (Type == "KScene") {
			item->setIcon(0, QIcon(":/icons/scene"));
		}
		pitem.first()->setExpanded(true);

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
	
	if (item != nullptr) {
		// checking user rclicked on category or items?
		if (item->parent() == nullptr) {
			QPoint pt(pos);
			addRes->setText("Add New " + item->text(0));
			openRes->setText("Add Existing " + item->text(0));
			cmenu.addAction(addRes);
			cmenu.addAction(openRes);
			cmenu.exec(mapToGlobal(pos));
		} else {
			QPoint pt(pos);
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
			cmenu.exec(mapToGlobal(pos));
		}
	}
}

void ResourceDock::actAdd() {
	auto pitem = resTree->currentItem();
	auto restype = pitem->text(0);

	if (pitem->parent() != nullptr) {
		return;
	}

	// create new resource and add it to res tree
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
		auto avail = dictinary.find(text);
		if (avail != dictinary.end()) {
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

	auto newres = rman.create(restype.toStdString(), text.toStdString());
	if (newres != nullptr) {
		// add to resource dictionary map
		dictinary.insert(text, newres);

		// register ctypes
		if (restype == "KScene") {
			auto scene = (Kite::KScene *)newres;
			Kite::registerCTypes(scene->getEManager());
		}

		auto item = new QTreeWidgetItem(pitem);
		item->setText(0, text);
		if (restype == "KScript") {
			item->setIcon(0, QIcon(":/icons/script"));
		} else if (restype == "KScene") {
			item->setIcon(0, QIcon(":/icons/scene"));
		}
		pitem->setExpanded(true);

		emit(resourceAdded(newres));

	} else {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("resource add module return nullptr!\nresource type: " + restype);
		msg.exec();
	}
}

void ResourceDock::actOpen() {
	auto pitem = resTree->currentItem();
	auto restype = pitem->text(0);

	if (pitem->parent() != nullptr) {
		return;
	}

	QString fileName = QFileDialog::getOpenFileName(this, "Open " + restype, "", "Kite2D Resource File (*.kres)");

	if (!fileName.isEmpty()) {
		openResource(fileName, restype);
	}
}

void ResourceDock::actSave() {
	auto item = resTree->currentItem();
	auto res = (*dictinary.find(item->text(0)));

	Kite::KFOStream ostream;
	if (!ostream.open(res->getResourceName(), Kite::IOMode::BIN)) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("cant open file stream with the given address.\nfile address: " + QString(res->getResourceName().c_str()));
		msg.exec();
		return;
	}

	res->saveStream(&ostream);
}

void ResourceDock::actSaveAs() {
	auto item = resTree->currentItem();
	auto res = (*dictinary.find(item->text(0)));

	QString fileName = QFileDialog::getSaveFileName(this, "Save " + item->text(0),
													"", "Kite2D Resource File (*.kres)");

	if (!fileName.isEmpty()) {
		Kite::KFOStream ostream;
		if (!ostream.open(fileName.toStdString(), Kite::IOMode::BIN)) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("cant open file stream with the given address.\nfile address: " + fileName);
			msg.exec();
			return;
		}

		res->saveStream(&ostream);
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