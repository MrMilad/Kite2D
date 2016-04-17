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
#include "Kite/core/kresourcemanager.h"

namespace Kite {
	KResourceManager::~KResourceManager() {
		clear();
	}


	bool KResourceManager::registerResource(const std::string &RType, KResource *(*Func)(const std::string &), bool CatchStream) {
		auto found = _kfactory.find(RType);
		if (found != _kfactory.end()) {
			KD_FPRINT("this type has already been registered. rtype: %s", RType.c_str());
			return false;
		}

		_kfactory.insert({ RType, {Func, CatchStream}});
		return true;
	}

	/*bool KResourceManager::add(const std::string &ResName, KResource *Resource, KIStream *CatchStream) {
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
	}*/

	KResource *KResourceManager::get(const std::string &Name) {
		// checking file name
		if (Name.empty()) {
			KD_PRINT("empty resource name is not valid");
			return nullptr;
		}

		// first check our dictionary
		std::string ResName;
		auto dfound = _kdict.find(ResName);

		// using dictionary key
		if (dfound != _kdict.end()) {
			ResName = dfound->second;
		} else {
			ResName = Name;
		}

		// create key from file name
		std::string tempKey(ResName);
		std::transform(ResName.begin(), ResName.end(), tempKey.begin(), ::tolower);

		// first, check our resource catch
		auto found = _kmap.find(tempKey);
		if (found != _kmap.end()) {
			return found->second.first;
		}

		KD_FPRINT("this reaource not loaded yet. rname: %s", ResName.c_str());
		return nullptr;
	}

	void KResourceManager::unload(const std::string &Name) {
		// checking file name
		if (Name.empty()) {
			KD_PRINT("empty resource name is not valid");
			return;
		}

		// first check our dictionary
		std::string ResName;
		auto dfound = _kdict.find(ResName);

		// using dictionary key
		if (dfound != _kdict.end()) {
			ResName = dfound->second;
		} else {
			ResName = Name;
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

	const std::vector<KResource *> &KResourceManager::dump() {
		static std::vector<KResource *> res;
		res.reserve(_kmap.size());

		for (auto it = _kmap.begin(); it != _kmap.end(); ++it) {
			res.push_back(it->second.first);
		}

		return res;
	}

	void KResourceManager::clear() {
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
}