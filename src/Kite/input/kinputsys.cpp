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
#include "Kite/engine/kengine.h"

namespace Kite {

	void KInputSys::reset(KNode *Hierarchy, KSysInite *IniteData) {
		_klvm = IniteData->lstate;
		_keventTrig = LuaIntf::LuaRef(_klvm, "events.trigger");
		if (!_keventTrig.isFunction()) {
			throw std::string("events:trigger function is corrupted.");
		}
	}

	bool KInputSys::update(F64 Delta) {
#ifdef KITE_DEV_DEBUG
		if (_keventTrig.isFunction()) {
			try {
				if (KKeyboard::isAnyKeyDown()) {
					_keventTrig(nullptr, "onKeyDown");
				}
				if (KMouse::isAnyKeyDown()) {
					_keventTrig(nullptr, "onMouseDown");
				}

			}
			catch (std::exception& e) {
				KD_FPRINT("input function failed: %s", e.what());
				return false;
			}
		} else {
			KD_PRINT("kite events system is corrupted.");
			return false;
		}
#else
		if (KKeyboard::isAnyKeyDown()) {
			_keventTrig(nullptr, "onKeyDown");
		}
		if (KMouse::isAnyKeyDown()) {
			_keventTrig(nullptr, "onMouseDown");
		}
#endif
		return true;
	}

	void KInputSys::nodeAdded(KNode *Node) {}

	void KInputSys::componentAdded(KComponent *Com) {}

	void KInputSys::componentRemoved(KComponent *Com) {}

	void KInputSys::nodeRemoved(KNode *Node) {}

	KINPUTSYS_SOURCE();
}
