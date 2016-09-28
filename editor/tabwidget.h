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

	virtual void reload() = 0;

	virtual void onRemoveRes(Kite::RTypes Type) {}
	virtual void onAddRes(const Kite::KResource *Res) {}

    inline const auto getResource() const { return res; }

signals:
	void requestResList(Kite::RTypes Type, QList<const Kite::KResource *> &List);
	Kite::KResource *requestRes(const QString &Name);
	Kite::KResource *requestAddRes(Kite::RTypes Type, const QString &Name);
	void requestReloadTab(Kite::KResource *Res);
	void requestReloadRes(Kite::RTypes Type); // reload all opened resources with the given type

private:
	static std::string temp;
    Kite::KResource *res;
};

#endif // TABWIDGET_H
