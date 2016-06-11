#ifndef OBJECTDOCK_H
#define OBJECTDOCK_H

#include <qdockwidget.h>
#include <qtreewidget.h>
#include <qstringlist.h>
#include <qaction.h>
#include <qlabel.h>
#include <Kite/core/kscene.h>
#include <Kite/core/kentity.h>
#include <Kite/core/kentitymanager.h>
#include "shared.h"

class ObjectDock : public QDockWidget {
	Q_OBJECT

public:
	explicit ObjectDock(QWidget *parent = 0);
	~ObjectDock();

public slots:
	void sceneEdit(Kite::KResource *Scene);
	void sceneDelete(Kite::KResource *Scene);

signals:
	void objectAdded(Kite::KEntityManager *EMan, Kite::KEntity *Entity);
	void objectSelected(Kite::KEntityManager *EMan, Kite::KEntity *Entity);
	void objectDelete(Kite::KEntityManager *EMan, Kite::KEntity *Entity);
	void objectRenamed(Kite::KEntityManager *EMan, Kite::KEntity *Entity);

private slots:
	void entityChecked(QTreeWidgetItem *Item, int Col);
	void actClicked();
	void actRClicked(const QPoint & pos);
	void actAddChild();
	void actAddRoot();
	void actRemove();
	void actRename();
	void actSearch(const QString &Pharase);

private:
	void setupTree();
	void setupActions();
	void setupHTools();
	void actionsControl(ActionsState State);
	void loadChilds(Kite::KEntityManager *Eman, const Kite::KHandle &Entity, QTreeWidgetItem *Parrent);
	QString getAvailName();

	QTreeWidget *objTree;
	QAction *addRootObj;
	QAction *addChildObj;
	QAction *renameObj;
	QAction *remObj;
	QFrame *htools;
	QLineEdit *ledit;
	QLabel *hlabel;
	Kite::KScene *currScene;
};

#endif // OBJECTDOCK_H
