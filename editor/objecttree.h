#ifndef OBJECTTREE_H
#define OBJECTTREE_H

#include <qtreewidget.h>
#include <qstringlist.h>
#include <qaction.h>
#include <qlabel.h>
#include <Kite/core/kscene.h>
#include <Kite/core/kentity.h>
#include <Kite/core/kentitymanager.h>
#include "shared.h"

class ObjectTree : public QTreeWidget {
	Q_OBJECT

public:
	explicit ObjectTree(QWidget *parent = 0);
	~ObjectTree();

	inline QFrame *getHeaderTools() { return htools; }

public slots:
	void sceneEdit(Kite::KResource *Scene);
	void sceneDelete(Kite::KResource *Scene);

signals:
	void objectAdded(Kite::KEntity *Obj);
	void objectSelected(Kite::KEntity *Obj);
	void objectDelete(Kite::KEntity *Obj);
	void objectRenamed(Kite::KEntity *RObjes);

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
	void setupActions();
	void setupHTools();
	void actionsControl(ActionsState State);
	void loadChilds(Kite::KEntityManager *Eman, const Kite::KHandle &Entity, QTreeWidgetItem *Parrent);
	QString getAvailName();

	QAction *addRootObj;
	QAction *addChildObj;
	QAction *renameObj;
	QAction *remObj;
	QFrame *htools;
	QLineEdit *ledit;
	QLabel *hlabel;
	Kite::KScene *currScene;
};

#endif // OBJECTTREE_H
