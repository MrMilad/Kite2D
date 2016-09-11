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
		KResource(Name, false, false),
		_kisempty(true)
	{
		// prefab has an entity by default
		_kcode.append("function execute(eman, ser)\n");
		_kcode.append("local ent = eman:createEntity(\"\")\n");
		_kcode.append("return ent.handle\nend");

		setInite(true);
	}

	KPrefab::~KPrefab() {}

	bool KPrefab::inite() {
		return true;
	}

	bool KPrefab::_loadStream(KIStream &Stream, const std::string &Address) {
		clear();
		KBinarySerial bserial;
		auto ret = bserial.loadStream(Stream, Address);
		bserial >> _kcode;
		bserial >> _kdata;
		_kisempty = false;
		return ret;
	}

	bool KPrefab::_saveStream(KOStream &Stream, const std::string &Address) {
		KBinarySerial bserial;
		bserial << _kcode;
		bserial << _kdata;
		return bserial.saveStream(Stream, Address, 0);
	}

	void KPrefab::clear() {
		_kdata.clearCatch();
		_kcode.clear();
		_kisempty = true;
	}

	void KPrefab::initeLoad() {
		_kdata.setReadPos(0);
	}

	KMETA_KPREFAB_SOURCE();
}