#ifndef OBJECTDOCK_H
#define OBJECTDOCK_H

#include <qdockwidget.h>
#include <qtreewidget.h>
#include <qstringlist.h>
#include <qaction.h>
#include <qlabel.h>
#include <Kite/core/kscene.h>
#include <Kite/core/kentity.h>
#include <Kite/core/klistener.h>
#include <Kite/core/kentitymanager.h>
#include "shared.h"

struct Clipboard {
	bool isCopy;
	Kite::KPrefab data;

	Clipboard() :
		isCopy(false),
		data("clipboard") {}
};

class ObjectDock : public QDockWidget, public Kite::KListener {
	Q_OBJECT

public:
	explicit ObjectDock(QWidget *parent = 0);
	~ObjectDock();

	Kite::RecieveTypes onMessage(Kite::KMessage *Message, Kite::MessageScope Scope);

public slots:
	void installEntityCallback(Kite::KResource *Res);
	void resEdit(Kite::KResource *Scene);
	void resDelete(Kite::KResource *Scene);
	void revertPrefab(Kite::KEntity *Entity);
	void applyPrefab(Kite::KEntity *Entity);

signals:
	void objectAdded(Kite::KEntityManager *EMan, Kite::KEntity *Entity);
	void objectSelected(Kite::KEntityManager *EMan, Kite::KEntity *Entity, bool isPrefab);
	void objectDelete(Kite::KEntityManager *EMan, Kite::KEntity *Entity);
	void objectRenamed(Kite::KEntityManager *EMan, Kite::KEntity *Entity);
	Kite::KResource *requestCreateResource(const QString &Type);
	void requestResName(const QString &Type, QStringList &List);
	Kite::KResource *requestGetResource(const QString &Name);

private slots:
	void entityChecked(QTreeWidgetItem *Item, int Col);
	void actClicked();
	void actRClicked(const QPoint & pos);
	void actAddChild();
	void actAddRoot();
	void actRemove();
	void actRename();
	void actPrefab();
	void actSearch(const QString &Pharase);
	void actCut();
	void actCopy();
	void actPaste();

private:
	void setupTree();
	void setupActions();
	void setupHTools();
	void actionsControl(ActionsState State);
	void recursiveLoad(Kite::KEntity *Root);
	QTreeWidgetItem *loadChilds(Kite::KEntityManager *Eman, const Kite::KHandle &Entity, QTreeWidgetItem *Parrent);
	Kite::KHandle createObject(bool Prefab);

	QTreeWidget *objTree;
	QAction *addRootObj;
	QAction *addChildObj;
	QAction *renameObj;
	QAction *remObj;
	QAction *prefab;
	QAction *copy;
	QAction *cut;
	QAction *paste;
	Clipboard clipb;
	QFrame *htools;
	QLineEdit *ledit;
	QLabel *hlabel;
	Kite::KResource *currRes;
	Kite::KEntityManager *currEman;
	Kite::KEntityManager *preEman;
};

#endif // OBJECTDOCK_H
