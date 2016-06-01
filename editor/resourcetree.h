#ifndef RESOURCETREE_H
#define RESOURCETREE_H

#include <qtreewidget.h>
#include <qstringlist.h>
#include <qaction.h>
#include <qhash.h>
#include <unordered_map>
#include <Kite/core/kresource.h>
#include <Kite/core/kresourcemanager.h>
#include <Kite/meta/kmetaclass.h>
#include "shared.h"

class ResourceTree : public QTreeWidget {
	Q_OBJECT

public:
	explicit ResourceTree(QWidget *parent = 0);
	~ResourceTree();

	void setupCategories(const QStringList &CatList);
	bool openResource(const QString &Address, const QString &Type);
	void clearResources();
	void filterByType(const QString &Type, QStringList &List);
	void manageUsedResource(const QHash<QString, QVector<Kite::KMetaProperty>> *ResComponents);

	inline auto const getDictionary() const { return &dictinary; }
	inline QFrame *getHeaderTools() { return htools; }
	
	const std::unordered_map<std::string, std::string> *getKiteDictionary() const;

signals:
	void resourceAdded(Kite::KResource *Res);
	void resourceSelected(Kite::KResource *Res);
	void resourceEdit(Kite::KResource *Res);
	void resourceDelete(Kite::KResource *Res);
	void resourceOpen(Kite::KResource *Res);

private slots:
	void actDoubleClicked(QTreeWidgetItem * item, int column);
	void actClicked();
	void actRClicked(const QPoint & pos);
	void actAdd();
	void actOpen();
	void actSave();
	void actSaveAs();
	void actEdit();
	void actRemove();
	void actSearch(const QString &Pharase);

private:
	void setupActions();
	void setupHTools();
	void actionsControl(ActionsState State);

	QAction *addRes;
	QAction *openRes;
	QAction *saveRes;
	QAction *saveAsRes;
	QAction *editRes;
	QAction *remRes;
	QFrame *htools;
	QLineEdit *ledit;
	QHash<QString, Kite::KResource *> dictinary;
	std::unordered_map<std::string, std::string> *kiteDictionary;
	Kite::KResourceManager rman;
};

#endif // RESOURCETREE_H
