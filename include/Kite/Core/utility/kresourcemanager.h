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

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/core/utility/kstream.h"
#include "Kite/core/utility/kresource.h"
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

		inline void setInputStream(KStream *Stream) { _kstream = Stream; }

		// add a loadded resource to resource manager
		// pass stream if resource hase a catched stream 
		// replace current loaded resource not allowed
		bool add(const std::string &FileName, KResource *Resource, KIStream *CatchStream = 0) {
			// checking file name
			if (FileName.empty()) {
				KDEBUG_PRINT("empty file name is not valid");
				return false;
			}

			if (!Resource) {
				KDEBUG_PRINT("bad or null resource pointer");
				return false;
			}

			// create key from file name
			std::string tempKey(FileName);
			std::transform(FileName.begin(), FileName.end(), tempKey.begin(), ::tolower);

			// first, check our resource catch
			std::unordered_map<std::string, std::pair<KResource *, KIStream *>>::iterator found = _kmap.find(tempKey);
			if (found != _kmap.end()) {
				// replace current loaded resource not allowed
				KDEBUG_PRINT("replace loaded resource not allowed");
				return false;
			}

			// stream lifetime
			std::pair<KResource *, KIStream *> pair;
			if (CatchStream) {
				pair = std::make_pair(Resource, CatchStream);
			} else {
				pair = std::make_pair(Resource, (KIStream *)0);
			}

			// storing resource
			_kmap.insert({ tempKey, pair });
			_kptrMap.insert({ Resource, FileName });
			return true;
		}

		// use catch stream for stream resource eg: KStreamSource
		template <typename T>
		T *load(const std::string &FileName, bool CatchStream, U32 FileType = 0){
			// checking input stream
			if (!_kstream) {
				KDEBUG_PRINT("there is no input stream");
				return 0;
			}

			// checking file name
			if (FileName.empty()){
				KDEBUG_PRINT("empty file name is not valid");
				return 0;
			}

			// create key from file name
			std::string tempKey(FileName);
			std::transform(FileName.begin(), FileName.end(), tempKey.begin(), ::tolower);

			// first, check our resource catch
			std::unordered_map<std::string, std::pair<KResource *, KIStream *>>::iterator found = _kmap.find(tempKey);
			if (found != _kmap.end()) {
				found->second.first->incRef();
				return (T *)found->second.first;
			}

			// trying to load resource
			KResource *resource = new T;
			KIStream *stream = _kstream->openRead(FileName);
			if (stream == 0){
				KDEBUG_PRINT("can't create stream");
				delete resource;
				return 0;
			}

			if (!resource->loadStream(*stream, FileType)) {
				KDEBUG_PRINT("can't load resource");
				delete resource;
				return 0;
			}

			// stream lifetime
			std::pair<KResource *, KIStream *> pair;
			if (CatchStream){
				pair = std::make_pair(resource, stream);
			}else{
				stream->close();
				delete stream;
				pair = std::make_pair(resource, (KIStream *)0);
			}

			// increment refrence count
			resource->incRef();

			// storing resource
			_kmap.insert({ tempKey, pair });
			_kptrMap.insert({ resource, FileName });
			return (T *)resource;
		}

		// unload any resource with any type
		void unload(const std::string &FileName) {
			// checking file name
			if (FileName.empty()){
				KDEBUG_PRINT("empty file name is not valid");
				return;
			}

			// create key from file name
			std::string tempKey = FileName;
			std::transform(FileName.begin(), FileName.end(), tempKey.begin(), ::tolower);

			// check resource catch
			std::unordered_map<std::string, std::pair<KResource *, KIStream *>>::iterator found = _kmap.find(tempKey);
			if (found != _kmap.end()){
				found->second.first->decRef();

				// check resource count
				if (found->second.first->getReferencesCount() == 0) {
					// free resource
					delete found->second.first;

					// free associated input stream 
					if (found->second.second) {
						found->second.second->close();
						delete found->second.second;
					}

					// erase pair from catch
					_kmap.erase(found);
				}
			}
		}

		// unload any resource with any type
		void unload(KResource *Resource) {
			// checking ptr
			if (!Resource) {
				return;
			}


			// check resource catch
			std::unordered_map<KResource *, std::string>::iterator found = _kptrMap.find(Resource);
			if (found != _kptrMap.end()) {

				// unload
				unload(found->second);

				// erase pair from catch
				_kptrMap.erase(found);
			}
		}

		const std::string &getResourceName(KResource *Resource) {
			// checking ptr
			static std::string rname;
			if (!Resource) {
				return rname;
			}

			// check resource catch
			std::unordered_map<KResource *, std::string>::iterator found = _kptrMap.find(Resource);
			if (found != _kptrMap.end()) {

				rname = found->second;
			}

			return rname;
		}

		void dump(std::string &Names, std::vector<U8> &Types) {
			Names.clear();
			Types.clear();

			std::unordered_map<std::string, std::pair<KResource *, KIStream *>>::iterator it;
			for (it = _kmap.begin(); it != _kmap.end(); ++it) {
				// names
				Names.append(it->first);
				Names.append("\n");

				// types
				Types.push_back(it->second.first->getResourceType());
			}
		}

		// clear all resources
		void clear() {
			std::unordered_map<std::string, std::pair<KResource *, KIStream *>>::iterator it;
			for (it = _kmap.begin(); it != _kmap.end(); ++it){
				// free resource
				delete it->second.first;

				// free associated input stream 
				if (it->second.second) {
					it->second.second->close();
					delete it->second.second;
				}
			}

			// clear map
			_kmap.clear();
		}

	private:
		KStream *_kstream;
		std::unordered_map<std::string, std::pair<KResource *, KIStream *>> _kmap;
		std::unordered_map<KResource *, std::string> _kptrMap;
	};
}

#endif // KRESOURCEMANAGER_H