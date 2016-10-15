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
#include "Kite/math/ktransformsys.h"
#include <cmath>
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/math/ktransformcom.h"
#include <luaintf/LuaIntf.h>

namespace Kite {

	bool KTransformSys::update(F32 Delta, KEntityManager *EManager, KResourceManager *RManager) {
		EDITOR_STATIC const bool isregist = EManager->isRegisteredComponent(CTypes::Transform);
		if (isregist) {
			auto continer = EManager->getComponentStorage<KTransformCom>(CTypes::Transform);
			for (auto it = continer->begin(); it != continer->end(); ++it) {
				auto ehandle = it->getOwnerHandle();
				auto entity = EManager->getEntity(ehandle);
				if (entity->isActive()) {
					it->computeMatrix();
				}
			}
		}
		return true;
	}

	bool KTransformSys::inite(void *Opaque) { 
		setInite(true);
		return true;
	}

	void KTransformSys::destroy() { setInite(false); }

	KMETA_KTRANSFORMSYS_SOURCE();
}
