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
	flayout->setContentsMargins(3, 0, 3, 0);
	flayout->setHorizontalSpacing(5);
	flayout->setVerticalSpacing(2);

	for (auto it = propList->cbegin(); it != propList->cend(); ++it) {
		// only visible property
		if (!it->show) {
			continue;
		}

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

void ComponentView::propChanged(const QString &Pname, QVariant &Value) {
	emit(componentEdited(compHandle, Pname, Value));
}

void ComponentView::createPOD(Kite::KComponent *Comp, const Kite::KMetaProperty *Meta, QFormLayout *Layout) {
	// F32
	if (Meta->typeName == "F32") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		auto pgui = new priv::KF32(Comp, Meta->name.c_str(), "", this, ronly, Meta->min, Meta->max);
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

	// I32
	} else if (Meta->typeName == "I32") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		priv::KI32 *pgui = 0;
		pgui = new priv::KI32(Comp, Meta->name.c_str(), "", this, ronly, Meta->min, Meta->max);
		connect(pgui, &priv::KI32::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KI32::reset);
		Layout->addRow(Meta->name.c_str(), pgui);

	// U32
	} else if (Meta->typeName == "U32") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		priv::KU32 *pgui = 0;
		pgui = new priv::KU32(Comp, Meta->name.c_str(), "", this, ronly, Meta->min, UINT32_MAX);
		connect(pgui, &priv::KU32::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KU32::reset);
		Layout->addRow(Meta->name.c_str(), pgui);

	// I8
	} else if (Meta->typeName == "I8") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		priv::KI8 *pgui = 0;
		pgui = new priv::KI8(Comp, Meta->name.c_str(), "", this, ronly, Meta->min, INT8_MAX);
		connect(pgui, &priv::KI8::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KI8::reset);
		Layout->addRow(Meta->name.c_str(), pgui);

	// I16
	} else if (Meta->typeName == "I16") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		priv::KI16 *pgui = 0;
		pgui = new priv::KI16(Comp, Meta->name.c_str(), "", this, ronly, Meta->min, INT16_MAX);
		connect(pgui, &priv::KI16::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KI16::reset);
		Layout->addRow(Meta->name.c_str(), pgui);


	// U16
	} else if (Meta->typeName == "U16") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		priv::KU16 *pgui = 0;
		pgui = new priv::KU16(Comp, Meta->name.c_str(), "", this, ronly, Meta->min, INT16_MAX);
		connect(pgui, &priv::KU16::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KU16::reset);
		Layout->addRow(Meta->name.c_str(), pgui);

	// string
	} else if (Meta->typeName == "std::string") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		priv::KSTR *pgui = 0;
		pgui = new priv::KSTR(Comp, Meta->name.c_str(), this, ronly);
		connect(pgui, &priv::KSTR::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KSTR::reset);

		Layout->addRow(Meta->name.c_str(), pgui);

	// string id
	} else if (Meta->typeName == "KStringID") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		priv::KSTRID *pgui = 0;
		pgui = new priv::KSTRID(Comp, Meta->name.c_str(), this, Meta->resType, ronly);
		connect(pgui, &priv::KSTRID::updateResList, this, &ComponentView::updateResList);
		connect(pgui, &priv::KSTRID::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KSTRID::reset);

		Layout->addRow(Meta->name.c_str(), pgui);

	// KAtlasItem
	} else if (Meta->typeName == "KAtlasItem") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		priv::KATLASITEM *pgui = 0;
		pgui = new priv::KATLASITEM(Comp, Meta->name.c_str(), this, ronly);
		connect(pgui, &priv::KATLASITEM::requestPropValue, this, &ComponentView::requestPropValue);
		connect(pgui, &priv::KATLASITEM::requestRes, this, &ComponentView::requestRes);
		connect(pgui, &priv::KATLASITEM::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KATLASITEM::reset);

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

	// KVector2U32
	} else if (Meta->typeName == "KVector2U32") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		auto pgui = new priv::KV2U32(Comp, Meta->name.c_str(), this, ronly, Meta->min, Meta->max);
		connect(pgui, &priv::KV2U32::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KV2U32::reset);

		Layout->addRow(Meta->name.c_str(), pgui);

	// KVector2I32
	} else if (Meta->typeName == "KVector2I32") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		auto pgui = new priv::KV2I32(Comp, Meta->name.c_str(), this, ronly, Meta->min, Meta->max);
		connect(pgui, &priv::KV2I32::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KV2I32::reset);

		Layout->addRow(Meta->name.c_str(), pgui);

	// KRectF32
	} else if (Meta->typeName == "KRectF32") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		auto pgui = new priv::KRF32(Comp, Meta->name.c_str(), this, ronly);
		connect(pgui, &priv::KRF32::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KRF32::reset);

		Layout->addRow(Meta->name.c_str(), pgui);

	// KBitset
	} else if (Meta->typeName == "KBitset") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		auto pgui = new priv::KBITSET(Comp, Meta->name.c_str(), this, ronly);
		connect(pgui, &priv::KBITSET::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KBITSET::reset);

		Layout->addRow(Meta->name.c_str(), pgui);

	// KGlyphMarker
	} else if (Meta->typeName == "KGlyphMarker") {
		// create an appropriate widgte and bind property to it
		bool ronly = false;
		if (Meta->type == Kite::KMetaPropertyTypes::KMP_GETTER) ronly = true;
		auto pgui = new priv::KGMARKER(Comp, Meta->name.c_str(), this, ronly);
		connect(pgui, &priv::KGMARKER::propertyEdited, this, &ComponentView::propChanged);
		connect(this, &ComponentView::resetSig, pgui, &priv::KGMARKER::reset);

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



