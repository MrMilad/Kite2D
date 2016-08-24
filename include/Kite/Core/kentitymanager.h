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
#ifndef KENTITYMANAGER_H
#define KENTITYMANAGER_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcoretypes.h"
#include "Kite/core/kcorestructs.h"
#include "Kite/core/kcfstorage.h"
#include "Kite/core/kentity.h"
#include "Kite/core/kistream.h"
#include "Kite/core/kostream.h"
#include "Kite/core/kprefab.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/serialization/kserialization.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "ktypes.khgen.h"
#include "kentitymanager.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KEntityManager : public KMessenger {
		friend KBaseSerial &operator<<(KBaseSerial &Out, const KEntityManager &Value) {
			Value.serial(Out); return Out;
		}

		friend KBaseSerial &operator>>(KBaseSerial &In, KEntityManager &Value) {
			Value.deserial(In); return In;
		}

		KMETA_KENTITYMANAGER_BODY();
	public:
		KEntityManager();
		~KEntityManager();

		template <typename T>
		bool registerComponent(KCTypes Type) {
			// check base of T
			static_assert(std::is_base_of<KComponent, T>::value, "T must be derived from KComponent");
			auto tindex = (SIZE)Type;

			// check type
			if (_kcstorage[tindex] != nullptr) {
				KD_FPRINT("this type has already been registered. ctype: %s", getCTypesName(Type).c_str());
				return false;
			}

			// register type
			_kcstorage[tindex] = new Internal::CHolder<T, KComponent>;
			_kcstorage[tindex]->type = typeid(T).hash_code();
			return true;
		}

		//void unregisterComponent(KCTypes Type);

		KM_FUN()
		bool isRegisteredComponent(KCTypes Type);

		KM_FUN()
		bool createLayer(const std::string &Name);

		/// remove built-in layers is not allowed
		/// all entities of removed layer will be added to 'unlayered' layer
		KM_FUN()
		void removeLayer(const std::string &Name);

		KM_FUN()
		const std::vector<KLayerInfo> &getLayersInfo();

		void addEntityToLayer(const KHandle &Entity, const std::string &Layer);

		/// create entity in the root branch. (parent = 0)
		/// after creating an entity, all previous pointers may be invalid.
		/// so always use handle whene need an older entity.
		/// if an empty string pass as name parameter, 
		/// a new name (ent + N) will generated for entity. (N = ordred number)
		/// entity will added to 'unlayered' layer.
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

		KM_FUN()
			SIZE inline getEntityCount() { return _kestorage.getSize(); }

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

		inline const auto getLayerStorage() const { return &_klayermap; }

		// create new entity will invalidate iterators (use index instead)
		inline const auto getEntityStorage() { return _kestorage.getContiner(); }

		template<typename T>
		auto getComponentStorage(KCTypes Type){
			Internal::CHolder<T, KComponent> *drived = static_cast<Internal::CHolder<T, KComponent> *>(_kcstorage[(SIZE)Type]);
			return drived->getStorage()->getContiner();
		}

#ifdef KITE_EDITOR
		bool copyPrefab(const KHandle &EHandle, KPrefab *Prefab, bool Name);
		KHandle pastePrefab(KPrefab *Prefab);
#endif

	private:
		void initeCStorages();
		KHandle loadPrefabRaw(KPrefab *Prefab, bool isPaste);
		void serial(KBaseSerial &Out) const;
		void deserial(KBaseSerial &In);
		void recursiveDeleter(KHandle EHandle);
		void initeRoot();
		U32 recursiveSaveChilds(KEntity *Entity, KPrefab *Prefab, U32 Level, bool Name, bool CopyPrefab);
		void removeCurrentLayer(KEntity *Entity);
		static bool initeLua();

		KHandle _kroot;
		KCFStorage<KEntity> _kestorage;											// entity storage
		Internal::BaseCHolder<KComponent> *_kcstorage[(SIZE)KCTypes::maxSize];	// component storages (array)
		std::unordered_map<std::string, KHandle> _kentmap; 
		std::unordered_map<std::string, std::pair<KLayerInfo, std::vector<KHandle>>> _klayermap;

		std::vector<KHandle> _ktrash;
		static lua_State *_klstate;
		U32 _knum;
		U32 _kzorder;
	};
}

#endif // KCOMMANAGER_H
