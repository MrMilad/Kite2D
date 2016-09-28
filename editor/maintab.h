#ifndef MAINTAB_H
#define MAINTAB_H

#include <qlist.h>
#include <qtabwidget.h>
#include <qhash.h>
#include <qaction.h>
#include <qpair.h>
#include <qdockwidget.h>
#include <Kite/core/kresource.h>
#include <qstandarditemmodel.h>
#include "tabwidget.h"
#include "shared.h"

class QEvent;

class MainTab : public QTabWidget {
	Q_OBJECT

public:
	explicit MainTab(KiteInfo *KInfo ,QWidget *parent = 0);
	void setScene(QWidget *Scene);
	bool isOpen(Kite::KResource *Res);
	~MainTab();

signals:
	void requestResList(Kite::RTypes Type, QList<const Kite::KResource *> &List);
	Kite::KResource *requestRes(const QString &Name);
	Kite::KResource *requestAddRes(Kite::RTypes Type, const QString &Name);

public slots:
	void saveAll();
	void saveRes(Kite::KResource *Res);
	void openTabs(Kite::KResource *Res);
	void selectResource(Kite::KResource *Res);
	void closeResource(const Kite::KResource *Res);
	void reloadRes(const Kite::KResource *Res);
	void reloadResType(Kite::RTypes Type);

	void resourceAdded(const Kite::KResource *Res);
	void resourceDeleted(Kite::RTypes Type);

private slots:
	void closeTab();
	void unpinTab();

	void closeDock();
	void pinDock();

protected:
	void focusInEvent(QFocusEvent *Event) override;
	void focusOutEvent(QFocusEvent *Event) override;

	bool eventFilter(QObject *Obj, QEvent *Event);

private:
	void registerTabs();
	QWidget *createTabHead(Kite::KResource *);
	int createTab(QWidget *Widget, Kite::KResource *ResPtr);
	void deleteTab(QWidget *Widget);
	void deleteDock(QDockWidget *Dock);

	QWidget *scene;
	KiteInfo *kinfo;
	QVector<QShortcut *> shortcuts;
	QHash<size_t, TabWidget *(*)(Kite::KResource *, KiteInfo *, QWidget *)> tabFactory; // <type, factory, inite>>
	QHash<const Kite::KResource *, QPair<TabWidget *, QDockWidget *>> resMap;
};

#endif // MAINTAB_H
