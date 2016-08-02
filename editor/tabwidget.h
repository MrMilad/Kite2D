#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QFrame>
#include <Kite/core/kresource.h>
#include "kiteinfo.h"

class TabWidget: public QFrame{
	Q_OBJECT
public:
    TabWidget(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent = nullptr);
    virtual ~TabWidget();

	virtual void inite() = 0;

    virtual bool saveChanges() = 0;

    inline const auto getResource() const { return res; }

signals:
	void requestResList(const QString &Type, QList<const Kite::KResource *> &List);
	Kite::KResource *requestRes(const QString &Name);

private:
    Kite::KResource *res;
};

#endif // TABWIDGET_H
