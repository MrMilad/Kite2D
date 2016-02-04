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
#include "kite/Core/memory/kpoolstorage.h"
#include <luaintf\LuaIntf.h>
#include <cstdlib>
#include <kmessenger.khgen.h>

namespace Kite {
	KMessenger::KMessenger(U32 MessagePoolSize, KBaseStorage &Allocator, lua_State *Lua):
		_kbasePool(&Allocator), _kmsgPool(nullptr),
		_kmsgPoolSize(MessagePoolSize), _kusedPool(0),
		_kfirstNode(nullptr), _klua(Lua)
	{
		KDEBUG_ASSERT(MessagePoolSize > 0);

		SIZE pSize = MessagePoolSize * sizeof(KLinkNode<TableHolder>);
		_kmsgPool = newPoolAllocator(sizeof(KLinkNode<TableHolder>), alignof(KLinkNode<TableHolder>), pSize, Allocator);
		KDEBUG_ASSERT(_kmsgPool != nullptr);

		// register messenger table to lua
		// this tbale is used for transfering data between scripts
		if (Lua != nullptr) {
			_kdataTable = LuaIntf::LuaRef::createTableWithMeta(Lua, "Kite.MessageData");
		}
	}

	KMessenger::~KMessenger() {
		reset();
		deletePoolAllocator(*_kmsgPool, *_kbasePool);
	}

	void KMessenger::update(F32 DeltaTime) {
		if (_kfirstNode == nullptr) {
			return;
		}

		// iterate over queue and update message wait times
		auto iter = _kfirstNode;
		while (iter != nullptr) {
			iter->data.waitTime -= DeltaTime;

			// dispatch message
			if (iter->data.waitTime <= 0) {
				if (iter->data.stype == SEND) {
					_setLuaData(iter->data.msg);
					send(iter->data.msg, iter->data.scope);
				} else {
					_setLuaData(iter->data.msg);
					publish(iter->data.msg, iter->data.scope);
				}

				// remove processed message from the queue
				// its always first message in the queue because queue is sorted
				iter = _freeTopNode();
				continue;
			}
			iter = iter->NextNode();
		}
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

	void KMessenger::sendToQueue(const KMessage &Message, KMessageScopeTypes Scope, F32 WaitTime) {
		_configQueue(Message, Scope, WaitTime, SEND, Scope);
	}

	void KMessenger::publishToQueue( const KMessage &Message, KMessageScopeTypes Scope, F32 WaitTime) {
		_configQueue(Message, Scope, WaitTime, PUBLISH, Scope);
	}

	void KMessenger::_configQueue(const KMessage &Message, KMessageScopeTypes Scope, F32 WaitTime,
								  SendType Type, KMessageScopeTypes SType) {
		KDEBUG_ASSERT(_kusedPool < _kmsgPoolSize);
		if (WaitTime < 0) WaitTime = 0.0f;

		// copy meesage 
		KLinkNode<TableHolder> *msgNode = allocateNew<KLinkNode<TableHolder>>(*_kmsgPool);
		msgNode->data.msg = Message;
		msgNode->data.waitTime = WaitTime;
		msgNode->data.stype = Type;
		msgNode->data.scope = SType;
		++_kusedPool;

		// first we check c++ data side
		if (Message.getData() != nullptr) {
			// copy its data
			void *data = _kbasePool->allocate(Message.getDataSize());
			memcpy(data, Message.getData(), Message.getDataSize());

			// assocate new message with its new data
			msgNode->data.msg.setData(data, Message.getDataSize());

		// check lua table data
		} else if (!Message.getLuaTable().empty() && _klua != nullptr) {
			LuaIntf::LuaRef table(_klua, Message.getLuaTable().c_str());
			if (table.isTable()) {
				// copy table into our table holder by ref
				msgNode->data.dataTable = table;
				// and release user table
				table.~LuaRef();
			}
		}

		// put the message in the appropriate place in queue (sort by its time)
		// queue is empty
		if (_kfirstNode == nullptr) {
			_kfirstNode = msgNode;
			return;

		// check first object in the queue
		} else if (_kfirstNode->data.waitTime > msgNode->data.waitTime){
			_kfirstNode->InsertAfter(msgNode);
			msgNode->InsertAfter(_kfirstNode);
			_kfirstNode = msgNode;
			return;

		// check entire objects in the queue
		} else {
			KLinkNode<TableHolder> *iter = _kfirstNode;
			while (iter != nullptr) {
				 if (iter->NextNode() == nullptr) {
					iter->InsertAfter(msgNode);
					break;
				}else if (iter->NextNode()->data.waitTime > msgNode->data.waitTime) {
					iter->InsertAfter(msgNode);
					break;
				} 

				iter = iter->NextNode();
			}
		}
	}

	void KMessenger::_setLuaData(const KMessage &Msg) {
		if (_klua != nullptr && _kdataTable.isValid() && !Msg.getLuaTable().empty()) {
			LuaIntf::LuaRef oldTable(_klua, Msg.getLuaTable().c_str());
			if (oldTable.isValid() && oldTable.isTable()) {
				// set message data to Kite.MessageData table 
				_kdataTable = oldTable;

				// release old table
				oldTable.~LuaRef();
			}
		}
	}

	KLinkNode<KMessenger::TableHolder> *KMessenger::_freeTopNode() {
		if (_kfirstNode != nullptr) {
			auto node = _kfirstNode;
			_kfirstNode = _kfirstNode->NextNode();

			// freemsg data
			void *data = node->data.msg.getData();
			if (data != nullptr) {
				_kbasePool->deallocate(data);
			}

			// free node itself
			_kmsgPool->deallocate((void *)&node);
			--_kusedPool;
			return _kfirstNode;
		}

		return nullptr;
	}

	void KMessenger::discardQueue() {
		if (_kfirstNode == nullptr) {
			return;
		}

		auto iter = _kfirstNode;
		while (iter != nullptr) {
			void *msgData = iter->data.msg.getData();
			if (msgData != nullptr) {
				_kbasePool->deallocate(msgData);
			}
			deallocateDelete<KLinkNode<TableHolder>>(*_kmsgPool, *iter);
			iter = iter->NextNode();
		}

		_kfirstNode = nullptr;
		_kusedPool = 0;
	}

	void KMessenger::reset() {
		_kregMap.clear();
		_khndlMap.clear();
		_kpubList.clear();
		discardQueue();
	}

	KMETA_KMESSENGER_SOURCE();
}