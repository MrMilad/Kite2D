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
#include <type_traits>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <string>

namespace Kite{
	class KResourceManager{
	public:
		KResourceManager();
		~KResourceManager();

		bool registerResource(const std::string &RType, KResource *(*Func)(const std::string &));

		/// use catch stream for stream resource eg: KStreamSource
		/// S: stream type
		template <typename S>
		KResource *load(const std::string &RType, const std::string &Name, bool CatchStream, U32 Flag = 0){
			// check base of S (resource and stream)
			static_assert(std::is_base_of<KIStream, S>::value, "S must be derived from KIStream");

			// check for factory methode
			auto factory = _kfactory.find(RType);
			if (factory == _kfactory.end()) {
				KD_FPRINT("unregistered resource type. rtype: %s", RType.c_str());
				return nullptr;
			}
			
			// checking file name
			std::string ResName;
			if (ResName.empty()){
				KD_PRINT("empty resource name is not valid");
				return nullptr;
			}

			// first check our dictionary
			if (_kdict != nullptr) {
				auto found = _kdict->find(ResName);

				// using dictionary key
				if (found != _kdict->end()) {
					ResName = found->second;
				} else {
					ResName = Name;
				}
			} else {
				ResName = Name;
			}

			// create key from file name
			std::string tempKey(ResName);
			std::transform(ResName.begin(), ResName.end(), tempKey.begin(), ::tolower);

			// first, check our resource catch
			auto found = _kmap.find(tempKey);
			if (found != _kmap.end()) {
				found->second.first->incRef();
				return static_cast<R *>(found->second.first);
			}

			// create new resoyrce and assocated input stream
			KResource *resource = factory->second(ResName);
			auto stream = new S();
			stream->open(ResName, KIOTypes::KRT_BIN);

			if (!resource->loadStream(*stream, Flag)) {
				KD_FPRINT("can't load resource. rname: %s", ResName.c_str());
				delete resource;
				delete stream;
				return nullptr;
			}

			// stream lifetime
			std::pair<KResource *, KIStream *> pair;
			if (CatchStream){
				pair = std::make_pair(resource, stream);
			}else{
				pair = std::make_pair(resource, nullptr);
				stream->close();
				delete stream;
			}

			// increment refrence count
			resource->incRef();

			// storing resource
			_kmap.insert({ tempKey, pair });
			return static_cast<R *>(resource);
		}

		/// add a loadded resource to resource manager
		/// pass stream if resource hase a catched stream 
		/// replace current loaded resource not allowed
		bool add(const std::string &ResName, KResource *Resource, KIStream *CatchStream = nullptr);

		/// get loaded resource
		/// dont increment refrence counter
		KResource *get(const std::string &ResName);

		/// unload any resource with any type
		void unload(const std::string &ResName);

		const std::vector<KResource *> &dump();

		/// clear all resources
		void clear();

		inline void setDictionary(const std::unordered_map<std::string, std::string> *Dictionary) { _kdict = Dictionary; }
		inline const auto getDictionary() const { return _kdict; }

	private:
		const std::unordered_map<std::string, std::string> *_kdict;
		std::unordered_map<std::string, std::pair<KResource *, KIStream *>> _kmap;
		std::unordered_map<std::string, KResource *(*)(const std::string &)> _kfactory;
	};
}

#endif // KRESOURCEMANAGER_H