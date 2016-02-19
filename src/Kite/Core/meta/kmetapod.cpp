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
#include "Kite/Core/meta/kmetapod.h"
#include "Kite/Core/meta/kmetamanager.h"

namespace Kite {
	KMetaPOD::KMetaPOD(const std::string &Name, U32 Flag, U32 Size, KPrimitiveTypes PODType) :
		KMetaObject(Name, Flag, Size, KMT_POD), _ktype(PODType)
	{}

	namespace Internal {
		void registerMetaPOD() {
			static bool registered = false;
			static KMetaPOD instances[] =
			{
				KMetaPOD("I8", 0, (U32)sizeof(I8), KPT_I8),
				KMetaPOD("I16", 0, (U32)sizeof(I16), KPT_I16),
				KMetaPOD("I32", 0, (U32)sizeof(I32), KPT_I32),
				KMetaPOD("I64", 0, (U32)sizeof(I64), KPT_I64),
				KMetaPOD("U8", 0, (U32)sizeof(U8), KPT_U8),
				KMetaPOD("U16", 0, (U32)sizeof(U16), KPT_U16),
				KMetaPOD("U32", 0, (U32)sizeof(U32), KPT_U32),
				KMetaPOD("U64", 0, (U32)sizeof(U64), KPT_U64),
				KMetaPOD("F32", 0, (U32)sizeof(F32), KPT_F32),
				KMetaPOD("F64", 0, (U32)sizeof(F64), KPT_F64),
				KMetaPOD("bool", 0, (U32)sizeof(bool), KPT_BOOL),
				KMetaPOD("string", 0, 0, KPT_STR) // std::string
			};

			if (!registered) {
				registered = true;
				KMetaManager::setMeta(&instances[0]);
				KMetaManager::setMeta(&instances[1]);
				KMetaManager::setMeta(&instances[2]);
				KMetaManager::setMeta(&instances[3]);
				KMetaManager::setMeta(&instances[4]);
				KMetaManager::setMeta(&instances[5]);
				KMetaManager::setMeta(&instances[6]);
				KMetaManager::setMeta(&instances[7]);
				KMetaManager::setMeta(&instances[8]);
				KMetaManager::setMeta(&instances[9]);
				KMetaManager::setMeta(&instances[10]);
				KMetaManager::setMeta(&instances[11]);
			}
		}
	}
}