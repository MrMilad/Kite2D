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
#include "Kite/core/kentity.h"
#include "Kite/meta/kmetadef.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "kentitymanager.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KEntityManager: public KMessenger {
		KMETA_KENTITYMANAGER_BODY();
	public:
		KEntityManager();
		~KEntityManager();

		template <typename T>
		bool registerComponent(KComTypes Type) {
			// check base of T
			static_assert(std::is_base_of<KComponent, T>::value, "T must be derived from KComponents");

			if (_kcstorage[(U8)Type] != nullptr) {
				if (_kcstorage[(U8)Type]->type != typeid(T).hash_code()) {
					KD_FPRINT("diffrent component class with same type detected. ctype: %i", (int) Type);
					return false;
				}
				KD_FPRINT("this type has already been registered. ctype: %i", (int)Type);
				return false;
			}

			// register type
			_kcstorage[(U8)Type] = new Internal::CHolder<T, KComponent>;
			_kcstorage[(U8)Type]->type = typeid(T).hash_code();
			return true;
		}

		void unregisterComponent(KComTypes Type);

		KM_FUN()
		bool isRegistered(KComTypes Type);

		/// create entity in the root branch (parrent = 0)
		KM_FUN()
		U32 createEntity(const std::string &Name = "");

		KM_FUN()
		void removeEntity(U32 ID);

		KM_FUN()
		void removeEntityByName(const std::string &Name);

		KM_FUN()
		SIZE inline getEntityCount() { return _kestorage.getSize(); }

		KM_FUN()
		KEntity *getEntity(U32 ID);

		KM_FUN()
		KEntity *getEntityByName(const std::string &Name);

		inline auto beginEntity() { return _kestorage.begin(); }

		inline auto endEntity() { return _kestorage.end(); }

		template<typename T>
		auto beginComponent(KComTypes Type){
			KD_ASSERT(_kcstorage[(U8)Type] != nullptr);
			Internal::CHolder<T, KComponent> *drived = static_cast<Internal::CHolder<T, KComponent> *>(_kcstorage[(U8)Type]);
			return drived->getStorage()->begin();
		}

		template<typename T>
		auto endComponent(KComTypes Type) {
			KD_ASSERT(_kcstorage[(U8)Type] != nullptr);
			Internal::CHolder<T, KComponent> *drived = static_cast<Internal::CHolder<T, KComponent> *>(_kcstorage[(U8)Type]);
			return drived->getStorage()->end();
		}

	private:
		Internal::CFStorage<KEntity> _kestorage;
		std::unordered_map<std::string, U32> _kentmap;
		Internal::BaseCHolder<KComponent> *_kcstorage[(SIZE)KComTypes::KCT_MAX_COMP_SIZE];
	};
}

#endif // KCOMMANAGER_H
