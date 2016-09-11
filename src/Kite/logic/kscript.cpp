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
#include <luaintf/LuaIntf.h>

namespace Kite {
	KScript::KScript(const std::string &Name) :
		KResource(Name, false, false) 
	{
		// dont need initialize
		setInite(true);
	}

	bool KScript::inite() {
		return true;
	}

	bool KScript::_loadStream(KIStream &Stream, const std::string &Address) {
		_kcode.clear();

		if (!Stream.isOpen()) {
			Stream.close();
		}

		if (!Stream.open(Address, IOMode::TEXT)) {
			KD_FPRINT("can't open stream. address: %s", Address.c_str());
			return false;
		}

		// create buffer
		SIZE rsize = 0;
		char *buffer = (char*)malloc(sizeof(char) * KTEXT_BUFF_SIZE);

		// reading content
		while (!Stream.eof()) {
			rsize = Stream.read(buffer, sizeof(char) * KTEXT_BUFF_SIZE);
			buffer[rsize] = 0;
			_kcode.append(buffer);
		}

		// cleanup
		free(buffer);
		Stream.close();
		return true;
	}

	bool KScript::_saveStream(KOStream &Stream, const std::string &Address) {
		if (!Stream.isOpen()) {
			Stream.close();
		}

		if (!Stream.open(Address, IOMode::TEXT)) {
			KD_FPRINT("can't open stream. address: %s", Address.c_str());
			return false;
		}

		if (Stream.write(_kcode.data(), _kcode.size()) != _kcode.size()) {
			KD_FPRINT("can't write data to stream. address: %s", Address.c_str());
			Stream.close();
			return false;
		}

		Stream.close();
		return true;
	}

	KMETA_KSCRIPT_SOURCE();
}