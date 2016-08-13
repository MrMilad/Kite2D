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
#include "Kite/core/kcorestructs.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/core/kcoreutil.h"
#include "Kite/serialization/kserialization.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf\LuaIntf.h>

namespace Kite {
	KMETA_KHANDLE_SOURCE();
	//KMETA_KFILEINFO_SOURCE();

	KStringID::KStringID(const std::string &String) :
		str(String) {
		hash = getHash32(str.c_str(), str.size(), KHASH_SEED);
	}

	KStringID::KStringID() :
		str(""), hash(0) {}
	KMETA_KSTRINGID_SOURCE();

	KLayerInfo::KLayerInfo(const std::string &Name, bool IsBuiltin):
		name(Name), isBuiltin(IsBuiltin){}
	KMETA_KLAYERINFO_SOURCE();
}