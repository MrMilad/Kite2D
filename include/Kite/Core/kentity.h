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
#include "Kite/serialization/kserialization.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "kentity.khgen.h"

KMETA
namespace Kite {
	class KEntityManager;
	KM_CLASS(ENTITY)
	class KITE_FUNC_EXPORT KEntity: public KMessenger, public KListener{
		friend class KEntityManager;
		KMETA_KENTITY_BODY();

	public:
		KEntity(const std::string &Name);
		virtual ~KEntity();

		/// redirect recieved message to all sub-components
		KM_FUN()
		KRecieveTypes onMessage(KMessage &Message, KMessageScopeTypes Scope);

		KM_PRO_GET("pHandle", U32, "parrent handle")
		inline const KHandle &getParrentHandle() const { return _kphandle; }

		KM_PRO_GET("handle", U32, "entity handle")
		inline const KHandle &getHandle() const { return _khandle; }

		KM_PRO_GET("name", std::string, "entity unique name")
		inline const std::string &getName() const { return _kname; }

		KM_PRO_GET("active", bool, "is active")
		inline bool getActive() const { return _kactive; }

		KM_PRO_SET("active")
		inline void setActive(bool Active) { _kactive = Active; }

		KM_FUN()
		KHandle addComponent(KComTypes Type, const std::string &ComponentName = "");

		KM_FUN()
		KComponent *getComponent(KComTypes Type, const KHandle &CHandle);

		KM_FUN()
		KComponent *getComponentByName(KComTypes Type, const std::string &ComponentName);

		KM_FUN()
		void getScriptComponents(std::vector<KComponent *> &Output);

		KM_FUN()
		bool hasComponent(KComTypes Type, const std::string &Name);

		KM_FUN()
		bool hasComponentType(KComTypes Type);

		KM_FUN()
		KHandle getComponentHandle(KComTypes Type, const std::string &Name);

		KM_FUN()
		void removeComponent(KComTypes Type, const std::string &ComponentName = "");

		KM_FUN()
		void clearComponents();

		KM_FUN()
		void addChild(const KHandle &EHandle);

		KM_FUN()
		void removeChild(const KHandle &EHandle);

		KM_FUN()
		void clearChilds();

		KM_FUN()
		bool hasChild() const;

		inline auto beginChild() { return _kchilds.cbegin(); }

		inline auto endChild() { return _kchilds.cend(); }

	private:
		// internal use 
		inline void setCStorage(Internal::BaseCHolder<KComponent> **Storage) { _kcstorage = Storage; }
		inline void setEStorage(Internal::CFStorage<KEntity> *Storage) { _kestorage = Storage; }
		void setComponent(KComTypes Type, const std::string &Name, const KHandle &Index);

		inline void setHandle(const KHandle &Handle) { _khandle = Handle; }
		inline void setPHandle(const KHandle &Handle) { _kphandle = Handle; }
		inline void setPListID(U32 PLID) { _kplistid = PLID; }
		inline U32 getPListID() const { return _kplistid; }
		void remChildIndex(U32 ID);
		inline bool hasParrent() const { return _khparrent; }
		inline void setHParrent(bool Par) { _khparrent = Par; }
		
		KM_VAR() bool _kactive;											// entity actitvity state
		KM_VAR() bool _khparrent;										// entity actitvity state
		KM_VAR() KHandle _khandle;										// entity handle in the entity manager
		KM_VAR() KHandle _kphandle;										// entity parrent handle
		KM_VAR() U32 _kplistid;											// entity self id in the parrent list
		KM_VAR() std::string _kname;									// entity unique name
		KM_VAR() KHandle _kfixedComp[(U8)KComTypes::KCT_MAX_COMP_SIZE]; // fixed components slots (built-in components)
		KM_VAR() std::unordered_map<std::string, KHandle> _kscriptComp;	// dynamic components slots (logic components)
		KM_VAR() std::vector<KHandle> _kchilds;							// children list

		// runtime variables (
		Internal::BaseCHolder<KComponent>  **_kcstorage;
		Internal::CFStorage<KEntity> *_kestorage;

	};
}

#endif // KENTITY_H