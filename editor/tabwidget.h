#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QFrame>
#include <Kite/core/kresource.h>
#include "kiteinfo.h"

class QEvent;

class TabWidget: public QFrame{
	Q_OBJECT
public:
    TabWidget(Kite::KResource *Res, Kite::KIStream *Stream, QWidget *Parent = nullptr);
    virtual ~TabWidget();

	virtual void inite() = 0;

    virtual void saveChanges() = 0;

	virtual void reload() = 0;

	virtual void onRemoveRes(const QString &Name, Kite::RTypes Type) {}
	virtual void onAddRes(const QString &Name, Kite::RTypes Type) {}

    inline const auto getResource() const { return res; }
	inline const auto getStream() const { return stream; }

signals:
	void reqResList(Kite::RTypes Type, QStringList &List);
	Kite::KResource *reqResLoad(Kite::KIStream *Stream, const QString &Name);
	void reqReloadName(const QString &Name);
	void reqReloadType(Kite::RTypes Type); // reload all opened resources with the given type

protected :
	Kite::KResource *res;
	Kite::KIStream *stream;

private:
	static std::string temp;
};

#endif // TABWIDGET_H
