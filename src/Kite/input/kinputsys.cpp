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
#include "Kite/input/kinputsys.h"
#include "Kite/input/kkeyboard.h"
#include "Kite/input/kmouse.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include <luaintf/LuaIntf.h>

namespace Kite {
	bool KInputSys::update(F32 Delta, KEntityManager *EManager, KResourceManager *RManager) {
		STATIC_OUT_EDITOR const bool isregist = EManager->isRegisteredComponent(CTypes::Input);

		if (isregist) {
			STATIC_OUT_EDITOR auto continer = EManager->getComponentStorage<KInputCom>(CTypes::Input); // avoid hash lookup in every frame
			for (auto it = continer->begin(); it != continer->end(); ++it) {
				auto EHandle = it->getOwnerHandle();
				auto entity = EManager->getEntity(EHandle);
				if (entity->isActive()) {
					if (it->getNeedUpdate()) {
						if (it->getEnableKeyboard() && KKeyboard::isAnyKeyDown()) {
							KMessage msg("KEY_DOWN");
							entity->onMessage(&msg, MessageScope::ALL);
						}
						if (it->getEnableMouse() && KMouse::isAnyKeyDown()) {
							KMessage msg("MOUSE_DOWN");
							entity->onMessage(&msg, MessageScope::ALL);
						}
					}
				}
			}
		}
		return true;
	}

	bool KInputSys::inite(void *Opaque) {
		KKeyboard::initeKeyboard();
		KMouse::initeMouse();
		setInite(true);
		return true;
	}

	void KInputSys::destroy() { setInite(false); }

	KMETA_KINPUTSYS_SOURCE();
}