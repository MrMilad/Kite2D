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
#include <cstring>
#include <unordered_map>
#include <algorithm>
#include <utility>

namespace Kite{
	template < class T >
	class KITE_FUNC_EXPORT KResourceManager{
	public:
		~KResourceManager() {
			clear();
		}

		static inline void setInputStream(KInputStream *InputStream) { _kstream = InputStream; }

		static inline void setDefaultResource(T *DefaultResource) {
			_kdefault = DefaultResource;
		}

		// use catch stream for stream resource eg: KStreamSource
		static T *load(const std::string &FileName, bool CatchStream){
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
			std::string tempKey = FileName;
			std::transform(FileName.begin(), FileName.end(), tempKey.begin(), ::tolower);

			// first, check our resource catch
			std::unordered_map<std::string, std::pair<T *, KInputStream *>>::iterator found = _kmap.find(tempKey);
			if (found != _kmap.end())
				return (T *)found->second.first;

			// trying to load resource
			T *resource = new T;
			KInputStream *stream = _kstream->openRead(FileName);
			if (stream == 0){
				KDEBUG_PRINT("can't create stream");
				delete resource;

				// return default resource
				if (_kdefault)
					return _kdefault;

				return 0;
			}

			if (!resource->loadStream(*stream)){
				KDEBUG_PRINT("can't load resource");
				delete resource;

				// return default resource
				if (_kdefault)
					return _kdefault;

				return 0;
			}

			// stream lifetime
			std::pair<T *, KInputStream *> pair;
			if (CatchStream){
				pair = std::make_pair(resource, stream);
			}else{
				delete stream;
				pair = std::make_pair(resource, (KInputStream *)0);
			}

			// increment refrence count
			resource->incRef();

			// storing resource
			_kmap.insert({ tempKey, pair });
			return resource;
		}

		static void unload(const std::string &FileName) {
			// checking file name
			if (FileName.empty()){
				KDEBUG_PRINT("empty file name is not valid");
				return;
			}

			// create key from file name
			std::string tempKey = FileName;
			std::transform(FileName.begin(), FileName.end(), tempKey.begin(), ::tolower);

			// check resource catch
			std::unordered_map<std::string, std::pair<T *, KInputStream *>>::iterator found = _kmap.find(tempKey);
			if (found != _kmap.end()){
				found->second.first->decRef();

				// check resource count
				if (found->second.first->getReferencesCount() == 0) {
					// free resource
					delete found->second.first;

					// free associated input stream 
					if (found->second.second)
						delete found->second.second;

					// erase pair from catch
					_kmap.erase(found);
				}
			}
		}

		static void clear() {
			std::unordered_map<std::string, std::pair<T *, KInputStream *>>::iterator it;
			for (it = _kmap.begin(); it != _kmap.end(); ++it){
				// free resource
				delete it->second.first;

				// free associated input stream 
				if (it->second.second)
					delete it->second.second;
			}

			// clear map
			_kmap.clear();
		}

	private:
		static KStream *_kstream;
		static std::unordered_map<std::string, std::pair<T *, KInputStream *>> _kmap;
		static T *_kdefault;
	};
}

#endif // KRESOURCEMANAGER_H