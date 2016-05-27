#include "expander.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <qtoolbutton.h>
#include <QIcon>
#include <QHBoxLayout>

Expander::Expander(const QString& Text, const QIcon &Icon, QTreeWidgetItem* TreeItem, QWidget* Parent):
	QFrame(Parent), item(TreeItem)
{
	auto hlayout = new QHBoxLayout(this);
	hlayout->setMargin(0);
	hlayout->setSpacing(0);

	auto btnExpand = new QToolButton(this);
	btnExpand->setIcon(Icon);
	btnExpand->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	btnExpand->setText(Text);
	btnExpand->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	hlayout->addWidget(btnExpand);

	auto btnClose = new QToolButton(this);
	btnClose->setMaximumWidth(16);
	btnClose->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	btnClose->setIcon(QIcon(":/icons/close"));
	btnClose->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hlayout->addWidget(btnClose);

	this->setLayout(hlayout);

	connect(btnExpand, &QToolButton::pressed, this, &Expander::expandPressed);
	connect(btnClose, &QToolButton::clicked, this, &Expander::closePressed);
}

void Expander::expandPressed() {
	item->setExpanded(!item->isExpanded());
}

void Expander::closePressed() {
	emit(closeClicked(cname, ctype, item));
}