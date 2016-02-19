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
#include "Kite/Core/gameplay/kcomponent.h"
#include "Kite/Core/system/ksystemutil.h"
#include "Kite/Core/meta/kmetamanager.h"
#include "Kite/Core/meta/kmetaclass.h"
#include <luaintf\LuaIntf.h>

namespace Kite {
	KComponent::KComponent(KComponentTypes Type, const std::string &Name,
						   U32 OwnerID, bool NeedUpdate) :
		KMessageHandler(getUniqueNumber() + 1), // 0 reserved
		_ktype(Type), _kname(Name), _kownerID(OwnerID), _kneedup(NeedUpdate) 
	{
		for (U8 i = 0; i < (U8)KComponentTypes::KCT_MAX_COMP_SIZE; i++) {
			_kdependency[i] = false;
		}
	}

	KComponent::~KComponent() {}

	void KComponent::setDependency(KComponentTypes Type, bool Value) {
		_kdependency[(U8)Type] = Value;
	}

	KMETA_KMESSAGE_SOURCE();
}