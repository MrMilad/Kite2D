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
#include "Kite/ecs/kmessenger.h"
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
#include "ktypes.khgen.h"
#include "kresourcemanager.khgen.h"

#define KSHAREDRES_MEM_CHUNK 500

KMETA
namespace Kite{
	class KSharedResource;

	KM_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KResourceManager : public KMessenger{
		KMETA_KRESOURCEMANAGER_BODY();
	public:
		KResourceManager();
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
		KM_FUN()
		void forceUnload(const std::string &Name);

		/// register a key-name resource information for loading phase into the dictinary.
		/// for replacing a registered key with a new key you must unload its associated resource first
		/// RType: resource type
		/// SType: stream type
		/// Name: key-name of the resource. (case-sensitive)
		/// Address: physical address of the resource
		KM_FUN()
		bool registerName(const std::string &Name, const std::string &Address, RTypes RType, IStreamTypes SType);

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

		void registerIStream(IStreamTypes SType, KIStream *(*Func)());
		void registerResource(RTypes RType, KResource *(*Func)(const std::string &, const std::string &));

	private:
		void initeFactory();

		struct Info {
			std::string address;
			RTypes rtype;
			IStreamTypes stype;
			KSharedResource *res;

			Info(const std::string &Address, RTypes RType, IStreamTypes SType) :
				address(Address), rtype(RType), stype(SType), res(nullptr) {}
		};

		typedef KIStream *(*isFactory)(); // istream factory
		typedef KResource *(*rFactory)(const std::string &, const std::string &); // resource factory

		std::unordered_map<std::string, Info> _kmap;
		rFactory _krfactory[(SIZE)RTypes::maxSize];
		isFactory _ksfactory[(SIZE)IStreamTypes::maxSize];
	};

	KM_CLASS(POD)
	class KSharedResource {
		friend class KResourceManager;
		KMETA_KSHAREDRESOURCE_BODY();
	public:
		KM_CON()
		KSharedResource();

		explicit KSharedResource(KResource *Resource);

		KSharedResource(const KSharedResource& Other);

		~KSharedResource();

		KSharedResource& operator=(const KSharedResource& other);

		KResource* operator->();

		const KResource* operator->() const;

		KM_PRO_GET(KP_NAME = "isExpired", KP_TYPE = bool, KP_CM = "is expired (force unloaded) by resource manager")
		inline bool isExpired() const { return _kdata->expired; }

		KM_PRO_GET(KP_NAME = "counts", KP_TYPE = SIZE, KP_CM = "counts of shared users")
		inline SIZE getCounts() const { return _kdata->ref; }

		KM_PRO_GET(KP_NAME = "isNull", KP_TYPE = bool, KP_CM = "resource pointer is null")
		inline bool isNull() const { return (_kdata->ptr == nullptr); }

	private:
		struct dynamic {
			bool expired;
			KResource *ptr;
			SIZE ref;

			dynamic(KResource *Resource) :
				expired(false), ptr(Resource), ref(1)
			{}
		};

		dynamic *_kdata;
		ED_STATIC static memory::memory_pool<> _kpool; // data pool

		/// lua side 
		KM_PRO_GET(KP_NAME = "resource", KP_TYPE = KResource, KP_CM = "raw resource pointer")
		inline KResource *getResource() { return operator->(); }

		void clear();

		/// internal use by resource manager
		/// will delete resource immediately
		void expire();
	};
}

#endif // KRESOURCEMANAGER_H