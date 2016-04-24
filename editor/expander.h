#include <qtoolbutton.h>
class QTreeWidget;
class QTreeWidgetItem;
class QIcon;

class Expander : public QToolButton {
	Q_OBJECT
public:
	Expander(const QString& Text, const QIcon &Icon,
					 QWidget* Parent, QTreeWidgetItem* TreeItem);

private slots:
	void ButtonPressed();

private:
	QTreeWidgetItem* m_pItem;
};