#ifndef MAINTAB_H
#define MAINTAB_H

#include <qlist.h>
#include <qtabwidget.h>
#include <qhash.h>
#include <qpair.h>
#include <Kite/core/kresource.h>
#include "tabwidget.h"

class QDockWidget;
class QEvent;
class QUndoStack;
class QUndoGroup;
class QWidget;
class QAction;

class FocusCover : public QWidget {
public:
	FocusCover(QWidget *par);

protected:
	void mousePressEvent(QMouseEvent *Event);
	bool eventFilter(QObject *Obj, QEvent *Event);
};

class MainTab : public QTabWidget {
	Q_OBJECT

public:
	explicit MainTab(QUndoGroup *UGroup, KiteInfo *KInfo ,QWidget *parent = 0);
	bool isOpen(const QString &Name);
	bool needSave(const QString &Name);
	bool needSave();
	~MainTab();

signals:
	void tabSelected(const QString &Name);
	void tabClose(const QString &Name);

	void reqResList(Kite::RTypes Type, QStringList &List);
	Kite::KResource *reqResLoad(Kite::KIStream *Stream, const QString &Name);
	bool reqResAdd(Kite::RTypes Type, const QString &Name, bool AskName);

public slots:
	void saveAll();
	void save(const QString &Name);
	void open(const QString &Name, Kite::RTypes Type);
	void select(const QString &Name, Kite::RTypes Type);
	void close(const QString &Name, Kite::RTypes Type);
	void reload(const QString &Name);
	void reloadType(Kite::RTypes Type);

	void resourceAdded(const QString &Name, Kite::RTypes Type);
	void resourceDeleted(const QString &Name, Kite::RTypes Type);

private slots:
	void closeTabAct();
	void unpinTabAct();
	void tabChanged(int index);
	void cleanChanged(bool clean);

	void closeDockAct();
	void pinDockAct();

protected:
	bool eventFilter(QObject *Obj, QEvent *Event);

private:
	void registerTabs();
	QWidget *createTabHead(Kite::KResource *);
	int createTab(QWidget *Widget, Kite::KResource *ResPtr);
	void deleteTab(QWidget *Widget);
	void deleteDock(QDockWidget *Dock);

	struct TabInfo {
		TabWidget *widget;
		QDockWidget *dock;
		QUndoStack *ustack;

		TabInfo() :
			widget(nullptr),
			dock(nullptr),
			ustack(nullptr)
		{}

		TabInfo(TabWidget *Tab, QDockWidget *Dock, QUndoStack *Stack) :
			widget(Tab), dock(Dock), ustack(Stack) {}
	};

	FocusCover *fcover;
	QUndoGroup *ugroup;
	KiteInfo *kinfo;
	QHash<size_t, TabWidget *(*)(Kite::KResource *, Kite::KIStream *Stream, QUndoStack *UStack, KiteInfo *, QWidget *)> tabFactory; // <type, factory>
	QHash<QString, TabInfo> resMap;
};

#endif // MAINTAB_H
