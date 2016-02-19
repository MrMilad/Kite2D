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
#include "Kite/Core/meta/kmetamanager.h"
#include "Kite/Core/utility/kutilitydef.h"
#include "Kite/Core/system/kobject.h"

namespace Kite {
	std::unordered_map<std::string, const KMetaObject *> KMetaManager::_kmetamap;
	std::unordered_map<std::string, KObject *(*)(KBaseStorage &)> KMetaManager::_kfactorymap;

	const KMetaObject *KMetaManager::getMeta(const std::string &Type) {
		auto found = _kmetamap.find(Type);
		if (found != _kmetamap.end()) {
			return found->second;
		}

		return nullptr;
	}

	bool KMetaManager::setMeta(const KMetaObject *Meta) {
		auto found = _kmetamap.find(Meta->getName());
		if (found != _kmetamap.end()) {
			// registered!
			return false;
		}

		_kmetamap.insert({ Meta->getName(), Meta });
		return true;
	}

	bool KMetaManager::setFactory(const std::string & Name, KObject *(*Function)(KBaseStorage &)) {
		auto found = _kfactorymap.find(Name);
		if (found != _kfactorymap.end()) {
			// registered!
			return false;
		}

		_kfactorymap.insert({ Name, Function });
		return true;
	}

	std::shared_ptr<KObject> KMetaManager::createClass(const std::string & Name, KBaseStorage &Allocator) {
		KObject *instance = nullptr;

		// find name in the registry and call factory method.
		auto found = _kfactorymap.find(Name);
		if (found != _kfactorymap.end()) {
			instance = found->second(Allocator);
		}

		if (instance != nullptr) {
			StorageDeleter<KObject> deleter;
			deleter.storage = &Allocator;
			std::shared_ptr<KObject> ptr(instance, deleter);
			return ptr;
		}
		std::shared_ptr<KObject> ptr;
		return ptr;
	}

	void KMetaManager::dump(std::vector<const KMetaObject *> &DumpList) {
		auto iter = _kmetamap.begin();
		DumpList.clear();
		DumpList.reserve(_kmetamap.size());
		for (iter; iter != _kmetamap.end(); ++iter) {
			if (iter->second)
				DumpList.push_back(iter->second);
		}
	}

}