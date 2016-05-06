#ifndef SHARED_H
#define SHARED_H

#include <qstring.h>
#include <Kite/core/kresource.h>

struct ResourceItem {
	QString address;
	QString type;
	Kite::KResource *resource;

	ResourceItem(const QString &Address, const QString &Type, Kite::KResource *Resource) :
		address(Address), type(Type), resource(Resource) {}
};

enum ActionsState {
	AS_ON_CAT,
	AS_ON_ITEM,
	AS_ON_LOAD,
	AS_ON_INITE
};

#endif // SHARED_H