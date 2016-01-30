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
#ifndef KLINEARALLOCATOR_H
#define KLINEARALLOCATOR_H

#include "Kite/Core/system/ksystemdef.h"
#include "kite/Core/system/knoncopyable.h"
#include "Kite/Core/memory/kbaseallocator.h"

namespace Kite {
	class KITE_FUNC_EXPORT KLinearAllocator : public KBaseAllocator, KNonCopyable {
	public:
		KLinearAllocator(SIZE Size, void *Start);
		~KLinearAllocator();

		// New allocations simply move the pointer to the first free address forward.
		void* allocate(SIZE Size, U8 Alignment) override;

		// Individual deallocations cannot be made in linear allocators,
		// instead use clear() to completely clear the memory used by the allocator.
		void deallocate(void * Pointer) override;

		void clear();

	private:
		void *_kcurPos;
	};

	inline KLinearAllocator* newLinearAllocator(SIZE Size, KBaseAllocator& Allocator) {
		void* p = Allocator.allocate(Size + sizeof(KLinearAllocator), __alignof(KLinearAllocator));
		return new (p) KLinearAllocator(Size, Internal::add(p, sizeof(KLinearAllocator)));
	}

	inline void deleteLinearAllocator(KLinearAllocator& LinearAllocator, KBaseAllocator& Allocator) {
		LinearAllocator.~KLinearAllocator();

		Allocator.deallocate(&LinearAllocator);
	}
}

#endif // KLINEARALLOCATOR_H