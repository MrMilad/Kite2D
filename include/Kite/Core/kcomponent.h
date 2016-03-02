/*
Kite2D Game Framework.
Copyright (C) 2010-2015  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
USA
*/
#ifndef KCOMPONENT_H
#define KCOMPONENT_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcoretypes.h"
#include "Kite/core/klistener.h"
#include "Kite/core/kmessenger.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/serialization/kbaseserial.h"
#include <string>

KMETA
namespace Kite {
	KMETA_CLASS(COMPONENT)
	class KITE_FUNC_EXPORT KComponent : public KMessenger, public KListener{
	public:
		KComponent(KComponentTypes Type, const std::string &Name, U32 Index);
		virtual ~KComponent();

		/// called when atached to a entity
		virtual void inite(const std::string &EntityName) = 0;

		/// called when deattached from an entity
		virtual void remove(const std::string &EntityName) = 0;

		KMETA_PROPERTY("name", "component name")
		inline const std::string &getName() const { return _kname; }

		KMETA_PROPERTY("type", "component type")
		inline KComponentTypes getType() const { return _ktype; }

		KMETA_PROPERTY("needUpdate")
		inline bool getNeedUpdate() const { return _kneedup; }

		KMETA_PROPERTY("indexID", "storage index")
		inline U32 getIndex() const { return _kindex; }

		/// for simulate polymorphism in script
		KMETA_FUNCTION()
		inline KComponent *getBase() { return this; }

		KMETA_FUNCTION()
		inline bool isDependOn(KComponentTypes Type) const { return _kdependency[(U8)Type]; }

	protected:
		void setDependency(KComponentTypes Type, bool Value);

		inline void setNeedUpdate(bool NeedUpdate) { _kneedup = NeedUpdate; }

	private:
		KMETA_VARIABLE() std::string _kname;
		KMETA_VARIABLE() KComponentTypes _ktype;
		KMETA_VARIABLE() U32 _kindex;
		KMETA_VARIABLE() bool _kneedup;
		KMETA_VARIABLE() bool _kdependency[(U8)KComponentTypes::KCT_MAX_COMP_SIZE];
	};
}

#endif // KCOMPONENT_H