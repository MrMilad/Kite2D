#ifndef OBJECTTREE_H
#define OBJECTTREE_H

#include <qtreewidget.h>
#include <qstringlist.h>
#include <qaction.h>
#include <Kite/core/kscene.h>
#include <Kite/core/kentity.h>
#include <Kite/core/kentitymanager.h>

enum ObjectActState {
	OA_ON_ITEM,
	OA_ON_INITE
};

class ObjectTree : public QTreeWidget {
	Q_OBJECT

public:
	explicit ObjectTree(QWidget *parent = 0);
	~ObjectTree();

	inline QFrame *getHeaderTools() { return htools; }

public slots:
	void sceneEdit(Kite::KScene *Scene);
	void sceneDelete(Kite::KScene *Scene);

signals:
	void objectAdded(Kite::KEntity *Obj);
	void objectSelected(Kite::KResource *Res);
	void objectDelete(Kite::KResource *Res);
	void objectRenamed(Kite::KResource *Res);

private slots:
	void entityChecked(QTreeWidgetItem *Item, int Col);
	void actClicked();
	void actRClicked(const QPoint & pos);
	void actAdd();
	void actRemove();
	void actRename();
	void actSearch(const QString &Pharase);

protected:
	void focusInEvent(QFocusEvent *Event) override;
	void focusOutEvent(QFocusEvent *Event) override;

private:
	void setupActions();
	void setupShortcuts();
	void setupHTools();
	void actionsControl(ObjectActState State);
	void loadChilds(Kite::KEntityManager *Eman, const Kite::KHandle &Entity, QTreeWidgetItem *Parrent);

	QAction *addRootObj;
	QAction *addChildObj;
	QAction *renameObj;
	QAction *remObj;
	QFrame *htools;
	QLineEdit *ledit;
	QVector<QShortcut *> shortcuts;
	Kite::KScene *scene;
};

#endif // OBJECTTREE_H
