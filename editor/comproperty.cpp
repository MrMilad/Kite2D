#include "comproperty.h"
#include "expander.h"
#include <qobject.h>
#include <Kite/meta/kmetaenum.h>

ComponentView::ComponentView(Kite::KComponent *Component, QWidget *Parent) :
	QFrame(Parent) {
	compHandle = Component->getHandle();
	auto meta = (Kite::KMetaClass *)getKMeta()->getMeta(Component->getClassName());
	auto propList = meta->getProperties();

	auto flayout = new QFormLayout(this);
	flayout->setContentsMargins(10, 0, 10, 0);
	flayout->setHorizontalSpacing(5);
	flayout->setVerticalSpacing(3);

	for (auto it = propList->cbegin(); it != propList->cend(); ++it) {
		auto propTypeMeta = getKMeta()->getMeta(it->typeName);
		if (propTypeMeta == nullptr) {
			QString val("Unregistered Property! Name: ");
			auto invalidType = new QLabel(val + it->name.c_str() + " Type: " + it->typeName.c_str());
			invalidType->setStyleSheet("QLabel { color : red; }");
			flayout->addRow(invalidType);
			continue;

		// POD
		} else if ((propTypeMeta->getMetaType() == Kite::KMetaTypes::KMT_POD) ||
				   (propTypeMeta->getFlag() & POD)) {

			createPOD(Component, &(*it), flayout);

		// Enum
		} else if (propTypeMeta->getMetaType() == Kite::KMetaTypes::KMT_ENUM){
			createEnum(Component, &(*it), (const Kite::KMetaEnum *)propTypeMeta, flayout);

		} else {
			QString val("Unsupported Property Type! Name: ");
			auto invalidType = new QLabel(val + it->name.c_str() + " Type: " + it->typeName.c_str());
			invalidType->setStyleSheet("QLabel { color : red; }");
			flayout->addRow(invalidType);
			continue;

			
		}
	}

	this->setLayout(flayout);
}

void ComponentView::reset(Kite::KComponent *Comp) {
	compHandle = Comp->getHandle();
	emit(resetSig(Comp));
}

void ComponentView::propChanged(const QString &CType, const QString &Pname, QVariant &Value) {
	emit(componentEdited(compHandle, CType, Pname, Value));
}

void ComponentView::createPOD(Kite::KComponent *Comp, const Kite::KMetaProperty *Meta, QFormLayout *Layout) {
	// F32
	if (Meta->typeName == "F32") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		auto pgui = new priv::KF32(Comp, Meta->name.c_str(), " ", this, ronly, Meta->min, Meta->max);
		connect(pgui, &priv::KF32::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KF32::reset);
		Layout->addRow(Meta->name.c_str(), pgui);

	//bool
	} else if (Meta->typeName == "bool") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		auto pgui = new priv::KBOOL(Comp, Meta->name.c_str(), this, ronly);
		connect(pgui, &priv::KBOOL::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KBOOL::reset);
		Layout->addRow(Meta->name.c_str(), pgui);

	// U32, I32
	} else if (Meta->typeName == "I32" || Meta->typeName == "U32") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		priv::KI32 *pgui = 0;
		if (Meta->typeName == "I32") {
			pgui = new priv::KI32(Comp, Meta->name.c_str(), " ", this, ronly, Meta->min, Meta->max);
		} else {
			pgui = new priv::KI32(Comp, Meta->name.c_str(), " ", this, ronly, Meta->min, INT32_MAX);
		}
		connect(pgui, &priv::KI32::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KI32::reset);
		Layout->addRow(Meta->name.c_str(), pgui);

	// string
	} else if (Meta->typeName == "std::string") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		priv::KSTR *pgui = 0;
		if (Meta->resType.empty()) {
			pgui = new priv::KSTR(Comp, Meta->name.c_str(), this, ronly);

		// resource field
		} else {
			pgui = new priv::KSTR(Comp, Meta->name.c_str(), this, ronly, Meta->resType.c_str(), true);
			connect(pgui, &priv::KSTR::updateResList, this, &ComponentView::updateResList);
		}
		connect(pgui, &priv::KSTR::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KSTR::reset);

		Layout->addRow(Meta->name.c_str(), pgui);

	// KVector2F32
	} else if (Meta->typeName == "KVector2F32") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		auto pgui = new priv::KV2F32(Comp, Meta->name.c_str(), this, ronly, Meta->min, Meta->max);
		connect(pgui, &priv::KV2F32::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KV2F32::reset);

		Layout->addRow(Meta->name.c_str(), pgui);

	// KColor
	} else if (Meta->typeName == "KColor") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		auto pgui = new priv::KCOLOR(Comp, Meta->name.c_str(), this, ronly);
		connect(pgui, &priv::KCOLOR::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KCOLOR::reset);
		Layout->addRow(Meta->name.c_str(), pgui);

	// unimplemented type
	} else {
		QString val("Unimplemented Property Type! Name: ");
		auto invalidType = new QLabel(val + Meta->name.c_str() + " Type: " + Meta->typeName.c_str());
		invalidType->setStyleSheet("QLabel { color : red; }");
		Layout->addRow(invalidType);
	}

}

void ComponentView::createEnum(Kite::KComponent *Comp, const Kite::KMetaProperty *PMeta,
							   const Kite::KMetaEnum *EMeta, QFormLayout *Layout) {

	// is read-only
	bool ronly = false;
	if (PMeta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;

	// catch items
	auto members = EMeta->getMembers();
	QStringList items;
	for (auto it = members->cbegin(); it != members->cend(); ++it) {
		items.push_back(it->name.c_str());
	}

	auto pgui = new priv::KENUM(Comp, PMeta->name.c_str(), this, items, ronly);
	connect(pgui, &priv::KENUM::propertyEdited, this, &ComponentView::propChanged);
	connect(this, &ComponentView::resetSig, pgui, &priv::KENUM::reset);
	Layout->addRow(PMeta->name.c_str(), pgui);
}



