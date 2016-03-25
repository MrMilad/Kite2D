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
		~KResourceManager() {
			clear();
		}
		/// add a loadded resource to resource manager
		/// pass stream if resource hase a catched stream 
		/// replace current loaded resource not allowed
		bool add(const std::string &ResName, KResource *Resource, KIStream *CatchStream = nullptr) {
			// checking file name
			if (ResName.empty()) {
				KD_PRINT("empty resource name is not valid");
				return false;
			}

			if (!Resource) {
				KD_FPRINT("bad or null resource pointer. rname: %s", ResName.c_str());
				return false;
			}

			// create key from file name
			std::string tempKey(ResName);
			std::transform(ResName.begin(), ResName.end(), tempKey.begin(), ::tolower);

			// first, check our resource catch
			std::unordered_map<std::string, std::pair<KResource *, KIStream *>>::iterator found = _kmap.find(tempKey);
			if (found != _kmap.end()) {
				// replace current loaded resource not allowed
				KD_FPRINT("replace loaded resource not allowed. rname: %s", ResName.c_str());
				return false;
			}

			// stream lifetime
			std::pair<KResource *, KIStream *> pair;
			if (CatchStream) {
				pair = std::make_pair(Resource, CatchStream);
			} else {
				pair = std::make_pair(Resource, (KIStream *)nullptr);
			}

			// storing resource
			_kmap.insert({ tempKey, pair });
			return true;
		}

		/// use catch stream for stream resource eg: KStreamSource
		template <typename R, typename S>
		R *load(const std::string &ResName, bool CatchStream, U32 Flag = 0){
			// check base of R and S (resource and stream)
			static_assert(std::is_base_of<KResource, R>::value, "R must be derived from KResource");
			static_assert(std::is_base_of<KIStream, S>::value, "S must be derived from KIStream");

			// checking file name
			if (ResName.empty()){
				KD_PRINT("empty resource name is not valid");
				return nullptr;
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
			auto resource = new R(ResName);
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

		/// get loaded resource
		template <typename R>
		R *get(const std::string &ResName) {
			// checking file name
			if (ResName.empty()) {
				KD_PRINT("empty resource name is not valid");
				return nullptr;
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

			KD_FPRINT("this reaource not loaded yet. rname: %s", ResName.c_str());
			return nullptr;
		}

		/// unload any resource with any type
		void unload(const std::string &ResName) {
			// checking file name
			if (ResName.empty()) {
				KD_PRINT("empty resource name is not valid");
				return;
			}

			// create key from file name
			std::string tempKey = ResName;
			std::transform(ResName.begin(), ResName.end(), tempKey.begin(), ::tolower);

			// check resource catch
			auto found = _kmap.find(tempKey);
			if (found != _kmap.end()) {
				found->second.first->decRef();

				// check resource count
				if (found->second.first->getReferencesCount() == 0) {
					// free resource
					delete found->second.first;

					// free associated input stream 
					if (found->second.second != nullptr) {
						found->second.second->close();
						delete found->second.second;
					}

					// erase pair from catch
					_kmap.erase(found);
				}
			}
		}

		const std::vector<KResource *> &dump() {
			static std::vector<KResource *> res;
			res.reserve(_kmap.size());

			for (auto it = _kmap.begin(); it != _kmap.end(); ++it) {
				res.push_back(it->second.first);
			}

			return res;
		}

		/// clear all resources
		void clear() {
			for (auto it = _kmap.begin(); it != _kmap.end(); ++it) {
				// free resource
				delete it->second.first;

				// free associated input stream 
				if (it->second.second != nullptr) {
					it->second.second->close();
					delete it->second.second;
				}
			}

			// clear map
			_kmap.clear();
		}

	private:
		std::unordered_map<std::string, std::pair<KResource *, KIStream *>> _kmap;
	};
}

#endif // KRESOURCEMANAGER_H