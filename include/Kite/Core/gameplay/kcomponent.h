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

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/gameplay/kgameplaytypes.h"
#include "Kite/Core/gameplay/kmessagehandler.h"
#include "Kite/Core/meta/kmetadef.h"
#include "Kite/Core/serialization/kbaseserial.h"
#include <string>
#include <kcomponent.khgen.h>

KMETA
namespace Kite {
	KMETA_CLASS(COMPONENT)
	class KITE_FUNC_EXPORT KComponent : public KMessageHandler{
	public:
		KComponent(KComponentTypes Type, const std::string &Name, U32 OwnerID, bool NeedUpdate);
		virtual ~KComponent();

		virtual void inite() = 0;
		virtual void remove() = 0;

		KMETA_PROPERTY("name", "component name")
		inline const std::string &getName() const { return _kname; }

		KMETA_PROPERTY("type", "component type")
		inline KComponentTypes getType() const { return _ktype; }

		KMETA_PROPERTY("needUpdate", "need update")
		inline void setNeedUpdate(bool NeedUpdate) { _kneedup = NeedUpdate; }

		KMETA_PROPERTY("ownerID", "owner(entity) id")
		inline U32 getOwnerID() const { return _kownerID; }

		KMETA_PROPERTY("needUpdate")
		inline bool getNeedUpdate() const { return _kneedup; }

		KMETA_FUNCTION()
		inline bool isDependOn(KComponentTypes Type) const { return _kdependency[(U8)Type]; }

		KMETA_KCOMPONENT_BODY();

	protected:
		void setDependency(KComponentTypes Type, bool Value);

	private:
		KMETA_VARIABLE() std::string _kname;
		KMETA_VARIABLE() KComponentTypes _ktype;
		KMETA_VARIABLE() U32 _kownerID;
		KMETA_VARIABLE() bool _kneedup;
		KMETA_VARIABLE() bool _kdependency[(U8)KComponentTypes::KCT_MAX_COMP_SIZE];
	};
}

#endif // KCOMPONENT_H