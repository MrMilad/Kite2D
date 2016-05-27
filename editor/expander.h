#include <qframe.h>
#include <qstring.h>

class QTreeWidget;
class QTreeWidgetItem;
class QIcon;

class Expander : public QFrame {
	Q_OBJECT
public:
	Expander(const QString& Text, const QIcon &Icon, QTreeWidgetItem *TreeItem, QWidget* Parent);

	inline void setName(const QString &Name) { cname = Name; }

	inline void setType(const QString &Type) { ctype = Type; }

signals:
	void closeClicked(const QString &CName, const QString &CType, QTreeWidgetItem *Item);

private slots:
	void expandPressed();
	void closePressed();

private:
	QString cname;
	QString ctype;
	QTreeWidgetItem	*item;
};