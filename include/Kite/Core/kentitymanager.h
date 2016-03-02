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

KMETA
namespace Kite {
	KMETA_CLASS(CONTINER)
	class KITE_FUNC_EXPORT KEntityManager: public KMessenger {
	public:
		KEntityManager();
		~KEntityManager();

		KMETA_FUNCTION()
		KEntity *createEntity(const std::string &Name);

		void removeEntity(KEntity *Entity);
		void removeEntity(const std::string &Name);

		KEntity *getEntity(const std::string &Name);

		template <typename T>
		void registerType() {
			// check base of T
			static_assert(std::is_base_of<KComponent, T>::value, "T must be derived from KComponents");

			// create a dummy version of T for retrieving its type
			T dummy = T(std::string(), 0);
			KComponentTypes type = dummy.getType();

			if (_kcontiners[(U8)type] != nullptr) {
				if (_kcontiners[(U8)type]->type != typeid(T).hash_code()) {
					KDEBUG_PRINT("diffrent component class with same type detected");
					KDEBUG_ASSERT(false);
				}
				KDEBUG_PRINT("this type has already been registered");
				return;
			}

			// register type
			_kcontiners[(U8)type] = new Internal::CompHolder<T>;
			_kcontiners[(U8)type]->type = typeid(T).hash_code();
		}

		template <typename T>
		void unregisterType() {
			// check base of T
			static_assert(std::is_base_of<KComponent, T>::value, "T must be derived from KComponents");

			// create a dummy version of T for retrieving its type
			T dummy = T(std::string(), 0);
			KComponentTypes type = dummy.getType();

			if (_kcontiners[(U8)type] != nullptr) {
				delete _kcontiners[(U8)type];
				_kcontiners[(U8)type] = nullptr;
			}
		}

		template <typename T>
		T *createComponent(KEntity *Entity, const std::string &ComponentName = "") {
			static_assert(std::is_base_of<KComponent, T>::value, "T must be derived from KComponents");

			// create a dummy version of T for retrieving its type
			T dummy = T(std::string(), 0);
			KComponentTypes type = dummy.getType();

			// registered
			if (_kcontiners[(U8)type] == nullptr) {
				KDEBUG_PRINT("unregistered component types");
				return nullptr;

				// check type
			} else if (_kcontiners[(U8)type]->type != typeid(T).hash_code()) {
				KDEBUG_PRINT("type mismatch");
				return nullptr;

				// this component is already exist
				// we just return it
			} else if (Entity->hasComponent(type, ComponentName)) {
				// cast continer to correct type
				void *ptr = _kcontiners[(U8)type]->get(Entity->getComponentIndex(type, ComponentName));
				return static_cast<T *>(ptr);
			}

			// create component
			void *comPtr = _kcontiners[(U8)type]->create(ComponentName);

			// attach the created component to the entity and call inite functon
			KComponent *bPtr = static_cast<KComponent *>(comPtr);
			Entity->setComponent(type, ComponentName, bPtr->getIndex());
			bPtr->inite(Entity->getName());

			return static_cast<T *>(comPtr);
		}

		template <typename T>
		T *createComponent(const std::string &EntityName, const std::string &ComponentName = "") {
			auto found = _kentmap.find(EntityName);
			if (found != _kentmap.end()) {
				return createComponent<T>(&found->second, ComponentName);
			}
			KDEBUG_PRINT("there is no entity with the given name");
			return nullptr;
		}

		template <typename T>
		void removeComponent(KEntity *Entity, const std::string &ComponentName = "") {
			static_assert(std::is_base_of<KComponent, T>::value, "T must be derived from KComponents");

			// create a dummy version of T for retrieving its type
			T dummy = T(std::string(), 0);
			KComponentTypes type = dummy.getType();

			// registered
			if (_kcontiners[(U8)type] == nullptr) {
				KDEBUG_PRINT("unregistered component types");
				return;
			} else if (_kcontiners[(U8)type]->type != typeid(T).hash_code()) {
				KDEBUG_PRINT("type mismatch");
				return;
			} else if (!Entity->hasComponent(type, ComponentName)) {
				return;
			}

			// deattach component from entity and call remove function
			void *comPtr = _kcontiners[(U8)type]->get(Entity->getComponentIndex(type, ComponentName));
			KComponent *bPtr = static_cast<KComponent *>(comPtr);
			U32 cindex = bPtr->getIndex();
			Entity->removeComponent(type, ComponentName);
			bPtr->remove(Entity->getName());

			// and remove it
			_kcontiners[(U8)type]->remove(cindex);
		}

		template <typename T>
		void removeComponent(const std::string &EntityName, const std::string &ComponentName = "") {
			auto found = _kentmap.find(EntityName);
			if (found != _kentmap.end()) {
				removeComponent<T>(&found->second, ComponentName);
			}
		}

		void removeAllComponent(KEntity *Entity);
		void removeAllComponent(const std::string &EntityName);

		template <typename T>
		T *getComponent(KEntity *Entity, const std::string &ComponentName = "") {
			static_assert(std::is_base_of<KComponent, T>::value, "T must be derived from KComponents");

			// create a dummy version of T for retrieving its type
			T dummy = T(std::string(), 0);
			KComponentTypes type = dummy.getType();

			// registered
			if (_kcontiners[(U8)type] == nullptr) {
				KDEBUG_PRINT("unregistered component types");
				return nullptr;
			} else if (_kcontiners[(U8)type]->type != typeid(T).hash_code()) {
				KDEBUG_PRINT("type mismatch");
				return nullptr;
			} else if (!Entity->hasComponent(type, ComponentName)) {
				KDEBUG_PRINT("there is no component of this type in the given entity");
				return nullptr;
			}

			void *ptr = _kcontiners[(U8)type]->get(Entity->getComponentIndex(type, ComponentName));
			return static_cast<T *>(ptr);
		}

		template <typename T>
		T *getComponent(const std::string &EntityName, const std::string &ComponentName = "") {
			auto found = _kentmap.find(EntityName);
			if (found != _kentmap.end()) {
				return getComponent<T>(&found->second, ComponentName);
			}
			KDEBUG_PRINT("there is no entity with the given name");
			return nullptr;
		}

		template <typename T>
		bool isRegistered() {
			// create a dummy version of T for retrieving its type
			T dummy = T(std::string(), 0);
			KComponentTypes type = dummy.getType();

			if (_kcontiners[(U8)type] == nullptr) {
				return false;
			}

			return true;
		}

		/// usage: c++ side (by systems)
		template <typename T>
		auto beginComponent() {

		}
		inline auto beginEntity() { return _kentmap.begin(); }
		inline auto endEntity() { return _kentmap.end(); }

	private:
		KMETA_VARIABLE() std::unordered_map<std::string, KEntity> _kentmap;
		Internal::BaseCompHolder *_kcontiners[(U8)KComponentTypes::KCT_MAX_COMP_SIZE];
	};
}

#endif // KCOMMANAGER_H
