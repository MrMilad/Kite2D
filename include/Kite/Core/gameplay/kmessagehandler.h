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
#ifndef KMESSAGEHANDLER_H
#define KMESSAGEHANDLER_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/ksystemtypes.h"
#include "Kite/Core/gameplay/kmessage.h"
#include "Kite/Core/gameplay/kgameplaytypes.h"
#include "Kite/Core/meta/kmetadef.h"
#include <kmessagehandler.khgen.h>

namespace Kite {
	KMETA_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KMessageHandler {
		// fake function 
		// we need it for derived serializable class
		friend KBaseSerial &operator<<(KBaseSerial &Out, KObject &Value) {
			return Out;
		}
		friend KBaseSerial &operator>>(KBaseSerial &In, KObject &Value) {
			return In;
		}
	public:
		KMessageHandler(U32 OwnerID) :
			_kid(OwnerID), _kenable(true) {}

		virtual KMSGReceiveTypes onMessage(KMessage &Message, KMessageScopeTypes Scope) = 0;

		KMETA_PROPERTY("ID", "Message handler owner ID (Entity or Component ID)")
		inline U32 getMsgHandlerID() const { return _kid; }

		KMETA_PROPERTY("Enable", "enable/disable recieving message")
		inline bool getEnableRecieveMsg() const { return _kenable; }

		KMETA_PROPERTY("Enable")
		inline void setEnableRecieveMsg(bool Value) { _kenable = Value; }

		KMETA_KMESSAGEHANDLER_BODY();

	private:
		U32 _kid;
		bool _kenable;
	};
}


#endif // KMESSAGEHANDLER_H