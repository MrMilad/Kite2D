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
#include "Kite/meta/kmetamanager.h"
#include "kmeta.khgen.h"

namespace Kite {
	KMetaManager::KMetaManager() {
		REGISTER_META();
	}

	const KMetaBase *KMetaManager::getMeta(const std::string &Type) {
		auto found = _kmetamap.find(Type);
		if (found != _kmetamap.end()) {
			return found->second;
		}

		return nullptr;
	}

	void KMetaManager::setMeta(const KMetaBase *Meta) {
		auto found = _kmetamap.find(Meta->getName());
		if (found != _kmetamap.end()) {
			KD_FPRINT("duplicate in meta manager detected. name: %s", Meta->getName().c_str());
			return;
		}

		_kmetamap.insert({ Meta->getName(), Meta });
	}

	void KMetaManager::dump(std::vector<const KMetaBase *> &DumpList) {
		auto iter = _kmetamap.begin();
		DumpList.clear();
		DumpList.reserve(_kmetamap.size());
		for (iter; iter != _kmetamap.end(); ++iter) {
			if (iter->second)
				DumpList.push_back(iter->second);
		}
	}

}