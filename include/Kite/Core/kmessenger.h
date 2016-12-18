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

#include "Kite/core/kcoredef.h"
#include "Kite/core/knoncopyable.h"
#include "Kite/core/kcoretypes.h"
#include "Kite/core/kcorestructs.h"
#include "Kite/core/klistener.h"
#include "Kite/core/kmessage.h"
#include "Kite/meta/kmetadef.h"
#include <unordered_map>
#include "kmessenger.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KMessenger{
		KMETA_KMESSENGER_BODY();
	public:
		KM_CON()
		KMessenger();

		virtual ~KMessenger();

		KM_FUN()
		void invoke(KListener *Listener, const std::string &Type);

		KM_FUN()
		void cancelInvoke(KListener *Listener, const std::string &Type);

	protected:
		/// post message by its type (immediately)
		/// return number of objects that recieved message
		KM_FUN()
		U32 postMessage(KMessage *Message, MessageScope Scope);

	private:
		std::unordered_multimap<std::string, KListener *> _khndlMap;
	};
}

#endif // KMESSENGER_H