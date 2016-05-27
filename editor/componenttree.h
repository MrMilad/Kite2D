#ifndef COMPONENTTREE_H
#define COMPONENTTREE_H

#include <qtreewidget.h>
#include <qstringlist.h>
#include <qaction.h>
#include <qlabel.h>
#include <Kite/core/kentity.h>
#include <Kite/core/kcomponent.h>
#include <Kite/meta/kmetamanager.h>
#include <Kite/meta/kmetabase.h>
#include <Kite/meta/kmetaclass.h>
#include <Kite/meta/kmetapod.h>
#include "shared.h"

class QMenu;
class QFormLayout;

class ComponentTree : public QTreeWidget {
	Q_OBJECT

public:
	explicit ComponentTree(QWidget *parent = 0);
	~ComponentTree();

	void setupTypes(const QStringList &TypeList);
	inline void setResDictionary(const QHash<QString, Kite::KResource *> *Dictionary) { resDict = Dictionary; }
	inline QFrame *getHeaderTools() { return htools; }

public slots:
	void entityEdit(Kite::KEntity *Entity);
	void entityDelete(Kite::KEntity *Entity);

signals:
	void componentAdded(Kite::KEntity *Entity, Kite::KComponent *Com);
	void componentDelete(Kite::KEntity *Entity, Kite::KComponent *Com);
	void componentEdited(Kite::KEntity *Entity, Kite::KComponent *Com);

private slots:
	void actRClicked(const QPoint & pos);
	void actAdd(QAction *Action);
	void actAddDef();
	void actRemove(const QString &CName, const QString &CType, QTreeWidgetItem *Item);
	void actEdit();
	void actClear();
	void actSearch(const QString &Pharase);

private:
	void setupActions();
	void setupHTools();
	void actionsControl(ActionsState State);
	QString getAvailName(Kite::KEntity *Entity);
	bool eventFilter(QObject *object, QEvent *event); // resource combo box event filter
	void removeComponentGUI(QTreeWidgetItem *Item);
	void createComponent(const Kite::KEntity *Entity, const Kite::KComponent *Component);
	void bindProperties(const Kite::KEntity *Entity, const Kite::KComponent *Component, QFrame *Frame);
	void addGUIItem(QFormLayout *Layout, const Kite::KMetaBase *Meta, const Kite::KEntity *Entity,
					const Kite::KComponent *Component, const Kite::KMetaProperty *PropMeta);

	QAction *addDefComp;
	QFrame *htools;
	QLineEdit *ledit;
	QLabel *hlabel;
	QMenu *mtypes;
	Kite::KEntity *currEntity;
	QStringList types;
	Kite::KMetaManager mman;
	const QHash<QString, Kite::KResource *> *resDict;
};

#endif // COMPONENTTREE_H
