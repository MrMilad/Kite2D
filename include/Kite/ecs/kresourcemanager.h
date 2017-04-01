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
#ifndef KRESOURCEMANAGER_H
#define KRESOURCEMANAGER_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kistream.h"
#include "Kite/core/kcorestructs.h"
#include "Kite/ecs/kresource.h"
#include "Kite/ecs/kecstypes.h"
#include "Kite/ecs/kecsstructs.h"
#include "Kite/meta/kmetadef.h"
#include <foonathan/memory/memory_pool.hpp>
#include <foonathan/memory/namespace_alias.hpp>
#include <type_traits>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <string>
#include <luaintf/LuaIntf.h>
#include "ktypes.khgen.h"
#include "kresourcemanager.khgen.h"

#define KSHAREDRES_MEM_CHUNK 500

KMETA
namespace Kite{
	template <typename T>
	KM_CLASS(POD)
		class KShared {
		KM_TEM_PARAM(T);
		KM_TEM_DEF("SharedResource", KResource);
		KSHARED_BODY();
		public:
			KM_CON()
			KShared() :
				_kdata(nullptr)
			{}

			explicit KShared(std::nullptr_t Ptr) :
				_kdata(nullptr)
			{}

			explicit KShared(T *Ptr) :
				_kdata(new(_kpool.allocate_node()) KShared<T>::dynamic(Ptr))
			{}

			KShared(const KShared<T>& Other) :
				_kdata(Other._kdata)
			{
				if (_kdata) {
					++_kdata->ref;
				}
			}


			~KShared() {
				clear();
			}

			KShared& operator=(const KShared<T> &other) {
				if (this != &other) {
					clear();

					_kdata = other._kdata;
					if (_kdata) { ++_kdata->ref; }
				}
				return *this;
			}


			inline bool operator==(const KShared<T> &right) const {
				return (constGet() == right.constGet());
			}

			inline bool operator!=(const KShared<T> &right) const {
				return !(constGet() == right.constGet());
			}

			T* operator->() {
				if (!_kdata) return nullptr;
				return _kdata->ptr;
			}

			const T* operator->() const {
				if (!_kdata) return nullptr;
				return _kdata->ptr;
			}

			KM_PRO_GET(KP_NAME = "useCounts", KP_TYPE = SIZE, KP_CM = "counts of shared users")
			inline SIZE getUseCounts() const {
				if (!_kdata) return 0;
				return _kdata->ref; 
			}

			KM_PRO_GET(KP_NAME = "isNull", KP_TYPE = bool, KP_CM = "resource pointer is null")
			inline bool isNull() const {
				if (!_kdata) return true;
				return (_kdata->ptr == nullptr); 
			}

			KM_FUN()
			inline T *get() {
				if (!_kdata) return nullptr;
				return _kdata->ptr;
			}

			inline const T *constGet() const {
				if (!_kdata) return nullptr;
				return _kdata->ptr;
			}

			explicit operator bool() const { return !isNull(); }

			T &operator*() const {
				return *_kdata->ptr; 
			}

			void clear() {
				if (_kdata) {
					if (!--_kdata->ref) {
						delete _kdata->ptr;
						_kdata->ptr = nullptr;
						_kpool.deallocate_node(_kdata);
						_kdata = nullptr;
					}
				}
			}

		private:
			struct dynamic {
				T *ptr;
				SIZE ref;

				dynamic(T *Ptr) :
					ptr(Ptr), ref(1)
				{}
			};

			dynamic *_kdata;
			static memory::memory_pool<> _kpool; // data pool

	};

	template<typename T>
	memory::memory_pool<> KShared<T>::_kpool(sizeof(KShared<T>::dynamic), KSHAREDRES_MEM_CHUNK * sizeof(KShared<T>::dynamic));
	typedef KShared<KResource> KSharedResource;

	KM_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KResourceManager {
		KM_INFO(KI_NAME = "ResourceManager");
		KRESOURCEMANAGER_BODY();
	public:
		~KResourceManager();

		/// catched streams will deleted automatically on owner resource destructures
		/// will keep an instance of shared-pointer of loaded resource in the manager
		KM_FUN()
		KSharedResource load(const std::string &Name);

		/// unload shared resources
		/// only remove stored shared-pointer in the manager
		/// and will not delete it immediately, so this function never affect expire()
		KM_FUN()
		void unload(const std::string &Name);

		/// unload shared resources
		/// immediately remove resource and its pointers
		/// will affect expire() and isExpired() will return true
		/// will not affect counts()
		//void forceUnload(const std::string &Name);

		/// register a key-name resource information for loading phase into the dictinary.
		/// for replacing a registered key with a new key you must unload its associated resource first
		/// RType: resource type
		/// SType: stream type
		/// Name: key-name of the resource. (case-sensitive)
		/// Address: physical address of the resource
		KM_FUN()
		bool registerName(const std::string &Name, const std::string &Address, Resource RType, InStream SType);

		/// save only ditionary so will not save loaded resource
		KM_FUN()
		bool saveDictionary(KOStream &Stream, const std::string &Address);

		/// loading a new Dictionary will clear current state of the manager
		KM_FUN()
		bool loadDictionary(KIStream &Stream, const std::string &Address);

		inline const auto getDictionary() const { return &_kmap; }

		void dump(std::vector<KResourceInfo> &Output) const;

		/// unload and clear all resources
		KM_FUN()
		void clear();

	private:
		struct Info {
			std::string address;
			Resource rtype;
			InStream stype;
			KSharedResource *res;

			Info(const std::string &Address, Resource RType, InStream SType) :
				address(Address), rtype(RType), stype(SType), res(nullptr) {}
		};

		typedef KIStream *(*isFactory)(); // istream factory
		typedef KResource *(*rFactory)(const std::string &, const std::string &); // resource factory

		std::unordered_map<std::string, Info> _kmap;
		const static rFactory _krfactory[(U16)Resource::maxSize];
		const static isFactory _ksfactory[(U16)InStream::maxSize];
	};
}
namespace LuaIntf
{
	LUA_USING_SHARED_PTR_TYPE(Kite::KShared);
}
#endif // KRESOURCEMANAGER_H