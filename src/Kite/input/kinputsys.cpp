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
	void KInputSys::update(F32 Delta, KEntityManager &EManager, KResourceManager &RManager) {
		if (EManager.isRegistered(KComTypes::KCT_INPUT)) {
			for (auto it = EManager.beginEntity(); it != EManager.endEntity(); ++it) {
				if (it->hasComponent(KComTypes::KCT_INPUT, "Input") &&
					it->getActive()) {
					KInputCom *ptr = (KInputCom *)it->getComponent(KComTypes::KCT_INPUT, "Input");
					if (ptr->getNeedUpdate()) {
						if (ptr->getEnableKeyboard() && KKeyboard::isAnyKeyDown()) {
							KMessage msg("INPUT_KEY_DOWN");
							it->onMessage(msg, KMessageScopeTypes::KMS_ALL);
						}
						if (ptr->getEnableMouse() && KMouse::isAnyKeyDown()) {
							KMessage msg("INPUT_MOUSE_DOWN");
							it->onMessage(msg, KMessageScopeTypes::KMS_ALL);
						}
					}
				}
			}
		}
	}

	bool KInputSys::inite(void *Opaque) {
		KKeyboard::initeKeyboard();
		KMouse::initeMouse();
		setInite(true);
		return true;
	}

	KMETA_KINPUTSYS_SOURCE();
}