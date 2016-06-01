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
#include "Kite/meta/kmetadef.h"
#include <type_traits>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <string>
#include "kresourcemanager.khgen.h"

KMETA
namespace Kite{
	KM_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KResourceManager{
		KMETA_KRESOURCEMANAGER_BODY();
	public:
		KResourceManager();
		~KResourceManager();

		bool registerIStream(const std::string &SType, KIStream *(*Func)());

		bool registerResource(const std::string &RType, KResource *(*Func)(const std::string &), bool CatchStream);

		void setDictionary(const std::unordered_map<std::string, std::string> *Dictionary);

		inline const auto getDictionary() const { return _kdict; }

		KM_FUN()
		KResource *create(const std::string &RType, const std::string &Name);

		/// use catch stream for stream resource eg: KStreamSource
		/// R: resource type
		/// S: stream type
		KM_FUN()
		KResource *load(const std::string &SType, const std::string &RType, const std::string &Address, U32 Flag = 0);

		/// add a loadded resource to resource manager
		/// pass stream if resource hase a catched stream 
		/// replace current loaded resource not allowed
		//bool add(const std::string &ResName, KResource *Resource, KIStream *CatchStream = nullptr);

		/// get loaded resource
		/// dont increment refrence counter
		KM_FUN()
		KResource *get(const std::string &Address);

		/// unload any resource with any type
		KM_FUN()
		void unload(const std::string &Address);

		const std::vector<KResource *> &dump();

		/// clear all resources
		KM_FUN()
		void clear();

	private:
		const std::unordered_map<std::string, std::string> *_kdict;
		std::unordered_map<std::string, std::pair<KResource *, KIStream *>> _kmap;
		std::unordered_map<std::string, std::pair<KResource *(*)(const std::string &), bool>> _krfactory;
		std::unordered_map<std::string, KIStream *(*)()> _ksfactory;
	};
}

#endif // KRESOURCEMANAGER_H