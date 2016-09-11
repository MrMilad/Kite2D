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
#include "Kite/graphic/katlastexture.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/core/kfostream.h"
#include "Kite/serialization/kbinaryserial.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf/LuaIntf.h>

namespace Kite {
	KAtlasTexture::KAtlasTexture(const std::string &Name) :
		KResource(Name, false, true),
		_ktexture(nullptr)
	{}

	bool KAtlasTexture::_saveStream(KOStream &Stream, const std::string &Address) {
		// texture information
		KBinarySerial bserial;
		bserial << std::string("KATex");

		bserial << _kitems;

		if (!bserial.saveStream(Stream, Address, 0)) {
			KD_PRINT("can't save stream.");
			Stream.close();
			return false;
		}
		Stream.close();

		// save composite list (texture)
		std::vector<KResource *> composite;
		composite.push_back((KResource *)_ktexture);
		setCompositeList(composite);

		return true;
	}

	bool KAtlasTexture::_loadStream(KIStream &Stream, const std::string &Address) {
		// load texture info 
		if (!Stream.isOpen()) {
			Stream.close();
		}

		if (!Stream.open(Address, IOMode::BIN)) {
			KD_FPRINT("can't open stream. address: %s", Address.c_str());
			return false;
		}

		KBinarySerial bserial;
		if (!bserial.loadStream(Stream, Address)) {
			KD_PRINT("can't load stream");
			Stream.close();
			return false;
		}
		std::string format;

		bserial >> format;

		if (format != "KATex") {
			KD_PRINT("incorrect file format");
			Stream.close();
			return false;
		}

		bserial >> _kitems;

		Stream.close();

		// load composite resources (texture)
		auto slist = getCompositeList();
		if (!slist.empty()) {
			_ktexture = (KTexture *)(*slist.begin());
		} else {
			_ktexture = nullptr;
		}

		return true;
	}

	
	bool KAtlasTexture::inite() {
		setInite(true);
		return true;
	}

	
	KMETA_KATLASTEXTURE_SOURCE();
}
