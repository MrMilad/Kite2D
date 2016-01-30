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
#include "Kite/Core/system/kobject.h"
#include "Kite/Core/gameplay/kgameplaytypes.h"
#include "Kite/Core/gameplay/kmessagehandler.h"

namespace Kite {
	class KITE_FUNC_EXPORT KComponent : public KObject , public KMessageHandler{
	public:
		KComponent(KComponentTypes Type, bool NeedUpdate);
		virtual ~KComponent();

		virtual void update(F32 Delta) = 0;

		inline KComponentTypes getType() const { return _ktype; }

		inline void setOwnerID(U32 OwnerID) { _koid = OwnerID; }
		inline U32 getOwnerID() const { return _koid; }

		inline void setNeedUpdate(bool NeedUpdate) { _kneedup = NeedUpdate; }
		inline bool getNeedUpdate() const { return _kneedup; }

	private:
		KComponentTypes _ktype;
		U32 _koid;
		bool _kneedup;
	};
}

#endif // KCOMPONENT_H