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
#include "Kite/Core/system/kobject.h"
#include "Kite/Core/system/krefvariant.h"

namespace Kite {
	std::unordered_map<std::string, void (KObject::*)(KRefVariant)> KObject::prpMap;

	KObject::~KObject() {}

	bool KObject::setProperty(const std::string & Class, const std::string & Name, KRefVariant Value) {
		if (Class.empty() || Name.empty()) {
			return false;
		}

		// make real name
		std::string real = "_prp" + Class + "set" + Name;

		auto found = prpMap.find(real);
		if (found != prpMap.end()) {
			(this->*(found->second))(Value);
			return true;
		}

		return false;
	}

	bool KObject::getProperty(const std::string & Class, const std::string & Name, KRefVariant Value) {
		if (Class.empty() || Name.empty()) {
			return false;
		}

		// make real name
		std::string real = "_prp" + Class + "get" + Name;

		auto found = prpMap.find(real);
		if (found != prpMap.end()) {
			(this->*(found->second))(Value);
			return true;
		}
		return false;
	}
}