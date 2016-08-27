#ifndef KITEINFO_H
#define KITEINFO_H

#include <qstandarditemmodel.h>
#include <qstringlist.h>
#include <qhash.h>
#include <qvector.h>
#include <Kite/meta/kmetaclass.h>
#include <ktypes.khgen.h>

class KiteInfo {
public:
	KiteInfo();
	~KiteInfo();

	inline auto getModel() const { return model; }
	inline const auto getResourceTypes() const { return resources; }
	inline const auto getComponentTypes() const { return components; }
	inline const auto getResourceComponentsTypes() const { return &resComponents; }

private:
	QStandardItemModel *model;
	QStringList *resources;
	QVector<QPair<Kite::CTypes, bool>> *components; // <type, visible>
	QHash<size_t, QVector<Kite::KMetaProperty>> resComponents; // <component name, resource properti>
};

#endif // KITEINFO_H
