#ifndef COMPONENTDOCK_H
#define COMPONENTDOCK_H

#include <qdockwidget.h>
#include <qtreewidget.h>
#include <qstringlist.h>
#include <qaction.h>
#include <qlabel.h>
#include <qhash.h>
#include <qlist.h>
#include <qvector.h>
#include <Kite/meta/kmetamanager.h>
#include <Kite/core/kentitymanager.h>
#include <Kite/core/kentity.h>
#include "shared.h"

class QMenu;
class QFormLayout;
class Expander;

namespace priv {
	struct TreeItemPool {
		QHash<QString, Expander *> fixedPool;
		QVector<Expander *> logicPool;
	};
}

class ComponentDock : public QDockWidget {
	Q_OBJECT

public:
	explicit ComponentDock(QWidget *parent = 0);
	~ComponentDock();

	void inite(const QStringList &TypeList, const QHash<QString, Kite::KResource *> *Dictionary);

public slots:
	void entityEdit(Kite::KEntityManager *Eman, Kite::KEntity *Entity);
	void entityDelete(Kite::KEntityManager *Eman, Kite::KEntity *Entity);

signals:
	void componentAdded(Kite::KEntity *Entity, Kite::KComponent *Component);
	void componentDelete(Kite::KEntity *Entity, Kite::KComponent *Component);
	void componentEdited(Kite::KEntity *Entity, Kite::KComponent *Component, const QString &PName);

private slots:
	void actExeOrder();
	void actCollAll();
	void actAdd(QAction *Action);
	void actAddDef();
	void actRemove(Kite::KHandle CHandle, const QString &CType);
	void actEdit(Kite::KHandle Chandle, const QString &CType, const QString &Pname, QVariant &Value);
	void actClear();
	void actSearch(const QString &Pharase);

private:
	void setupTree();
	void setupActions();
	void setupHTools();
	void actionsControl(ActionsState State);
	QString getAvailName(Kite::KEntity *Entity);
	Expander *createCom(const QString &CType);
	void addLogicToPool(int Count);
	void initePool(const QStringList &TypeList, unsigned int LogicCount = 15);
	void fetchFromPool(Kite::KComponent *Comp);
	void putIntoPool(Expander *Exp);

	QTreeWidget *comTree;
	QAction *addDefComp;
	QAction *collAll;
	QAction *exeOrder;
	QFrame *htools;
	QLineEdit *ledit;
	QLabel *hlabel;
	QMenu *mtypes;
	Kite::KHandle currEntity;
	QStringList types;
	Kite::KMetaManager mman;
	Kite::KEntityManager *eman;
	priv::TreeItemPool treePool;
	QList<Expander *> treeList;
	const QHash<QString, Kite::KResource *> *resDict;
};

#endif // COMPONENTDOCK_H
