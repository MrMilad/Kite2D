#ifndef MAINTAB_H
#define MAINTAB_H

#include <qtabwidget.h>
#include <qhash.h>
#include <qaction.h>
#include <qpair.h>
#include <qdockwidget.h>
#include <Kite/core/kresource.h>
#include <qstandarditemmodel.h>
#include "shared.h"

class MainTab : public QTabWidget {
	Q_OBJECT

public:
	explicit MainTab(QWidget *parent = 0);
	~MainTab();

	inline void setCompleterModel(QStandardItemModel *Model) { cmodel = Model; }

public slots:
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
	int createTab(QWidget *Widget, const QString &Name);
	void deleteTab(QWidget *Widget);
	void deleteDock(QDockWidget *Dock);

	QVector<QShortcut *> shortcuts;
	QHash<QString, QPair<QWidget *, QDockWidget *>> resMap;
	QStandardItemModel *cmodel;
};

#endif // MAINTAB_H
