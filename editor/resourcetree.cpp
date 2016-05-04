#include "resourcetree.h"
#include <QtWidgets>
#include <qinputdialog.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <Kite/core/kscene.h>
#include <Kite/core/kfistream.h>
#include <Kite/core/kfostream.h>
#include <kmeta.khgen.h>

ResourceTree::ResourceTree(QWidget *Parrent) :
	QTreeWidget(Parrent) 
{
	setColumnCount(1);
	setHeaderHidden(true);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &QTreeWidget::itemSelectionChanged, this, &ResourceTree::actClicked);
	connect(this, &QTreeWidget::customContextMenuRequested, this, &ResourceTree::actRClicked);

	setupActions();
	actionsControl(AS_ON_INITE);
	setupHTools();
	setupShortcuts();

	registerRTypes(&rman);
}

ResourceTree::~ResourceTree() {}

void ResourceTree::focusInEvent(QFocusEvent *Event) {
	for (auto it = shortcuts.begin(); it != shortcuts.end(); ++it) {
		(*it)->setEnabled(true);
	}
}

void ResourceTree::focusOutEvent(QFocusEvent *Event) {
	for (auto it = shortcuts.begin(); it != shortcuts.end(); ++it) {
		(*it)->setEnabled(false);
	}
}

void ResourceTree::setupCategories(const QStringList &CatList) {
	clear();
	for (auto it = CatList.begin(); it != CatList.end(); ++it) {
		auto cat = new QTreeWidgetItem(this);
		cat->setText(0, (*it));
		cat->setIcon(0, QIcon(":/icons/open"));
	}
}

void ResourceTree::setupActions() {
	addRes = new QAction(QIcon(":/icons/add"), "Add New Resource", this);
	connect(addRes, &QAction::triggered, this, &ResourceTree::actAdd);

	openRes = new QAction(QIcon(":/icons/open"), "Add Existing Resource", this);
	connect(openRes, &QAction::triggered, this, &ResourceTree::actOpen);

	saveRes = new QAction(QIcon(":/icons/save"), "Save Resource", this);
	connect(saveRes, &QAction::triggered, this, &ResourceTree::actSave);

	saveAsRes = new QAction(QIcon(":/icons/saveAs"), "Save Resource As", this);
	connect(saveAsRes, &QAction::triggered, this, &ResourceTree::actSaveAs);

	editRes = new QAction(QIcon(":/icons/resEdit"), "Edit", this);
	connect(editRes, &QAction::triggered, this, &ResourceTree::actEdit);

	remRes = new QAction(QIcon(":/icons/remove"), "Remove", this);
	connect(remRes, &QAction::triggered, this, &ResourceTree::actRemove);

}

void ResourceTree::setupShortcuts() {
	shortcuts.push_back(new QShortcut(QKeySequence(Qt::Key_Delete), this));
	connect(shortcuts.back(), &QShortcut::activated, remRes, &QAction::trigger);

	shortcuts.push_back(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this));
	connect(shortcuts.back(), &QShortcut::activated, openRes, &QAction::trigger);

	shortcuts.push_back(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this));
	connect(shortcuts.back(), &QShortcut::activated, saveRes, &QAction::trigger);

	shortcuts.push_back(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N), this));
	connect(shortcuts.back(), &QShortcut::activated, addRes, &QAction::trigger);

	shortcuts.push_back(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_E), this));
	connect(shortcuts.back(), &QShortcut::activated, editRes, &QAction::trigger);
}

void ResourceTree::setupHTools() {
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

	hlayout->addSpacing(10);

	ledit = new QLineEdit(htools);
	ledit->addAction(QIcon(":/icons/search"), QLineEdit::ActionPosition::TrailingPosition);
	ledit->setStyleSheet("background-color: gray;");
	connect(ledit, &QLineEdit::textChanged, this, &ResourceTree::actSearch);
	
	hlayout->addWidget(ledit);

	vlayout->addLayout(hlayout);

	htools->setLayout(vlayout);
}

