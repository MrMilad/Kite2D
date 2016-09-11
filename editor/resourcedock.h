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
#include <Kite/core/klistener.h>
#include <Kite/meta/kmetaclass.h>
#include "shared.h"

class ResourceDock : public QDockWidget, public Kite::KListener {
	Q_OBJECT

public:
	// pass empty string if there is no dictionary
	explicit ResourceDock(QWidget *parent = 0);
	~ResourceDock();

	bool openResource(const QString &Address, Kite::RTypes Type, bool WarnExist = true);
	Kite::KResource *getResource(const QString &Name);
	Kite::KResource *addResource(Kite::RTypes Type);
	Kite::KResource *addResourceInternal(Kite::RTypes Type, const QString &Name);
	void filterByType(Kite::RTypes Type, QStringList &List);
	void filterByTypeRes(Kite::RTypes Type, QList<const Kite::KResource *> &List);
	void selectResource(const QString &Name);
	void manageUsedResource(const QHash<size_t, QVector<Kite::KMetaProperty>> *ResComponents);
	void clearResources();
	const std::vector<Kite::KResource *> &dumpResource();
	inline void setCurrentDirectory(const QString &Directory) { currDirectory = Directory; }
	void resetModify();

	Kite::RecieveTypes onMessage(Kite::KMessage *Message, Kite::MessageScope Scope);
	
signals:
	void resourceAdded(Kite::KResource *Res);
	void resourceSelected(Kite::KResource *Res);
	void resourceEdit(Kite::KResource *Res);
	void resourceDelete(Kite::KResource *Res);
	void resourceSave(Kite::KResource *Res);

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
	void setupCategories();

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
	//QHash<QString, Kite::KResource *> dictinary;
	QMultiHash<size_t, QPair<QString, QString>> formats; // <resource name, <format, icon>>
	Kite::KResourceManager rman;
};

#endif // RESOURCEDOCK_H
