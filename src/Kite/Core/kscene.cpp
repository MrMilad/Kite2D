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
#include "Kite/core/kscene.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/serialization/kbinaryserial.h"
#include "Kite/serialization/kserialization.h"
#include "Kite/serialization/types/kstdstring.h"
#include "Kite/serialization/types/kstdvector.h"
#include "Kite/serialization/types/kstdpair.h"
#include "Kite/serialization/types/kstdumap.h"
#include <luaintf\LuaIntf.h>
#include <utility>

namespace Kite {
	KScene::KScene(const std::string &Name) :
		KResource(Name, "Scene"),
		_kloaded(false) {}

	KScene::~KScene() {}

	bool KScene::loadStream(KIStream &Stream, U32 Flag) {
		KBinarySerial bserial;
		if (!bserial.loadStream(Stream)) {
			_kloaded = false;
			KD_PRINT("can't load stream");
			return false;
		}

		bserial >> _kname;
		bserial >> _kres;
		bserial >> _keman;

		_kloaded = true;
		return true;
	}

	bool KScene::saveStream(KOStream &Stream) {
		KBinarySerial bserial;

		bserial << _kname;
		bserial << _kres;
		bserial << _keman;

		if (!bserial.saveStream(Stream)) {
			KD_PRINT("can't save stream");
			return false;
		}

		return true;
	}

	bool KScene::addResource(const std::string &RName, const std::string &RType, U32 Flag) {
		auto found = _kres.find(RName);
		if (found != _kres.end()) {
			KD_FPRINT("this resource is exist. rname: %s", RName.c_str());
			return false;
		}

		_kres.insert({ RName, {RType, Flag}});
		return true;
	}

	void KScene::removeResource(const std::string &RName) {
		_kres.erase(RName);
	}

	KMETA_KSCENE_SOURCE();
}