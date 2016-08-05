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
#include "Kite/core/kcoredef.h"

namespace Kite {
	printCallback pcallback = &defaultPrint;
	char *buffer = new char[500];

	void defaultPrint(const std::string &Text, msgType MType) {
		switch (MType) {
		case Kite::msgType::MSG_DEBUG:
			printf("DEBUG: %s\n", Text.c_str());
			break;
		case Kite::msgType::MSG_ASSERT:
			printf("ASSERT: %s\n", Text.c_str());
			break;
		case Kite::msgType::MSG_BREAK:
			printf("BREAK: %s\n", Text.c_str());
			break;
		case Kite::msgType::MSG_LUA:
			printf("LUA: %s\n", Text.c_str());
			break;
		default:
			printf("%s\n", Text.c_str());
			break;
		}
		
	}

	void setEditorPrintCallback(printCallback Callback) {
		pcallback = Callback;
	}

}