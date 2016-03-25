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
#include "Kite/meta/kmetaclass.h"

namespace Kite {
	KMetaClass::KMetaClass(const std::string &Name, U32 Flag, U32 Size) :
		KMetaBase(Name, Flag, Size, KMT_CLASS)
	{}

	void KMetaClass::addProperty(const KMetaProperty &Property) {
		_kprop.push_back(Property);
	}
	
	void KMetaClass::addFunction(const KMetaFunction &Function) {
		_kfunc.push_back(Function);
	}

	void KMetaClass::addInfo(const std::pair<std::string, std::string> &Info) {
		_kinfo.push_back(Info);
	}
}