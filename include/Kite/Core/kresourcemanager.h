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
#include "Kite/core/kresource.h"
#include "Kite/core/kistream.h"
#include "Kite/core/kmessenger.h"
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
		KMETA_KRESOURCEMANAGER_BODY();
	public:
		KResourceManager();
		~KResourceManager();

		KM_FUN()
		KResource *create(RTypes Type, const std::string &Name);

		/// create and register resource on rhe fly
		KM_FUN()
		KResource *createAndRegist(RTypes Type, const std::string &Name);

		/// R: resource type
		/// S: stream type
		/// this function can use same as get function but will increment ref counter of source with each call
		KM_FUN()
		KResource *load(IStreamTypes SType, RTypes RType, const std::string &Address);

		/// add a loadded resource to resource manager
		/// pass stream if resource hase a catched stream 
		/// replace current loaded resource not allowed
		//bool add(const std::string &ResName, KResource *Resource, KIStream *CatchStream = nullptr);

		/// get loaded resource
		/// dont increment refrence counter
		KM_FUN()
		KResource *get(const std::string &Address);

		/// unload any resource with any type.
		/// Immediately: ref counter will ignored and resource will deleted immediately if passed true
		/// so careful about pointer dangling 
		KM_FUN()
		void unload(const std::string &Address, bool Immediately = false);

		bool saveDictionary(KOStream &Stream, const std::string &Address);

		bool loadDictionary(KIStream &Stream, const std::string &Address);

		inline const auto getDictionary() const { return &_kdict; }

		const std::vector<KResource *> &dump();

		/// clear all resources
		KM_FUN()
		void clear();

		void registerIStream(IStreamTypes SType, KIStream *(*Func)());

		void registerResource(RTypes RType, KResource *(*Func)(const std::string &));

	private:
		void initeRFactory();
		bool loadCompositeList(KResource *Res, KIStream &Stream, const std::string &Address);

		// input stream factory methode
		typedef KIStream *(*isFactory)();

		std::unordered_map<std::string, std::string> _kdict;
		std::unordered_map<std::string, std::pair<KResource *, KIStream *>> _kmap;
		std::pair<KResource *(*)(const std::string &), bool> _krfactory[(SIZE)RTypes::maxSize];
		isFactory _ksfactory[(SIZE)IStreamTypes::maxSize];
	};
}

#endif // KRESOURCEMANAGER_H