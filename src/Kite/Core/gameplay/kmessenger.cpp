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
#include "Kite/Core/gameplay/kmessenger.h"
#include "Kite/Core/system/ksystemutil.h"
#include "Kite/Core/meta/kmetamanager.h"
#include "Kite/Core/meta/kmetaclass.h"
#include "kite/Core/memory/kpoolallocator.h"
#include <luaintf\LuaIntf.h>
#include <cstdlib>
#include <kmessenger.khgen.h>

namespace Kite {
	KMessenger::KMessenger(U32 MessagePoolSize, U32 DataPoolSize) {

	}

	KMessenger::KMessenger(U32 MessagePoolSize, U32 DataPoolSize, KBaseAllocator &Allocator) 
	{

	}

	KMessenger::~KMessenger() {

	}

	bool KMessenger::publicSubscribe(KMessageHandler &Handler) {
		// check duplication
		auto duplicate = _kregMap.find(Handler.getMsgHandlerID());

		// registered once
		if (duplicate != _kregMap.end()) {
			// registered in public section
			if (duplicate->second.inPublic) {
				KDEBUG_PRINT("this message handler is already registered in public section.")
				return false;
			} else {
				duplicate->second.inPublic = true;
			}

		}else{
			// register in duplicate checking map
			Internal::MSGHandlerHolder hnd(true, false);
			_kregMap.insert({ Handler.getMsgHandlerID(), hnd });
		}

		// register message handler
		_kpubList.push_back(&Handler);

		return true;
	}

	bool KMessenger::typeSubscribe(KMessageHandler &Handler, const std::string &Type) {
		// check registeration (for duplication)
		auto duplicate = _kregMap.find(Handler.getMsgHandlerID());

		// registered once
		if (duplicate != _kregMap.end()) {
			// registered in public section
			if (duplicate->second.inType) {
				KDEBUG_PRINT("this message handler is already registered in type section.")
					return false;
			} else {
				duplicate->second.inType = true;
			}

		} else {
			// register in duplicate checking map
			Internal::MSGHandlerHolder hnd(false, true);
			_kregMap.insert({ Handler.getMsgHandlerID(), hnd });
		}

		// register message handler
		U32 typeHash;
		getHash32((void *)Type.c_str(), Type.size(), KHASH_SEED, (void *)&typeHash);
		_khndlMap.insert({ typeHash, &Handler });

		return true;
	}

	void KMessenger::publicUnsubscribe(KMessageHandler &Handler) {
		auto isExist = _kregMap.find(Handler.getMsgHandlerID());
		
		// already exists
		if (isExist != _kregMap.end()) {
			// registered in public section
			if (isExist->second.inPublic) {

				// find handler in the list and remove it
				_kpubList.remove(&Handler);

				isExist->second.inPublic = false;
			}

			// erase from map if we dont need it anymore
			if (!isExist->second.inPublic && !isExist->second.inType) {
				_kregMap.erase(isExist);
			}
		}
	}

	void KMessenger::typeUnsubscribe(KMessageHandler &Handler, const std::string &Type) {
		auto isExist = _kregMap.find(Handler.getMsgHandlerID());

		// already exists
		if (isExist != _kregMap.end()) {
			// registered in public section
			if (isExist->second.inType) {

				// find handler in the range[type] and remove it
				U32 typeHash;
				getHash32((void *)Type.c_str(), Type.size(), KHASH_SEED, (void *)&typeHash);

				auto range = _khndlMap.equal_range(typeHash);
				if (range.first != _khndlMap.end()) {
					for (auto it = range.first; it != range.second; ++it) {
						if (it->second == &Handler) {
							_khndlMap.erase(it);
							break; // calling erase() with the iterator will invalidate it, so we can't then continue to use it.
						}
					}
				}

				isExist->second.inType = false;
			}

			// erase from map if we dont need it anymore
			if (!isExist->second.inPublic && !isExist->second.inType) {
				_kregMap.erase(isExist);
			}
		}
	}

	U32 KMessenger::send(KMessage &Message, KMessageScopeTypes Scope) {
		auto range = _khndlMap.equal_range(Message.getHash());
		U32 recvCount = 0;

		if (range.first != _khndlMap.end()) {
			for (auto it = range.first; it != range.second; ++it) {
				if (it->second->getEnableRecieveMsg()) {
					if (it->second->onMessage(Message, Scope) == KMR_RECEIVED) {
						++recvCount;
					}
				}
			}
		}

		return recvCount;
	}

	U32 KMessenger::publish(KMessage &Message, KMessageScopeTypes Scope) {
		U32 recvCount = 0;
		for (auto it = _kpubList.begin(); it != _kpubList.end(); ++it) {
			if ((*it)->getEnableRecieveMsg()) {
				if ((*it)->onMessage(Message, Scope) == KMR_RECEIVED) {
					++recvCount;
				}
			}
		}
		return recvCount;
	}

	void KMessenger::sendToQueue(KMessage &Message, KMessageScopeTypes Scope, F32 SendTime) {

	}

	void KMessenger::publishToQueue(KMessage &Message, KMessageScopeTypes Scope, F32 SendTime) {

	}

	void KMessenger::reset() {
		_kregMap.clear();
		_khndlMap.clear();
		_kpubList.clear();
	}

	KMETA_KMESSENGER_SOURCE();
}