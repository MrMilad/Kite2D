#include "expander.h"
#include <QtWidgets>
#include <comproperty.h>

Expander::Expander(Kite::KComponent *Comp, QTreeWidget *Parent):
	QObject(Parent), expandable(true)
{
	chandle = Comp->getHandle();
	ctype = Comp->getType().c_str();
	QString name = Comp->getType().c_str();
	if (name == "Logic") {
		name.append(" ");
		name.append(Comp->getName().c_str());
	}

	head = new QTreeWidgetItem(Parent);
	head->setFlags(head->flags() ^ Qt::ItemIsDropEnabled);
	head->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	head->setText(0, name);
	head->setHidden(true);

	mainFrame = new QFrame(Parent);

	btnExpand = new QToolButton(mainFrame);
	btnExpand->setIcon(QIcon(":/icons/comp32"));
	btnExpand->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	btnExpand->setText(name);
	btnExpand->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

	auto hlayout = new QHBoxLayout(btnExpand);
	hlayout->setMargin(0);
	hlayout->setSpacing(0);
	hlayout->addStretch(1);

	// dependecy label
	if (!Comp->getDependency().empty()) {
		auto lblDep = new QLabel(mainFrame);
		lblDep->setText("<img src=\":/icons/depend\" height=\"16\" width=\"16\" >");
		lblDep->setStyleSheet("QToolTip { border: 1px solid #2c2c2c; background-color: #242424; color: white;}");
		QString tooltip("<font color=\"orange\">Dependency List:</font>");
		auto depList = Comp->getDependency();
		for (auto it = depList.begin(); it != depList.end(); ++it) {
			tooltip += "\n" + QString((*it).c_str());
		}
		lblDep->setToolTip(tooltip);
		hlayout->addWidget(lblDep);
		hlayout->addSpacing(2);
	}

	// remove button
	auto btnClose = new QToolButton(mainFrame);
	btnClose->setContentsMargins(0, 0, 0, 0); 
	btnClose->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	btnClose->setIcon(QIcon(":/icons/close"));
	btnClose->setToolButtonStyle(Qt::ToolButtonIconOnly);
	btnClose->setAutoRaise(true);
	btnClose->setToolTip("Remove Component");
	hlayout->addWidget(btnClose);

	auto mainLayout = new QHBoxLayout(mainFrame);
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(btnExpand);

	mainFrame->setLayout(mainLayout);

	Parent->setItemWidget(head, 0, mainFrame);

	content = new ComponentView(Comp, Parent);
	connect(content, &ComponentView::updateResList, this, &Expander::updateResList);
	
	auto child = new QTreeWidgetItem(head);
	child->setDisabled(true);
	Parent->setItemWidget(child, 0, content);

	connect(content, &ComponentView::componentEdited, this, &Expander::componentEdited);
	connect(btnExpand, &QToolButton::pressed, this, &Expander::expClicked);
	connect(btnClose, &QToolButton::clicked, this, &Expander::clsClicked);
}

void Expander::reset(Kite::KComponent *Comp) {
	chandle = Comp->getHandle();
	QString name = Comp->getType().c_str();
	if (name == "Logic") {
		name.append(" ");
		name.append(Comp->getName().c_str());
	}
	head->setText(0, name);
	btnExpand->setText(name);
	highlight(false);
	content->reset(Comp);
}

void Expander::highlight(bool Selected) {
	if (Selected) {
		btnExpand->setStyleSheet("background-color: green;");
	} else {
		btnExpand->setStyleSheet("");
	}
}

void Expander::expClicked() {
	if (expandable) {
		head->setExpanded(!head->isExpanded());
		emit(expandClicked(chandle, ctype));
	}
}

void Expander::clsClicked() {
	emit(closeClicked(chandle, ctype));
}