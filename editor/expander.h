#ifndef EXPANDER_H
#define EXPANDER_H

#include <Kite/core/kcomponent.h>
#include <qframe.h>
#include <qstring.h>
#include <qtreewidget.h>
#include <qvariant.h>
#include <Kite/core/kresource.h>

class QPushButton;
class ComponentView;

class Expander : public QObject {
	Q_OBJECT
public:
	Expander(Kite::KComponent *Comp, QTreeWidget* Parent);

	inline Kite::KCTypes getCType() const { return ctype; }
	inline const Kite::KHandle &getCHndle() const { return chandle; }
	inline auto getTreeItem() const { return head; }
	void reset(Kite::KComponent *Comp);
	void highlight(bool Selected);
	
signals:
	void expandClicked(Kite::KHandle CHandle);
	void closeClicked(Kite::KHandle CHandle);
	void componentEdited(Kite::KHandle Chandle, const QString &Pname, QVariant &Value);
	void updateResList(const QString &Type, QStringList &List);

private slots:
	void expClicked();
	void clsClicked();

private:
	bool expandable;
	QTreeWidgetItem *head;
	ComponentView *content;
	QPushButton *btnExpand;
	Kite::KHandle chandle;
	Kite::KCTypes ctype;
};

#endif // EXPANDER_H