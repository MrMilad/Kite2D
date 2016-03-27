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

	void KMessenger::subscribe(KListener &Handler, const std::string &Type) {
		U32 typeHash = getHash32((void *)Type.c_str(), Type.size(), KHASH_SEED);
		subscribeByHash(Handler, typeHash);
	}

	void KMessenger::subscribeByHash(KListener &Listener, U32 Hash) {
		_khndlMap.insert({ Hash, &Listener });
	}

	void KMessenger::unsubscribe(KListener &Handler, const std::string &Type) {
		// find handler in the range[type] and remove it
		 U32 typeHash = getHash32((void *)Type.c_str(), Type.size(), KHASH_SEED);
		unsubscribeByHash(Handler, typeHash);
	}

	void KMessenger::unsubscribeByHash(KListener &Listener, U32 Hash) {
		auto range = _khndlMap.equal_range(Hash);
		if (range.first != _khndlMap.end()) {
			for (auto it = range.first; it != range.second; ++it) {
				if (it->second == &Listener) {
					_khndlMap.erase(it);
					break; // calling erase() with the iterator will invalidate it, so we can't then continue to use it.
				}
			}
		}
	}

	U32 KMessenger::postMessage(KMessage &Message, KMessageScopeTypes Scope) {
		auto range = _khndlMap.equal_range(Message.getHash());
		U32 recvCount = 0;

		if (range.first != _khndlMap.end()) {
			for (auto it = range.first; it != range.second; ++it) {
				if (it->second->getEnableRecieveMsg()) {
					if (it->second->onMessage(Message, Scope) == KRecieveTypes::KMR_RECEIVED) {
						++recvCount;
					}
				}
			}
		}

		return recvCount;
	}

	KMETA_KMESSENGER_SOURCE();
}