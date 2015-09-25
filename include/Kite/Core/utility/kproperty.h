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
#ifndef KPROPERTY_H
#define KPROPERTY_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/utility/krefvariant.h"
#include <unordered_map>
#include <string>

namespace Kite {
	// here we use CRTP pattern because we want each derived class to have it's own static map
	template<typename T>
	class KProperty {
	public:
		bool setProperty(const std::string &Name, KRefVariant Value) {
			auto found = setterMap()->find(Name);
			if (found != setterMap()->end()) {
				(this->*(found->second))(Value);
				return true;
			}

			return false;
		}

		const KRefVariant getProperty(const std::string &Name) const {
			auto found = getterMap()->find(Name);
			if (found != getterMap()->end()) {
				return (this->*(found->second))();
			}

			return KRefVariant;
		}

	protected:
		static std::unordered_map<std::string, void (KProperty::*)(KRefVariant)> *setterMap() {
			static std::unordered_map<std::string, void (KProperty::*)(KRefVariant)> setMap;
			return &setMap;
		}

		static std::unordered_map<std::string, KRefVariant(KProperty::*)() const> *getterMap() {
			static std::unordered_map<std::string, KRefVariant(KProperty::*)() const> getMap;
			return &getMap;
		}
	};
}

#endif // KPROPERTY_H