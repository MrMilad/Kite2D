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
#include "kite/Core/memory/kpoolallocator.h"
#include "Kite/Core/meta/kmetadef.h"
#include "Kite/Core/serialization/kbaseserial.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <kmessenger.khgen.h>

KMETA
namespace Kite {
	KMETA_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KMessenger : public KObject, KNonCopyable{
	public:
		// MessagePoolSize: message pool capacity
		// DataPoolSize: data pool capacity
		// the messenger will use default c++ allocator
		KMETA_CONSTRUCTURE()
		KMessenger(U32 MessagePoolSize, U32 DataPoolSize);

		// MessagePoolSize: message pool capacity
		// DataPoolSize: data pool capacity
		KMessenger(U32 MessagePoolSize, U32 DataPoolSize, KBaseAllocator &Allocator);

		virtual ~KMessenger();

		// recieve section
		KMETA_FUNCTION()
		bool publicSubscribe(KMessageHandler &Handler);

		KMETA_FUNCTION()
		bool typeSubscribe(KMessageHandler &Handler, const std::string &Type);

		// always use enable/disable option in KMessageHandler is better than unsubscribe it
		// unsubscribe a handle, specialy a public handle is a heavy task
		KMETA_FUNCTION()
		void publicUnsubscribe(KMessageHandler &Handler);

		KMETA_FUNCTION()
		void typeUnsubscribe(KMessageHandler &Handler, const std::string &Type);

		// send message by its type (immediately)
		// return number of components that recieved message
		KMETA_FUNCTION()
		U32 send(KMessage &Message, KMessageScopeTypes Scope);

		// send message to all public subscribers (ignore message types - immediately)
		// return number of components that recieved message 
		KMETA_FUNCTION()
		U32 publish(KMessage &Message, KMessageScopeTypes Scope);

		// send message by its type 
		// SendTime in seconds. message will send after SendTime seconds
		KMETA_FUNCTION()
		void sendToQueue(KMessage &Message, KMessageScopeTypes Scope, F32 SendTime);

		// send message to all public subscribers (ignore message types)
		// SendTime in seconds. message will send after SendTime seconds
		KMETA_FUNCTION()
		void publishToQueue(KMessage &Message, KMessageScopeTypes Scope, F32 SendTime);

		// remove all subscribers and types
		KMETA_FUNCTION()
		void reset();

		KMETA_KMESSENGER_BODY();

	private:
		std::unordered_map<U32, Internal::MSGHandlerHolder> _kregMap;
		std::unordered_multimap<U32, KMessageHandler *> _khndlMap;
		std::list<KMessageHandler *> _kpubList;
		KBaseAllocator *_kbaseAlloc;
		U32 _kmsgPoolSize;
		U32 _kdataPoolSize;
	};
}

#endif // KMESSENGER_H