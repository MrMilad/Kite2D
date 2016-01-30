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
#ifndef KOBJECT_H
#define KOBJECT_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/krefvariant.h"
#include "Kite/Core/meta/kmetadef.h"
#include "Kite/Core/serialization/kbaseserial.h"
#include "Kite/Core/serialization/kserializationtypes.h"
#include "Kite/Core/memory/kbaseallocator.h"
#include <unordered_map>
#include <string>

namespace Kite {
	class KITE_FUNC_EXPORT KObject {
		friend KBaseSerial &operator<<(KBaseSerial &Out, KObject &Value) {
			Value.serial(Out, KST_SERIALIZE); return Out;
		}
		friend KBaseSerial &operator>>(KBaseSerial &In, KObject &Value) {
			Value.serial(In, KST_DESERIALIZE); return In;
		}

	public:
		virtual ~KObject();

		bool setProperty(const std::string &Class, const std::string &Name, KRefVariant Value);

		bool getProperty(const std::string &Class, const std::string &Name, KRefVariant Value);

		virtual const std::string &getClassName() const = 0;

	protected:
		virtual void serial(KBaseSerial &Serializer, KSerialStateTypes State) = 0;

		static std::unordered_map<std::string, void (KObject::*)(KRefVariant)> prpMap;

	private:
		std::string _kname;
	};
}

#endif // KOBJECT_H