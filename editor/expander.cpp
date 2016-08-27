#include "expander.h"
#include <QtWidgets>
#include <comproperty.h>

Expander::Expander(Kite::KComponent *Comp, QTreeWidget *Parent):
	QObject(Parent), expandable(true)
{
	chandle = Comp->getHandle();
	ctype = Comp->getType();
	QString name = Comp->getTypeName().c_str();
	if (ctype == Kite::CTypes::Logic) {
		name.append(" ");
		name.append(Comp->getName().c_str());
	}

	head = new QTreeWidgetItem(Parent);
	head->setFlags(head->flags() ^ Qt::ItemIsDropEnabled);
	head->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	head->setText(0, name);
	head->setHidden(true);

	auto headFrame = new QFrame(Parent);
	auto headLayout = new QVBoxLayout(headFrame);
	headLayout->setMargin(0);
	headLayout->addSpacing(3);

	btnExpand = new QPushButton(Parent);
	btnExpand->setObjectName("btnExpand");
	btnExpand->setIcon(QIcon(":/icons/exp"));
	btnExpand->setText(name);
	btnExpand->setFocusPolicy(Qt::NoFocus);
	btnExpand->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	btnExpand->setStyleSheet("QPushButton { text-align: left;"
							 "color: rgb(255, 255, 255);"
							 "background-color: rgb(38, 38, 38);"
							 "border: 1px solid rgb(38, 38, 38);"
							 "border-radius: 3px;}");
	headLayout->addWidget(btnExpand);

	auto hlayout = new QHBoxLayout(btnExpand);
	hlayout->setMargin(0);
	hlayout->setSpacing(0);
	hlayout->addStretch(1);

	// dependecy label
	if (!Comp->getDependency().empty()) {
		auto lblDep = new QLabel(btnExpand);
		lblDep->setText("<img src=\":/icons/depend\" height=\"16\" width=\"16\" >");
		lblDep->setStyleSheet("QToolTip { border: 1px solid #2c2c2c; background-color: #242424; color: white;}");
		QString tooltip("<font color=\"orange\">Dependency List:</font>");
		auto depList = Comp->getDependency();
		for (auto it = depList.begin(); it != depList.end(); ++it) {
			tooltip += "\n" + QString(Kite::getCTypesName((*it)).c_str());
		}
		lblDep->setToolTip(tooltip);
		hlayout->addWidget(lblDep);
		hlayout->addSpacing(2);
	}

	// remove on zero dependency label
	if (Comp->getRemoveOnZeroDep()) {
		auto lblZeroDep = new QLabel(btnExpand);
		lblZeroDep->setText("<img src=\":/icons/remzdep\" height=\"16\" width=\"16\" >");
		lblZeroDep->setStyleSheet("QToolTip { border: 1px solid #2c2c2c; background-color: #242424; color: white;}");
		lblZeroDep->setToolTip("<font color=\"orange\">will be removed if there is no dependency on it</font>");
		hlayout->addWidget(lblZeroDep);
		hlayout->addSpacing(2);
	}

	// remove button
	auto btnClose = new QToolButton(btnExpand);
	btnClose->setContentsMargins(0, 0, 0, 0); 
	btnClose->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	btnClose->setIcon(QIcon(":/icons/close"));
	btnClose->setIconSize(QSize(8, 8));
	btnClose->setToolButtonStyle(Qt::ToolButtonIconOnly);
	btnClose->setAutoRaise(true);
	btnClose->setToolTip("Remove Component");
	hlayout->addWidget(btnClose);

	Parent->setItemWidget(head, 0, headFrame);

	content = new ComponentView(Comp, Parent);
	content->setStyleSheet("QFrame {background-color: rgb(58, 58, 58);\n"
						   "border: 1px solid rgb(58, 58, 58);\n"
						   "border-bottom-left-radius: 3px;\n"
						   "border-bottom-right-radius: 3px;}");
	connect(content, &ComponentView::updateResList, this, &Expander::updateResList);
	
	auto child = new QTreeWidgetItem(head);
	child->setDisabled(true);
	Parent->setItemWidget(child, 0, content);

	connect(content, &ComponentView::componentEdited, this, &Expander::componentEdited);
	connect(btnExpand, &QPushButton::pressed, this, &Expander::expClicked);
	connect(btnClose, &QToolButton::clicked, this, &Expander::clsClicked);
}

void Expander::reset(Kite::KComponent *Comp) {
	chandle = Comp->getHandle();
	QString name = Comp->getTypeName().c_str();
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
	head->setHidden(!Selected);
}

void Expander::expClicked() {
	if (expandable) {
		head->setExpanded(!head->isExpanded());
		emit(expandClicked(chandle));
	}
}

void Expander::clsClicked() {
	emit(closeClicked(chandle));
}