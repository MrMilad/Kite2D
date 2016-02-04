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
#ifndef KMESSENGER_H
#define KMESSENGER_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/kobject.h"
#include "Kite/Core/system/knoncopyable.h"
#include "Kite/Core/gameplay/kgameplaytypes.h"
#include "Kite/Core/gameplay/kgameplaystructs.h"
#include "Kite/Core/gameplay/kmessagehandler.h"
#include "Kite/Core/gameplay/kmessage.h"
#include "kite/Core/memory/kpoolstorage.h"
#include "Kite/Core/meta/kmetadef.h"
#include "Kite/Core/serialization/kbaseserial.h"
#include "Kite/Core/continers/kcontinerstructs.h"
#include <luaintf\LuaIntf.h>
#include <unordered_map>
#include <vector>
#include <memory>
#include <kmessenger.khgen.h>

KMETA
namespace Kite {
	KMETA_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KMessenger : public KObject, KNonCopyable{
	public:
		/// MessagePoolSize: message pool capacity
		/// pass lua state if you want send message from lua
		//KMETA_CONSTRUCTURE()
		KMessenger(U32 MessagePoolSize, KBaseStorage &Allocator, lua_State *Lua = nullptr);

		virtual ~KMessenger();

		void update(F32 DeltaTime);

		// recieve section
		KMETA_FUNCTION()
		bool publicSubscribe(KMessageHandler &Handler);

		KMETA_FUNCTION()
		bool typeSubscribe(KMessageHandler &Handler, const std::string &Type);

		/// always use enable/disable option in KMessageHandler is better than unsubscribe it
		/// unsubscribe a handle, specialy a public handle is a heavy task
		KMETA_FUNCTION()
		void publicUnsubscribe(KMessageHandler &Handler);

		KMETA_FUNCTION()
		void typeUnsubscribe(KMessageHandler &Handler, const std::string &Type);

		/// send message by its type (immediately)
		/// return number of components that recieved message
		KMETA_FUNCTION()
		U32 send(KMessage &Message, KMessageScopeTypes Scope);

		/// send message to all public subscribers (ignore message type - immediately)
		/// return number of components that recieved message 
		KMETA_FUNCTION()
		U32 publish(KMessage &Message, KMessageScopeTypes Scope);

		/// send message by its type 
		/// WaitTime in seconds.
		/// if the WaitTime set to zero, the message will be sent in the next frame.(Regardless of the frame time)
		/// a copy of the message with its data will be placed in queue so
		/// you can delete the message and its data (in c++ or lua) after posting
		KMETA_FUNCTION()
		void sendToQueue(const KMessage &Message, KMessageScopeTypes Scope, F32 WaitTime);

		/// send message to all public subscribers (ignore message type)
		/// WaitTime in seconds
		/// if the WaitTime set to zero, the message will be sent in the next frame.(Regardless of the frame time)
		/// a copy of the message with its data will be placed in queue so
		/// you can delete the message and its data (in c++ or lua) after posting
		KMETA_FUNCTION()
		void publishToQueue(const KMessage &Message, KMessageScopeTypes Scope, F32 WaitTime);

		KMETA_FUNCTION()
		inline U32 getPoolSize() const { return _kmsgPoolSize; }

		KMETA_FUNCTION()
		inline U32 getUsedPoolSize() const { return _kusedPool; }

		/// discard queue
		KMETA_FUNCTION()
		void discardQueue();

		/// remove all subscribers and types and message queue
		KMETA_FUNCTION()
		void reset();

		KMETA_KMESSENGER_BODY();

	private:
		enum SendType {
			PUBLISH,
			SEND
		};
		struct TableHolder {
			LuaIntf::LuaRef dataTable;
			F32 waitTime;
			SendType stype;
			KMessageScopeTypes scope;
			KMessage msg;
		};
		void _configQueue(const KMessage &Message, KMessageScopeTypes Scope, F32 WaitTime,
						  SendType Type, KMessageScopeTypes SType);

		void _setLuaData(const KMessage &Msg);

		KLinkNode<TableHolder> *_freeTopNode();
		std::unordered_map<U32, Internal::MSGHandlerHolder> _kregMap;
		std::unordered_multimap<U32, KMessageHandler *> _khndlMap;
		std::list<KMessageHandler *> _kpubList;
		KBaseStorage *_kbasePool;
		KPoolStorage *_kmsgPool;
		const U32 _kmsgPoolSize;
		U32 _kusedPool;
		KLinkNode<TableHolder> *_kfirstNode;
		lua_State *_klua;
		LuaIntf::LuaRef _kdataTable;
	};
}

#endif // KMESSENGER_H