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
#include "Kite/core/kmessenger.h"
#include "Kite/core/kcoreutil.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include <luaintf\LuaIntf.h>
#include <cstdlib>

namespace Kite {
	KMessenger::KMessenger() {}

	KMessenger::~KMessenger() {}

	void KMessenger::invoke(KListener *Handler, const std::string &Type) {
		_khndlMap.insert({ Type, Handler });
	}

	void KMessenger::cancelInvoke(KListener *Handler, const std::string &Type) {
		// find handler in the range[type] and remove it
		auto range = _khndlMap.equal_range(Type);
		if (range.first != _khndlMap.end()) {
			for (auto it = range.first; it != range.second; ++it) {
				if (it->second == Handler) {
					_khndlMap.erase(it);
					break; // calling erase() with the iterator will invalidate it, so we can't then continue to use it.
				}
			}
		}
	}

	U32 KMessenger::postMessage(KMessage *Message, MessageScope Scope) {
		auto range = _khndlMap.equal_range(Message->getType());
		U32 recvCount = 0;

		if (range.first != _khndlMap.end()) {
			for (auto it = range.first; it != range.second; ++it) {
				if (it->second->getEnableRecieveMsg()) {
					if (it->second->onMessage(Message, Scope) == RecieveTypes::RECEIVED) {
						++recvCount;
					}
				}
			}
		}

		return recvCount;
	}

	KMETA_KMESSENGER_SOURCE();
}