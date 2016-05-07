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
#include "Kite/logic/kscript.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/serialization/kbinaryserial.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf/LuaIntf.h>

namespace Kite {
	KScript::KScript(const std::string &Name) :
		KResource("KScript") 
	{
		setResourceName(Name);
	}

	bool KScript::loadStream(KIStream *Stream, U32 Flag, const std::string &Key) {
		_kcode.clear();

		KBinarySerial ser;
		if (!ser.loadStream(Stream, Key)) {
			return false;
		}
		std::string  format;
		ser >> format;
		if (format != "KScript") {
			KD_PRINT("incorrect file format.");
			return false;
		}

		std::string name;
		ser >> name;
		setResourceName(name);
		ser >> _kcode;
		return true;
	}

	bool KScript::saveStream(KOStream *Stream, U32 Flag, const std::string &Key) {
		KBinarySerial ser;
		ser << std::string("KScript");
		ser << getResourceName();
		ser << _kcode;
		if (!ser.saveStream(Stream,Key)) {
			return false;
		}
		return true;
	}

	KMETA_KSCRIPT_SOURCE();
}