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
#include "Kite/core/kentity.h"
#include "Kite/core/kcoreutil.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/serialization/types/kstdlist.h"
#include <luaintf\LuaIntf.h>

namespace Kite {
	KEntity::KEntity(const std::string &Name):
		_kname(Name)
	{
		for (U8 i = 0; i < (U8)KComponentTypes::KCT_MAX_COMP_SIZE; ++i) {
			_kfixedComp[i] = -1;
		}
	}

	KEntity::~KEntity() {}

	KRecieveTypes KEntity::onMessage(KMessage &Message, KMessageScopeTypes Scope) {
		// redirect message
		postMessage(Message, Scope);

		return KRecieveTypes::KMR_RECEIVED;
	}

	void KEntity::setComponent(KComponentTypes Type, const std::string &Name, U32 Index) {
		if (Type == KComponentTypes::KCT_LOGIC) {
			_kscriptComp[Name] = Index;
		} else {
			_kfixedComp[(U8)Type] = Index;
		}
	}

	void KEntity::removeComponent(KComponentTypes Type, const std::string &Name) {
		if (Type == KComponentTypes::KCT_LOGIC) {
			_kscriptComp.erase(Name);
		} else {
			_kfixedComp[(U8)Type] = -1;
		}
	}

	void KEntity::clear() {
		// remove all fixed components
		for (U8 i = 0; i < (U8)KComponentTypes::KCT_MAX_COMP_SIZE; ++i) {
			_kfixedComp[i] = -1;
		}

		// remove all script component
		_kscriptComp.clear();
	}

	I32 KEntity::getComponentIndex(KComponentTypes Type, const std::string &Name) {
		// script component
		if (Type == KComponentTypes::KCT_LOGIC) {
			auto found = _kscriptComp.find(Name);
			if (found != _kscriptComp.end()) {
				return found->second;
			}

		// non-script component
		} else {
			return _kfixedComp[(U8)Type];
		}

		return -1;
	}

	const std::vector<U32> & KEntity::getScriptComponentIndex() {
		static std::vector<U32> index;
		index.clear();
		index.reserve(_kscriptComp.size());

		for (auto it = _kscriptComp.begin(); it != _kscriptComp.end(); ++it) {
			index.push_back(it->second);
		}

		return index;
	}

	bool KEntity::hasComponent(KComponentTypes Type, const std::string &Name) {
		if (Type == KComponentTypes::KCT_LOGIC) {
			auto found = _kscriptComp.find(Name);
			if (found != _kscriptComp.end()) {
				return true;
			}
		} else {
			if (_kfixedComp[(U8)Type] >= 0) {
				return true;
			}
		}

		return false;
	}
}