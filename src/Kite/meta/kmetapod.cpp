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
#include "Kite/meta/kmetapod.h"
#include "Kite/meta/kmetamanager.h"

namespace Kite {
	KMetaPOD::KMetaPOD(const std::string &Name, U32 Flag, U32 Size, Primitive PODType) :
		KMetaBase(Name, Flag, Size, KMT_POD), _ktype(PODType)
	{}

	namespace Internal {
		void registerMetaPOD(KMetaManager *MMan) {
			const static KMetaPOD instances[] =
			{
				KMetaPOD("I8", 0, (U32)sizeof(I8), Primitive::I8),
				KMetaPOD("I16", 0, (U32)sizeof(I16), Primitive::I16),
				KMetaPOD("I32", 0, (U32)sizeof(I32), Primitive::I32),
				KMetaPOD("I64", 0, (U32)sizeof(I64), Primitive::I64),
				KMetaPOD("U8", 0, (U32)sizeof(U8), Primitive::U8),
				KMetaPOD("U16", 0, (U32)sizeof(U16), Primitive::U16),
				KMetaPOD("U32", 0, (U32)sizeof(U32), Primitive::U32),
				KMetaPOD("U64", 0, (U32)sizeof(U64), Primitive::U64),
				KMetaPOD("F32", 0, (U32)sizeof(F32), Primitive::F32),
				KMetaPOD("F64", 0, (U32)sizeof(F64), Primitive::F64),
				KMetaPOD("bool", 0, (U32)sizeof(bool), Primitive::BOOL),
				KMetaPOD("std::string", 0, 0, Primitive::STR) // std::string
			};

			if (MMan != nullptr) {
				MMan->setMeta(&instances[0]);
				MMan->setMeta(&instances[1]);
				MMan->setMeta(&instances[2]);
				MMan->setMeta(&instances[3]);
				MMan->setMeta(&instances[4]);
				MMan->setMeta(&instances[5]);
				MMan->setMeta(&instances[6]);
				MMan->setMeta(&instances[7]);
				MMan->setMeta(&instances[8]);
				MMan->setMeta(&instances[9]);
				MMan->setMeta(&instances[10]);
				MMan->setMeta(&instances[11]);
			}
		}
	}
}