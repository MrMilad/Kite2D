#ifndef RESOURCEDOCK_H
#define RESOURCEDOCK_H

#include <qdockwidget.h>
#include <qtreewidget.h>
#include <qstringlist.h>
#include <qaction.h>
#include <qhash.h>
#include <unordered_map>
#include <Kite/core/kresource.h>
#include <Kite/core/kresourcemanager.h>
#include <Kite/meta/kmetaclass.h>
#include "shared.h"

class ResourceDock : public QDockWidget {
	Q_OBJECT

public:
	explicit ResourceDock(QWidget *parent = 0);
	~ResourceDock();

	void setupCategories(const QStringList &CatList);
	bool openResource(const QString &Address, const QString &Type);
	Kite::KResource *getResource(const QString &Name);
	Kite::KResource *addResource(const QString &Type);
	void filterByType(const QString &Type, QStringList &List);
	void selectResource(const QString &Name);
	void manageUsedResource(const QHash<QString, QVector<Kite::KMetaProperty>> *ResComponents);
	void clearResources();

	inline void setCurrentDirectory(const QString &Directory) { currDirectory = Directory; }
	inline auto const getDictionary() const { return &dictinary; }
	
	const std::unordered_map<std::string, std::string> *getKiteDictionary(const QString &AddressPrefix) const;

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
	void setupTree();
	void setupActions();
	void setupHTools();
	void actionsControl(ActionsState State);

	QTreeWidget *resTree;
	QAction *addRes;
	QAction *openRes;
	QAction *saveRes;
	QAction *saveAsRes;
	QAction *editRes;
	QAction *remRes;
	QFrame *htools;
	QLineEdit *ledit;
	QString currDirectory;
	QHash<QString, Kite::KResource *> dictinary;
	QHash<QString, QPair<QString, QString>> formats; // <resource name, <format, icon>>
	std::unordered_map<std::string, std::string> *kiteDictionary;
	Kite::KResourceManager rman;
};

#endif // RESOURCEDOCK_H
