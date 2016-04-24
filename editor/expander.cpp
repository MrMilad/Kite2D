#include "expander.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QIcon>

Expander::Expander(const QString& Text, const QIcon &Icon, QWidget* Parent, QTreeWidgetItem* TreeItem):
	QToolButton(Parent), m_pItem(TreeItem)
{
	this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	this->setIcon(Icon);
	this->setText(Text);
	connect(this, SIGNAL(pressed()), this, SLOT(ButtonPressed()));
}

void Expander::ButtonPressed() {
	m_pItem->setExpanded(!m_pItem->isExpanded());
}