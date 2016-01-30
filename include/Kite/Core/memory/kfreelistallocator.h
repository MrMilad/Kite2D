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
#ifndef KFREELISTALLOCATOR_H
#define KFREELISTALLOCATOR_H

#include "Kite/Core/system/ksystemdef.h"
#include "kite/Core/system/knoncopyable.h"
#include "Kite/Core/memory/kbaseallocator.h"

namespace Kite {
	class KITE_FUNC_EXPORT KFreeListAllocator : public KBaseAllocator, KNonCopyable {
	public:
		KFreeListAllocator(SIZE Size, void * Start);
		~KFreeListAllocator();

		void *allocate(SIZE Size, U8 Alignment) override;

		void deallocate(void *Pointer) override;

	private:

		struct AllocationHeader {
			SIZE size;
			U8 adjustment;
		};

		struct FreeBlock {
			SIZE size;
			FreeBlock *next;
		};

		FreeBlock *_kfreeBlocks;
	};

	inline KFreeListAllocator* newFreeListAllocator(SIZE size, KBaseAllocator& allocator) {
		void *p = allocator.allocate(size + sizeof(KFreeListAllocator), __alignof(KFreeListAllocator));
		return new (p) KFreeListAllocator(size, Internal::add(p, sizeof(KFreeListAllocator)));
	}

	inline void deleteFreeListAllocator(KFreeListAllocator& freeListAllocator, KBaseAllocator& allocator) {
		freeListAllocator.~KFreeListAllocator();

		allocator.deallocate(&freeListAllocator);
	}
}

#endif