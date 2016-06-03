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
#include "Kite/meta/kmetadef.h"
#include "Kite/serialization/kserialization.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "kentitymanager.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KEntityManager: public KMessenger {
		friend KBaseSerial &operator<<(KBaseSerial &Out, const KEntityManager &Value){
			Value.serial(Out); return Out;
		}

		friend KBaseSerial &operator>>(KBaseSerial &In, KEntityManager &Value) {
			Value.deserial(In); return In;
		}

		KMETA_KENTITYMANAGER_BODY();
	public:
		KEntityManager();
		~KEntityManager();

		KM_FUN()
		bool isModified() const;

		template <typename T>
		bool registerComponent(const std::string &CType) {
			// check base of T
			static_assert(std::is_base_of<KComponent, T>::value, "T must be derived from KComponents");

			// check type
			auto found = _kctypes.find(CType);
			if (found != _kctypes.end()) {
				KD_FPRINT("this type has already been registered. ctype: %s", CType.c_str());
				return false;
			}

			// check index
			U16 index;
			if (_kcompCount >= KCOMP_MAX_SIZE) {
				KD_FPRINT("maximum number of supported components was reached. ctype: %s   msize: %i", CType.c_str(), KCOMP_MAX_SIZE);
				return false;
			}
			index = _kcompCount++;

			if (_kcstorage[index] != nullptr) {
				if (_kcstorage[index]->type != typeid(T).hash_code()) {
					KD_FPRINT("diffrent component class with same type detected. ctype: %s", CType.c_str());
					return false;
				}
				KD_FPRINT("this type has already been registered. ctype: %s", CType.c_str());
				return false;
			}

			// register type
			_kcstorage[index] = new Internal::CHolder<T, KComponent>;
			_kcstorage[index]->type = typeid(T).hash_code();
			_kctypes[CType] = index;
			return true;
		}

		void unregisterComponent(const std::string &CType);

		KM_FUN()
		bool isRegisteredComponent(const std::string &CType);

		/// create entity in the root branch. (parent = 0)
		/// after creating one entity, all previous pointers may be invalid.
		/// so always use handle whene need an older entity.
		KM_FUN()
		KHandle createEntity(const std::string &Name = "");

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

		inline auto beginEntity() { return _kestorage.begin(); }

		inline auto endEntity() { return _kestorage.end(); }

		template<typename T>
		auto beginComponent(const std::string &CType){
			auto found = _kctypes.find(CType);
			KD_ASSERT(found != _kctypes.end());
			KD_ASSERT(_kcstorage[found->second] != nullptr);
			Internal::CHolder<T, KComponent> *drived = static_cast<Internal::CHolder<T, KComponent> *>(_kcstorage[found->second]);
			return drived->getStorage()->begin();
		}

		template<typename T>
		auto endComponent(const std::string &CType) {
			auto found = _kctypes.find(CType);
			KD_ASSERT(found != _kctypes.end());
			KD_ASSERT(_kcstorage[found->second] != nullptr);
			Internal::CHolder<T, KComponent> *drived = static_cast<Internal::CHolder<T, KComponent> *>(_kcstorage[found->second]);
			return drived->getStorage()->end();
		}

	private:
		void serial(KBaseSerial &Out) const;
		void deserial(KBaseSerial &In);
		void recursiveDeleter(KHandle EHandle);
		void initeRoot();
		void initeCStorage();

		KHandle _kroot;
		KCFStorage<KEntity> _kestorage;
		Internal::BaseCHolder<KComponent> *_kcstorage[KCOMP_MAX_SIZE];
		std::unordered_map<std::string, KHandle> _kentmap;
		std::unordered_map<std::string, U16> _kctypes;
		std::vector<KHandle> _ktrash;
		U16 _kcompCount;
	};
}

#endif // KCOMMANAGER_H
