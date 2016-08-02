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

class MainTab : public QTabWidget {
	Q_OBJECT

public:
	explicit MainTab(KiteInfo *KInfo ,QWidget *parent = 0);
	~MainTab();

signals:
	void requestResList(const QString &Type, QList<const Kite::KResource *> &List);
	Kite::KResource *requestRes(const QString &Name);

public slots:
	void saveAll();
	void openTabs(Kite::KResource *Res);
	void selectResource(Kite::KResource *Res);
	void closeResource(Kite::KResource *Res);

private slots:
	void closeTab();
	void unpinTab();

	void closeDock();
	void pinDock();

protected:
	void focusInEvent(QFocusEvent *Event) override;
	void focusOutEvent(QFocusEvent *Event) override;

private:
	void registerTabs();
	int createTab(QWidget *Widget, Kite::KResource *ResPtr);
	void deleteTab(QWidget *Widget);
	void deleteDock(QDockWidget *Dock);

	KiteInfo *kinfo;
	QVector<QShortcut *> shortcuts;
	QHash<QString, TabWidget *(*)(Kite::KResource *, KiteInfo *, QWidget *)> tabFactory;
	QHash<Kite::KResource *, QPair<TabWidget *, QDockWidget *>> resMap;
};

#endif // MAINTAB_H
