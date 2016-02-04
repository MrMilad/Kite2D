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
#ifndef KMETAMANAGER_H
#define KMETAMANAGER_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/meta/kmetaobject.h"
#include "Kite/Core/memory/kbasestorage.h"
#include <memory>
#include <vector>
#include <unordered_map>

namespace Kite {
	class KObject;
	class KITE_FUNC_EXPORT KMetaManager {
	public:
		static const KMetaObject *getMeta(const std::string &Name);
		static bool setMeta(const KMetaObject *Meta);
		
		static bool setFactory(const std::string &Name, KObject *(*Function)(KBaseStorage &));

		static std::shared_ptr<KObject> createClass(const std::string &Name, KBaseStorage &Allocator);

		static void dump(std::vector<const KMetaObject *> &DumpList);

	private:
		static std::unordered_map<std::string, const KMetaObject *> _kmetamap;
		static std::unordered_map<std::string, KObject *(*)(Kite::KBaseStorage &)> _kfactorymap;
	};
}

#endif // KMETAMANAGER_H