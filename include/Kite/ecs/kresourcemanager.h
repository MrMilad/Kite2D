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
#include "Kite/ecs/kresource.h"
#include "Kite/ecs/kmessenger.h"
#include "Kite/ecs/kecstypes.h"
#include "Kite/meta/kmetadef.h"
#include <type_traits>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <string>
#include "ktypes.khgen.h"
#include "kresourcemanager.khgen.h"

KMETA
namespace Kite{
	KM_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KResourceManager : public KMessenger{
		friend void registerRTypes(KResourceManager *);
		KMETA_KRESOURCEMANAGER_BODY();
	public:
		KResourceManager();
		~KResourceManager();

		/// don't delete loaded resource by yourself (use unload instead)
		/// catched streams will deleted automatically on owner resource destructures
		KM_FUN()
		KResource *load(const std::string &Name);

		/// unload any resource with any type.
		/// Mode: unload mode
		KM_FUN()
		void unload(const std::string &Name, UnloadMode Mode);

		/// get loaded resource
		/// dont increment refrence counter
		KM_FUN()
		KResource *getLoaded(const std::string &Name);

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

		const std::vector<KResource *> &dump();

		/// unload and clear all resources
		KM_FUN()
		void clear();

	private:
		void initeFactory();
		void registerIStream(IStreamTypes SType, KIStream *(*Func)());
		void registerResource(RTypes RType, KResource *(*Func)(const std::string &));
		bool loadCompositeList(IStreamTypes SType, KResource *Res, const std::string &Address);

		struct info {
			std::string address;
			RTypes rtype;
			IStreamTypes stype;
			KResource *res;

			info(const std::string &Address, RTypes RType, IStreamTypes SType) :
				address(Address), rtype(RType), stype(SType), res(nullptr) {}
		};

		// input stream factory methode
		typedef KIStream *(*isFactory)();
		typedef KResource *(*rFactory)(const std::string &);

		std::unordered_map<std::string, info> _kmap;
		rFactory _krfactory[(SIZE)RTypes::maxSize];
		isFactory _ksfactory[(SIZE)IStreamTypes::maxSize];
	};
}

#endif // KRESOURCEMANAGER_H