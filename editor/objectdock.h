#ifndef OBJECTDOCK_H
#define OBJECTDOCK_H

#include <qdockwidget.h>
#include <qstringlist.h>
#include <Kite/core/kprefab.h>
#include <Kite/core/klistener.h>

class QUndoStack;
class QUndoGroup;
class QTreeWidget;
class QAction;
class QLable;
class QTreeWidgetItem;

namespace Kite {
	class KResource;
	class KEntityManager;
	class KEntity;
}

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
	explicit ObjectDock(QUndoGroup *UndoGroup, QWidget *parent = 0);
	~ObjectDock();
	
	void initePanel();
	void clearPanel();

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
	Kite::KResource *requestCreateResource(Kite::RTypes Type);
	void requestResName(Kite::RTypes Type, QStringList &List);
	Kite::KResource *requestGetResource(const QString &Name);

private slots:
	void entityChecked(QTreeWidgetItem *Item, int Col);
	void clicked();
	void rightClicked(const QPoint & pos);
	void addChild();
	void addRoot();
	void remove();
	void rename();
	void prefab();
	void search(const QString &Pharase);
	void cut();
	void copy();
	void paste();

private:
	void setupTree();
	void setupActions();
	void setupHTools();
	//void actionsControl(ActionsState State);
	void recursiveLoad(Kite::KEntity *Root);
	QTreeWidgetItem *loadChilds(Kite::KEntityManager *Eman, const Kite::KHandle &Entity, QTreeWidgetItem *Parrent);
	Kite::KHandle createObject(bool Prefab);

	QUndoStack *undoStack;
	QTreeWidget *objTree;

	QAction *actAddRoot;
	QAction *actAddChild;
	QAction *actRename;
	QAction *actRemove;
	QAction *actPrefab;
	QAction *actCopy;
	QAction *actCut;
	QAction *actPaste;

	Clipboard clipb;
	Kite::KResource *currRes;
	Kite::KEntityManager *currEman;
	Kite::KEntityManager *preEman;
};

#endif // OBJECTDOCK_H
