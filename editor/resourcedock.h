#ifndef RESOURCEDOCK_H
#define RESOURCEDOCK_H

#include <qdockwidget.h>
#include <qhash.h>
#include <qvector.h>
#include <qstringlist.h>
#include <Kite/meta/kmetaclass.h>
#include <Kite/core/kresource.h>

struct ResInfo {
	QList<QPair<QString, QString>> resFI;	// <format, icon>
	void (*onResRemove)(Kite::KResource *, Kite::KResource *); // dependency and pointer checker (avoid pointer dangling in resources)

	ResInfo() :
		onResRemove(nullptr) {}
};

class QFileSystemWatcher;
class QTreeWidget;
class QAction;
class QTreeWidgetItem;

namespace Kite {
	class KResource;
	class KResourceManager;
}

struct DumpItem {
	QString name;
	QString address;
	Kite::RTypes type;

	DumpItem(const QString &Name, const QString &Address, Kite::RTypes Type) :
		name(Name), address(Address), type(Type) {}
};

class ResourceDock : public QDockWidget{
	Q_OBJECT

public:
	explicit ResourceDock(QWidget *parent = 0);
	~ResourceDock();

	bool addExisting(const QString &Address, Kite::RTypes Type, bool WarnExist = true);
	bool addNew(Kite::RTypes Type, const QString &Name, bool AskName = false);

	Kite::KResource *load(Kite::KIStream *Stream, const QString &Name);

	void filter(Kite::RTypes Type, QStringList &List);
	void select(const QString &Name);
	void dump(QList<DumpItem> &List);

	void initePanel();
	void clearPanel();

	//void resetModify();
	inline void setResDirectory(const QString &Dir) { resDir = Dir + "/"; }
	void manageUsedResource(const QHash<size_t, QVector<Kite::KMetaProperty>> *ResComponents);
	
signals:
	void resAdded(const QString &Name, Kite::RTypes Type);
	void resSelected(const QString &Name, Kite::RTypes Type);
	void resEdit(const QString &Name, Kite::RTypes Type);
	void resDeleted(const QString &Name, Kite::RTypes Type);

private slots:
	void dblClickedAct(QTreeWidgetItem *item, int column);
	void clickedAct();
	void rightClickedAct(const QPoint & pos);
	void addNewAct();
	void addExistAct();
	void editAct();
	void removeAct();
	void searchAct(const QString &Pharase);

private:
	void setupTree();
	void setupActions();
	void setupHTools();
	void setupCategories();

	QAction *actAddNew;
	QAction *actAddExist;
	QAction *actEdit;
	QAction *actRemove;

	QString resDir;
	QTreeWidget *resTree;
	QHash<size_t, ResInfo> resInfoMap; // <type, ResInfo>
	QHash<QString, QPair<QString, Kite::RTypes>> resMap; // <name, <address, type>> loaded resources
	QStringList fpaths;
	QFileSystemWatcher *fwatcher;
};

#endif // RESOURCEDOCK_H
