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
#include <vector>

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

		KMETA_PROPERTY("ParrentID", "entity parrent ID")
		inline U32 getParrentID() const { return _kpid; }

		KMETA_PROPERTY("ID", "entity unique ID")
		inline U32 getID() const { return _kid; }

		KMETA_PROPERTY("Name", "entity unique name")
		inline const std::string &getName() const { return _kname; }

		KMETA_PROPERTY("Active", "is active")
		inline bool getActive() const { return _kactive; }

		KMETA_PROPERTY("Active")
		inline void setActive(bool Active) { _kactive = Active; }

		KMETA_FUNCTION()
		U32 addComponent(KComTypes Type, const std::string &ComponentName = "");

		KMETA_FUNCTION()
		KComponent *getComponent(KComTypes Type, const std::string &ComponentName = "");

		KMETA_FUNCTION()
		void getScriptComponents(std::vector<KComponent *> &Output);

		KMETA_FUNCTION()
		bool hasComponent(KComTypes Type, const std::string &Name);

		KMETA_FUNCTION()
		bool hasComponent(KComTypes Type);

		KMETA_FUNCTION()
		I32 getComponentIndex(KComTypes Type, const std::string &Name);

		KMETA_FUNCTION()
		void removeComponent(KComTypes Type, const std::string &ComponentName = "");

		KMETA_FUNCTION()
		void clearComponents();

		KMETA_FUNCTION()
		void addChild(U32 EntityID);

		KMETA_FUNCTION()
		void removeChild(U32 EntityID);

		KMETA_FUNCTION()
		void clearChilds();

		KMETA_FUNCTION()
		bool hasChild() const;

		inline auto beginChild() { return _kchilds.cbegin(); }

		inline auto endChild() { return _kchilds.cend(); }

	private:
		// internal use 
		inline void setCStorage(Internal::BaseCHolder<KComponent> **Storage) { _kcstorage = Storage; }
		inline void setEStorage(Internal::CFStorage<KEntity> *Storage) { _kestorage = Storage; }
		void setComponent(KComTypes Type, const std::string &Name, U32 Index);

		inline void setID(U32 ID) { _kid = ID; }
		inline void setPID(U32 PID) { _kpid = PID; }
		inline void setPListID(U32 PLID) { _kplistid = PLID; }
		inline U32 getPListID() const { return _kplistid; }
		void remChildIndex(U32 ID);
		inline bool hasParrent() const { return _khparrent; }
		inline void setHParrent(bool Par) { _khparrent = Par; }
		
		KMETA_VARIABLE() bool _kactive;											// entity actitvity state
		KMETA_VARIABLE() bool _khparrent;										// entity actitvity state
		KMETA_VARIABLE() U32 _kid;												// entity self id in the entity manager
		KMETA_VARIABLE() U32 _kpid;												// entity parrent id
		KMETA_VARIABLE() U32 _kplistid;											// entity self id in the parrent list
		KMETA_VARIABLE() std::string _kname;									// entity unique name
		KMETA_VARIABLE() I32 _kfixedComp[(U8)KComTypes::KCT_MAX_COMP_SIZE]; 	// fixed components slots (built-in components)
		KMETA_VARIABLE() std::unordered_map<std::string, U32> _kscriptComp;		// dynamic components slots (logic components)
		KMETA_VARIABLE() std::vector<U32> _kchilds;								// dynamic components slots (logic components)

		// runtime variables (
		Internal::BaseCHolder<KComponent>  **_kcstorage;
		Internal::CFStorage<KEntity> *_kestorage;

	};
}

#endif // KENTITY_H