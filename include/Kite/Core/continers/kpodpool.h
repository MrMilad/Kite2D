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
#ifndef KPODPOOL_H
#define KPODPOOL_H

#include "Kite/Core/system/ksystemdef.h"
#include "kite/Core/memory/kbaseallocator.h"
#include "kite/Core/memory/kpoolallocator.h"
#include "kite/Core/memory/kstdallocator.h"
#include "Kite/Core/system/kobject.h"
#include "Kite/Core/meta/kmetadef.h"
#include "Kite/Core/serialization/kbaseserial.h"
#include <string>
#include <list>
#include <kpodpool.khgen.h>

KMETA
namespace Kite {
	namespace Internal {
		enum KPODUnType {
			KPD_INT,
			KPD_FLT,
			KPD_BOL
		};
		union KPODUnion {
			I32 i32;
			F32 f32;
			bool bl;
		};

		struct KPoolDictElement {
			U32 hash;
			KPODUnion data;
			KPODUnType dataType;

			bool operator==(const KPoolDictElement& rhs) {
				if (hash == rhs.hash)
					return true;
				else
					return false;
			}
		};
	}

	KMETA_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KPODPool : public KObject {
	public:
		// max number of objects in the pool
		// default c++ allocator (new\delete)
		KMETA_CONSTRUCTURE()
		KPODPool(SIZE MaxPoolSize);

		// Max number of objects in the pool
		// allocate MaxPoolSize + size of pool list + size of pool allocator from Allocator
		KPODPool(SIZE MaxPoolSize, KBaseAllocator &Allocator);

		~KPODPool();

		KMETA_FUNCTION()
		void setInt(const std::string &Key, I32 Data);

		KMETA_FUNCTION()
		void setFloat(const std::string &Key, F32 Data);

		KMETA_FUNCTION()
		void setBool(const std::string &Key, bool Data);

		KMETA_FUNCTION()
		I32 getInt(const std::string &Key) const;

		KMETA_FUNCTION()
		F32 getFloat(const std::string &Key) const;

		KMETA_FUNCTION()
		bool getBool(const std::string &Key) const;

		KMETA_FUNCTION()
		inline SIZE getMaxPoolSize() const { return _kmaxSize; }

		KMETA_FUNCTION()
		inline SIZE getUsedPoolSize() const { return _kusedSize; }

		KMETA_FUNCTION()
		void clear();

		KMETA_KPODPOOL_BODY()
	private:
		KBaseAllocator *_kbaseAlloc;
		KPoolAllocator *_kalloc;
		SIZE _kmaxSize;
		SIZE _kusedSize;
		void *_kdata;
		std::list<Internal::KPoolDictElement, KSTDAllocator<Internal::KPoolDictElement>> *_klist;
	};
}

#endif // KPODPOOL_H
