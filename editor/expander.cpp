#include "expander.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QIcon>
#include <QHBoxLayout>

Expander::Expander(const QString& Text, const QIcon &Icon, QWidget* Parent, QTreeWidgetItem* TreeItem):
	QToolButton(Parent), item(TreeItem)
{
	this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	this->setIcon(Icon);
	this->setText(Text);
	connect(this, SIGNAL(pressed()), this, SLOT(ButtonPressed()));
}

void Expander::ButtonPressed() {
	item->setExpanded(!item->isExpanded());
}