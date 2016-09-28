#include "texturearrayeditor.h"
#include "frmnewtexarray.h"
#include <Kite/core/kfistream.h>
#include <QtWidgets>

TextureArrayEditor::TextureArrayEditor(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent) :
	TabWidget(Res, KInfo, Parent) ,
	atlasArray((Kite::KAtlasTextureArray *)Res)
{

}

void TextureArrayEditor::inite() {
	auto vlayout = new QVBoxLayout(this);
	vlayout->setMargin(3);
	vlayout->setSpacing(0);

	auto hlayTools = new QHBoxLayout();
	hlayTools->setMargin(0);

	auto btnNew = new QToolButton(this);
	btnNew->setIcon(QIcon(":/icons/add"));
	btnNew->setToolTip("Create New Texture Group");
	hlayTools->addWidget(btnNew);
	connect(btnNew, &QToolButton::clicked, this, &TextureArrayEditor::createNew);
	
	hlayTools->addSpacing(5);
	auto sep1 = new QLabel(this);
	sep1->setText("|");
	sep1->setStyleSheet("color: yellow");
	hlayTools->addWidget(sep1);
	hlayTools->addSpacing(5);

	auto lblFilter = new QLabel(this);
	lblFilter->setText("Filter: ");
	hlayTools->addWidget(lblFilter);
	hlayTools->addSpacing(3);
	cmbFilter = new QComboBox(this);
	cmbFilter->addItem("Nearest");
	cmbFilter->addItem("Linear");
	cmbFilter->setCurrentIndex((int)atlasArray->getFilter());
	hlayTools->addWidget(cmbFilter);
	hlayTools->addSpacing(5);

	auto lblWrap = new QLabel(this);
	lblWrap->setText("Wrap: ");
	hlayTools->addWidget(lblWrap);
	cmbWrap = new QComboBox(this);
	cmbWrap->addItem("Repeat");
	cmbWrap->addItem("Mirrored Repeat");
	cmbWrap->addItem("Clamp to Edge");
	cmbWrap->setCurrentIndex((int)atlasArray->getWrap());
	hlayTools->addWidget(cmbWrap);

	hlayTools->addStretch(1);

	vlayout->addLayout(hlayTools);

	vlayout->addSpacing(5);

	// select list
	auto hlayView = new QHBoxLayout();
	selectList = new QListWidget(this);
	selectList->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
	selectList->setAlternatingRowColors(true);
	selectList->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(selectList, &QListWidget::customContextMenuRequested, this, &TextureArrayEditor::slistContextMenu);
	connect(selectList, &QListWidget::itemDoubleClicked, this, &TextureArrayEditor::add);
	hlayView->addWidget(selectList, 1);
	hlayView->addSpacing(3);

	// middle buttons
	auto vlayMiddle = new QVBoxLayout();
	vlayMiddle->setSpacing(3);
	vlayMiddle->setMargin(0);

	vlayMiddle->addStretch(1);

	actAdd = new QAction(QIcon(":/icons/farr"), "Add Items", this);
	//actAdd->setShortcut(Qt::Key::Key_Enter);
	//selectList->addAction(actAdd);
	auto btnAdd = new QToolButton(this);
	btnAdd->setDefaultAction(actAdd);
	btnAdd->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
	btnAdd->setToolTip("Add items");
	connect(actAdd, &QAction::triggered, this, &TextureArrayEditor::add);
	vlayMiddle->addWidget(btnAdd);

	actSwap = new QAction(QIcon(":/icons/swap"), "Swap Item", this);
	auto btnSwap = new QToolButton(this);
	btnSwap->setDefaultAction(actSwap);
	btnSwap->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
	btnSwap->setToolTip("Swap Item");
	connect(actSwap, &QAction::triggered, this, &TextureArrayEditor::swap);
	vlayMiddle->addWidget(btnSwap);

	actRemove = new QAction(QIcon(":/icons/remove"), "Remove Item", this);
	auto btnRemove = new QToolButton(this);
	btnRemove->setDefaultAction(actRemove);
	btnRemove->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
	btnRemove->setToolTip("Remove Item");
	connect(actRemove, &QAction::triggered, this, &TextureArrayEditor::remove);
	vlayMiddle->addWidget(btnRemove);

	vlayMiddle->addStretch(1);

	hlayView->addLayout(vlayMiddle);
	hlayView->addSpacing(3);

	// finla list
	finalList = new QListWidget(this);
	finalList->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	finalList->setAlternatingRowColors(true);
	finalList->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(finalList, &QListWidget::customContextMenuRequested, this, &TextureArrayEditor::flistContextMenu);
	hlayView->addWidget(finalList, 1);

	vlayout->addLayout(hlayView);

	vlayout->addSpacing(3);

	// bottom info
	botFrame = new QFrame(this);
	botFrame->hide();
	botFrame->setStyleSheet("QFrame { background-color: rgb(30,30,30); }");
	auto botHLayout = new QHBoxLayout(botFrame);
	botHLayout->setMargin(3);

	auto lblWarning = new QLabel(this);
	lblWarning->setText("<font color = \"orange\">Warning: </font>all item(s) that marked as "
						"<font color = \"red\">RED</font> should be swaped with an correct item or will be removed automatically from array.");
	botHLayout->addWidget(lblWarning);
	botHLayout->addStretch(1);
	vlayout->addWidget(botFrame);

	reload();
}

bool TextureArrayEditor::saveChanges() {
	atlasArray->setModified(true);
	return true;
}

