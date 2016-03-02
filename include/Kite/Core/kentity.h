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
#ifndef KENTITY_H
#define KENTITY_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcoretypes.h"
#include "Kite/core/kcorestructs.h"
#include "Kite/core/kcomponent.h"
#include "Kite/core/klistener.h"
#include "Kite/core/kmessenger.h"
#include "Kite/meta/kmetadef.h"
#include <unordered_map>
#include <string>

KMETA
namespace Kite {
	class KEntityManager;
	KMETA_CLASS(ENTITY)
	class KITE_FUNC_EXPORT KEntity: public KMessenger, public KListener{
		friend class KEntityManager;
	public:
		KEntity(const std::string &Name);
		virtual ~KEntity();

		/// redirect recieved message to all sub-components
		KMETA_FUNCTION()
		KRecieveTypes onMessage(KMessage &Message, KMessageScopeTypes Scope);

		/// only script components
		KMETA_FUNCTION()
		bool hasComponent(KComponentTypes Type, const std::string &Name);

		KMETA_PROPERTY("Name", "entity unique name")
		inline const std::string &getName() const { return _kname; }

	private:
		// internal use 
		void setComponent(KComponentTypes Type, const std::string &Name, U32 Index);
		void removeComponent(KComponentTypes Type, const std::string &Name);
		void clear();
		I32 getComponentIndex(KComponentTypes Type, const std::string &Name);
		const std::vector<U32> &getScriptComponentIndex();
		
		KMETA_VARIABLE() std::string _kname;										// entity unique name
		KMETA_VARIABLE() I32 _kfixedComp[(U8)KComponentTypes::KCT_MAX_COMP_SIZE]; 	// fixed components slots (built-in components)
		KMETA_VARIABLE() std::unordered_map<std::string, U32> _kscriptComp;			// dynamic components slots (logic components)
	};
}

#endif // KENTITY_H