void ResourceTree::actionsControl(ActionsState State) {
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

void ResourceTree::clearResources() {
	for (auto it = dictinary.begin(); it != dictinary.end(); ++it) {
		emit(resourceDelete(it->resource));
	}
	clear();
	dictinary.clear();
}

void ResourceTree::actClicked() {
	auto item = currentItem();

	if (item->parent() == nullptr) {
		actionsControl(AS_ON_CAT);
		addRes->setText("Add New " + item->text(0));
		openRes->setText("Add Existing " + item->text(0));
	} else {
		actionsControl(AS_ON_ITEM);
		editRes->setText("Edit " + item->text(0));
		remRes->setText("Remove " + item->text(0));
		emit(resourceSelected(dictinary.find(item->text(0))->resource));
	}
}

void ResourceTree::actRClicked(const QPoint & pos) {
	QTreeWidgetItem *item = itemAt(pos);
	QMenu cmenu(this);
	
	if (item != nullptr) {
		// checking user rclicked on category or items??
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

void ResourceTree::actAdd() {
	if (!addRes->isEnabled()) {
		return;
	}
	auto parrent = currentItem();
	auto restype = parrent->text(0);

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
		dictinary.insert(text, ResourceItem( "", restype, newres));

		// register ctypes
		if (restype == "KScene") {
			auto scene = (Kite::KScene *)newres;
			Kite::registerCTypes(scene->getEManager());
		}

		auto item = new QTreeWidgetItem(parrent);
		item->setText(0, text);
		parrent->setExpanded(true);

		emit(resourceAdded(newres));

	} else {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("resource add module return nullptr!\nresource type: " + restype);
		msg.exec();
	}
}

void ResourceTree::actOpen() {
	if (!openRes->isEnabled()) {
		return;
	}
	auto parrent = currentItem();
	auto restype = parrent->text(0);

	QString fileName = QFileDialog::getOpenFileName(this, "Open " + restype, "", "Kite2D Resource File (*.kres)");

	// chck name is available
	if (!fileName.isEmpty()) {
		QFileInfo finfo(fileName);
		auto avail = dictinary.find(finfo.baseName());
		if (avail != dictinary.end()) {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("this name is already exist!");
			msg.exec();
			return;
		}

		// cretae new resource 
		auto newres = rman.create(restype.toStdString(), fileName.toStdString());
		if (newres != nullptr) {
			// register ctypes with kcene
			if (restype == "KScene") {
				auto scene = (Kite::KScene *)newres;
				Kite::registerCTypes(scene->getEManager());
			}

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

			dictinary.insert(finfo.baseName(), ResourceItem(fileName, restype, newres));
			auto item = new QTreeWidgetItem(parrent);
			item->setText(0, finfo.baseName());
			parrent->setExpanded(true);

			emit(resourceOpen(newres));
		} else {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("resource add module return nullptr!\nresource type: " + restype);
			msg.exec();
		}
	}
}

void ResourceTree::actSave() {
	if (!saveRes->isEnabled()) {
		return;
	}
	auto item = currentItem();
	auto res = dictinary.find(item->text(0))->resource;

	Kite::KFOStream ostream;
	if (!ostream.open(res->getResourceName(), Kite::KIOTypes::KRT_BIN)) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("cant open file stream with the given address.\nfile address: " + QString(res->getResourceName().c_str()));
		msg.exec();
		return;
	}

	res->saveStream(&ostream);
}

void ResourceTree::actSaveAs() {
	if (!saveAsRes->isEnabled()) {
		return;
	}
	auto item = currentItem();
	auto res = dictinary.find(item->text(0))->resource;

	QString fileName = QFileDialog::getSaveFileName(this, "Save " + item->text(0),
													"", "Kite2D Resource File (*.kres)");

	if (!fileName.isEmpty()) {
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

void ResourceTree::actEdit() {
	if (!editRes->isEnabled()) {
		return;
	}
	auto item = currentItem();
	auto res = dictinary.find(item->text(0))->resource;
	
	// emit edit signal
	emit(resourceEdit(res));
}

void ResourceTree::actRemove() {
	if (!remRes->isEnabled()) {
		return;
	}

	auto item = currentItem();
	setCurrentItem(item->parent());
	auto res = dictinary.find(item->text(0))->resource;

	// first we emit the corresponding signal
	emit(resourceDelete(res));

	// then we erase it from gui
	dictinary.erase(dictinary.find(item->text(0)));
	delete item;
	delete res;
}

void ResourceTree::actSearch(const QString &Pharase) {
	auto allItems = QTreeWidgetItemIterator(this);
	if (Pharase.isEmpty()) {
		while (*allItems) {
			if ((*allItems)->parent() != nullptr) {
				(*allItems)->setHidden(false);
			}
			++allItems;
		}
	} else {
		while (*allItems) {
			if ((*allItems)->parent() != nullptr) {
				if (!(*allItems)->text(0).contains(Pharase)) {
					(*allItems)->setHidden(true);
				}
			}
			++allItems;
		}
	}
}