void TextureArrayEditor::reload() {
	fillSelectList();
	fillFinalList();
}

void TextureArrayEditor::onRemoveRes(Kite::RTypes Type) {
	if (Type == Kite::RTypes::AtlasTexture || Type == Kite::RTypes::Texture) {
		reload();
	}
}

void TextureArrayEditor::onAddRes(const Kite::KResource *Res) {
	if (Res->getType() == Kite::RTypes::AtlasTexture) {
		reload();
	}
}

void TextureArrayEditor::createNew() {
	auto newFrm = new frmNewTexArray(atlasArray->getTextureSize().x, atlasArray->getTextureSize().y, this);
	newFrm->exec();

	if (newFrm->isOk()) {
		atlasArray->clearItems();
		finalList->clear();
		atlasArray->setTextureSize(Kite::KVector2U32(newFrm->getWidth(), newFrm->getHeight()));

		reload();
	}

	delete newFrm;
}

void TextureArrayEditor::flistContextMenu(const QPoint &pos) {
	QPoint globalPos = finalList->mapToGlobal(pos);

	QMenu myMenu;
	myMenu.addAction(actSwap);
	myMenu.addSeparator();
	myMenu.addAction(actRemove);

	myMenu.exec(globalPos);
}

void TextureArrayEditor::slistContextMenu(const QPoint &pos) {
	QPoint globalPos = selectList->mapToGlobal(pos);

	QMenu myMenu;
	myMenu.addAction(actAdd);

	myMenu.exec(globalPos);
}

void TextureArrayEditor::fillSelectList() {
	selectList->clear();
	QList<const Kite::KResource *> rlist;
	emit(requestResList(Kite::RTypes::AtlasTexture, rlist));

	for (auto it = rlist.begin(); it != rlist.end(); ++it) {
		auto atex = (Kite::KAtlasTexture *)(*it);

		if (atex->getTexture()) {
			if (atex->getTexture()->getSize() == atlasArray->getTextureSize()) {
				QListWidgetItem *listItem = new QListWidgetItem(selectList);
				listItem->setText((*it)->getName().c_str());
				selectList->addItem(listItem);
			}
		}
	}

	actAdd->setDisabled(!(selectList->item(0)));
}

void TextureArrayEditor::fillFinalList() {
	botFrame->hide();
	finalList->clear();
	auto alist = atlasArray->getContiner();
	int id = 0;
	for (size_t i = 0; i < alist->size(); ++i) {
		auto item = alist->operator[](i);
		QListWidgetItem *listItem = new QListWidgetItem(finalList);
		finalList->addItem(listItem);
		
		if (!item) {
			botFrame->show();
			item = nullptr;
			listItem->setText("(removed resource)");
			listItem->setBackgroundColor(Qt::red);
			continue;
		}

		listItem->setText(item->getName().c_str());
		if (!item->getTexture()) {
			botFrame->show();
			listItem->setText("Name: " + listItem->text() + " (there is no texture)");
			item->setTexture(nullptr);
			listItem->setBackgroundColor(Qt::red);
			continue;
		} 

		if (item->getTexture()->getSize() != atlasArray->getTextureSize()) {
			botFrame->show();
			listItem->setText("Name: " + listItem->text() + " (texture size missmatch)");
			listItem->setBackgroundColor(Qt::red);
			continue;
		}

		// add id
		listItem->setText("Index: " + QString::number(id) + "\tName: " + listItem->text());
		++id;
	}

	actSwap->setDisabled(!(selectList->item(0)) || !(finalList->item(0)));
	actRemove->setDisabled(!(finalList->item(0)));
}

void TextureArrayEditor::add() {
	auto alist = atlasArray->getContiner();
	auto sitems = selectList->selectedItems();
	int addedItems = 0;

	for (auto it = sitems.begin(); it != sitems.end(); ++it) {
		Kite::KAtlasTexture *ta = nullptr;
		emit(ta = (Kite::KAtlasTexture *)requestRes((*it)->text()));
		if (ta) {
			// check duplication
			if (std::find(alist->begin(), alist->end(), ta) != alist->end()) {
				++addedItems;
				continue;
			}

			// add to list
			if (atlasArray->addItem(ta)) {
				++addedItems;
			}
		}
	}

	fillFinalList();
	if (addedItems != sitems.size()) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("only " + QString::number(addedItems) + " item(s) of total " 
					+ QString::number(sitems.size()) + " selected item(s) added.");
		msg.exec();
		return;
	}
}

void TextureArrayEditor::swap() {
	auto alist = atlasArray->getContiner();
	auto sitem = selectList->currentItem();
	auto findex = finalList->currentRow();

	if (findex <= alist->size() && sitem) {
		Kite::KAtlasTexture *ta = nullptr;
		emit(ta = (Kite::KAtlasTexture *)requestRes(sitem->text()));

		// check duplication
		if (std::find(alist->begin(), alist->end(), ta) == alist->end()) {
			alist->operator[](findex) = ta;
			fillFinalList();
		} else {
			QMessageBox msg;
			msg.setWindowTitle("Message");
			msg.setText("selected item for swap is currently exist in the array list.");
			msg.exec();
		}
	}
}

void TextureArrayEditor::remove() {
	auto alist = atlasArray->getContiner();
	auto findex = finalList->currentRow();

	if (findex <= alist->size()) {
		alist->erase(alist->begin() + findex);
		fillFinalList();
	}
}
