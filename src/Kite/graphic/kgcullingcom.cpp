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
#include "Kite/graphic/kgcullingcom.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/core/kentity.h"
#include <luaintf/LuaIntf.h>

namespace Kite {
	KGCullingCom::KGCullingCom(const std::string &Name):
		KComponent(Name),
		_kcallb(nullptr),
		_ksysptr(nullptr),
		_kobjptr(nullptr),
		_kobjIndex(0)
	{
		setRemoveOnDepZero(true);
		addDependency(CTypes::RegisterGCulling);
	}

	void KGCullingCom::attached(KEntity *Owner) {
		// entity manager will not create 'register components' on deserial because they are removed by systems after registration phase
		// so we will add it at runtime
		if (!Owner->hasComponentType(CTypes::RegisterGCulling)) {
			Owner->addComponent(CTypes::RegisterGCulling);
		}
	}

	void KGCullingCom::deattached(KEntity *Owner) {
		// cleanup callback
		if (_kcallb != nullptr) {
			(_kcallb)(this);
		}
	}

	RecieveTypes KGCullingCom::onMessage(KMessage *Message, MessageScope Scope) {
		return RecieveTypes::IGNORED;
	}
	KMETA_KGCULLINGCOM_SOURCE();
}