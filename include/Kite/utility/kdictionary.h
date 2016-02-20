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
#ifndef KDICTIONARY_H
#define KDICTIONARY_H

#include "Kite/Core/system/ksystemdef.h"
#include <unordered_map>
#include <algorithm>
#include <string>

namespace Kite {
	template <typename T>
	class KDictionary {
	public:
		// duplicate, null objects and empty key is not valid
		bool addObject(const std::string &Key, T *Object) {
			// checking key
			if (Key.empty()) {
				KDEBUG_PRINT("empty key is not valid");
				return false;
			}

			if (!Object) {
				KDEBUG_PRINT("bad or null object pointer");
				return false;
			}

			std::string tempKey(Key);
			std::transform(Key.begin(), Key.end(), tempKey.begin(), ::tolower);

			// first, check our catch
			std::unordered_map<std::string, T *>::iterator found = _kmap.find(tempKey);
			if (found != _kmap.end()) {
				KDEBUG_PRINT("duplicate not allowed");
				return false;
			}

			// storing objects
			_kmap.insert({ tempKey, Object });
			return true;
		}

		void removeObject(const std::string &Key) {
			// checking key
			if (Key.empty()) {
				return;
			}

			std::string tempKey(Key);
			std::transform(Key.begin(), Key.end(), tempKey.begin(), ::tolower);

			// check catch
			std::unordered_map<std::string, T *>::iterator found = _kmap.find(tempKey);
			if (found != _kmap.end()) {
				// remove object
				_kmap.erase(found);
			}
		}

		T *getObject(const std::string &Key){
			// checking key
			if (Key.empty()) {
				KDEBUG_PRINT("empty key is not valid");
				return 0;
			}

			std::string tempKey(Key);
			std::transform(Key.begin(), Key.end(), tempKey.begin(), ::tolower);

			// first, check our catch
			std::unordered_map<std::string, T *>::iterator found = _kmap.find(tempKey);
			if (found == _kmap.end()) {
				KDEBUG_PRINT("object not found");
				return 0;
			}

			return found->second;
		}

		void clear() {
			_kmap.clear();
		}

	private:
		std::unordered_map<std::string, T *> _kmap;
	};
}
#endif // KDICTIONARY_H
