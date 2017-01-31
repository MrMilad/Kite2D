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
#ifndef KSCENE_H
#define KSCENE_H

#include "Kite/core/kcoredef.h"
#include "Kite/ecs/kecsstructs.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/serialization/kserialization.h"
#include "Kite/ecs/kecstypes.h"
#include "Kite/ecs/kcfstorage.h"
#include "Kite/ecs/kentity.h"
#include "Kite/ecs/kprefab.h"
#include "Kite/ecs/kresource.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "ktypes.khgen.h"
#include "kscene.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(RESOURCE)
	class KITE_FUNC_EXPORT KScene : public KResource {
		friend void registerCTypes(KScene *);
		KM_INFO(KI_NAME = "Scene");
		KMETA_KSCENE_BODY()
	public:
		KM_CON(std::string)
		KScene(const std::string &Name);

		~KScene();

		bool inite() override;

		/// create entity in the root branch. (parent = 0)
		/// note: after creating an entity, all previous pointers may be invalid.
		///		so always use handle when need access to an older entity.
		/// note: if an empty string pass as name parameter, 
		///		entity will created but not registered in the dictionary.
		/// note: nameless entities will created faster than entities with name
		/// note: you can register a name for a nameless entity with renameEntity() function
		KM_FUN()
		KEntity *createEntity(const std::string &Name = "");

		KM_FUN()
		bool renameEntity(const KHandle &EHandle, const std::string &NewName);

		/// entity will not be deleted immediately
		/// but marked as deactive and stored in trash list
		/// it will removed after calling postWork function.
		/// we use this methode to prevent dangling pointer during updates.
		KM_FUN()
			void removeEntity(KHandle Handle);

		KM_FUN()
			void removeEntityByName(const std::string &Name);

		/// total number of entities
		KM_PRO_GET(KP_NAME = "entityCount", KP_TYPE = SIZE, KP_CM = "total number of entities")
		inline SIZE getEntityCount() { return _kestorage.getSize(); }

		KM_FUN()
		KEntity *getEntity(const KHandle &Handle);

		KM_FUN()
		KEntity *getEntityByName(const std::string &Name);

		KM_PRO_GET(KP_NAME = "root", KP_TYPE = KHandle, KP_CM = "root entity")
		inline const KHandle &getRoot() const { return _kroot; }

		void postWork();

		/// SaveName used for editor cut/copy purpose
		KM_FUN()
		bool createPrefab(const KHandle &EHandle, KPrefab *Prefab, bool SaveNames = false);

		/// ispaste used for editor cut/copy purpose
		KM_FUN()
		KHandle loadPrefab(KPrefab *Prefab);

		// create new entity will invalidate iterators (use index instead)
		inline const auto getEntityStorage() { return _kestorage.getContiner(); }

		template<typename T>
		std::vector<T> *getComponentStorage(CTypes Type){
			Internal::CHolder<T, KComponent, Type> *drived = static_cast<Internal::CHolder<T, KComponent, Type> *>(_kcstorage[(U16)Type]);
			return drived->getStorage()->getContiner();
		}

		// usage: register new entity to systems and clear list after registration
		template<typename T>
		void clearComponentStorage(CTypes Type) {
			if (Type == CTypes::Logic) {
				KD_PRINT("logic storage is not removable");
				return;
			}
			Internal::CHolder<T, KComponent, Type> *drived = static_cast<Internal::CHolder<T, KComponent, Type> *>(_kcstorage[(U16)Type]);
			auto continer = drived->getStorage()->getContiner();
			while (continer->size() > 0) {
				auto EHandle = continer->back().getOwnerHandle();
				auto entity = getEntity(EHandle);
				entity->forceRemoveCom(Type);
			}
		}

#ifdef KITE_EDITOR
		bool copyPrefab(const KHandle &EHandle, KPrefab *Prefab, bool Name);
		KHandle pastePrefab(KPrefab *Prefab);
#endif

	private:
		template <typename T>
		bool registerComponent(CTypes Type) {
			// check base of T
			static_assert(std::is_base_of<KComponent, T>::value, "T must be derived from KComponent");
			auto tindex = (U16)Type;

			// check type
			if (_kcstorage[tindex] != nullptr) {
				KD_FPRINT("this type has already been registered. ctype: %s", getCTypesName(Type).c_str());
				return false;
			}

			// register type
			_kcstorage[tindex] = new Internal::CHolder<T, KComponent, Type>;
			_kcstorage[tindex]->type = typeid(T).hash_code();
			return true;
		}

		bool _saveStream(KOStream &Stream, const std::string &Address) override;

		bool _loadStream(KIStream &Stream, const std::string &Address) override;

		void initeCStorages();
		KHandle loadPrefabRaw(KPrefab *Prefab, bool isPaste);
		void recursiveDeleter(KHandle EHandle);
		void initeRoot();
		U32 recursiveSaveChilds(KEntity *Entity, KPrefab *Prefab, U32 Level, bool Name, bool CopyPrefab);
		static bool initeLua();

		KHandle _kroot;
		KCFStorage<KEntity, CTypes::maxSize> _kestorage;	// entity storage
		Internal::BaseCHolder<KComponent> *_kcstorage[(U16)CTypes::maxSize];	// component storages (array)
		std::unordered_map<std::string, KHandle> _kentmap; 

		std::vector<KHandle> _ktrash;
		static lua_State *_klstate;
		U32 _knum;
		U32 _kzorder;
	};
}

#endif // KSCENE_H
