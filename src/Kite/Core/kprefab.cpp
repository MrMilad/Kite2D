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
#include "Kite/core/kprefab.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf\LuaIntf.h>

namespace Kite {
	KPrefab::KPrefab(const std::string &Name) :
		KResource(Name),
		_kisempty(true),
		_kreadPos(0)
	{
		// prefab has an entity by default
		_kcode.append("function execute(eman, ser)\n");
		_kcode.append("local ent = eman:createEntity(\"\")\n");
		_kcode.append("return ent.handle\nend");
	}

	KPrefab::~KPrefab() {}

	bool KPrefab::loadStream(KIStream *Stream, const std::string &Address, U32 Flag) {
		clear();
		auto ret = _kdata.loadStream(Stream, Address);
		_kdata >> _kcode;
		_kreadPos = _kdata.getReadPos();
		_kisempty = false;
		return ret;
	}

	bool KPrefab::saveStream(KOStream *Stream, const std::string &Address, U32 Flag) {
		KBinarySerial bserial;
		if (_kreadPos == 0) {
			bserial << _kcode;
			bserial.append(&_kdata);
			return bserial.saveStream(Stream, Address, 0);
		}
		return _kdata.saveStream(Stream, Address, 0);
	}

	void KPrefab::clear() {
		_kdata.clearCatch();
		_kcode.clear();
		_kreadPos = 0;
		_kisempty = true;
	}

	void KPrefab::initeLoad() {
		_kdata.setReadPos(_kreadPos);
	}

	KMETA_KPREFAB_SOURCE();
}