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
#ifndef KSTACKALLOCATOR_H
#define KSTACKALLOCATOR_H

#include "Kite/Core/system/ksystemdef.h"
#include "kite/Core/system/knoncopyable.h"
#include "Kite/Core/memory/kbaseallocator.h"

namespace Kite {
	class KITE_FUNC_EXPORT KStackAllocator : public KBaseAllocator, KNonCopyable {
	public:
		KStackAllocator(SIZE Size, void *Start);
		~KStackAllocator();

		// New allocations move the pointer up by the requested number of bytes
		// plus the adjustment needed to align the address and store the allocation header.
		void* allocate(SIZE Size, U8 Alignment) override;

		// Memory must be deallocated in inverse order it was allocated!
		// So if you allocate object A and then object B you must free object B memory before you can free object A memory. 
		void deallocate(void *Pointer) override;

	private:
		struct AllocationHeader {
			#if _DEBUG
			void *prev_address;
			#endif
			U8 adjustment;
		};

		#if _DEBUG
		void* _kprevPos;
		#endif

		void*  _kcurPos;
	};

	inline KStackAllocator *newStackAllocator(SIZE Size, KBaseAllocator& Allocator) {
		void *p = Allocator.allocate(Size + sizeof(KStackAllocator), __alignof(KStackAllocator));
		return new (p) KStackAllocator(Size, Internal::add(p, sizeof(KStackAllocator)));
	}

	inline void deleteStackAllocator(KStackAllocator &StackAllocator, KBaseAllocator &Allocator) {
		StackAllocator.~KStackAllocator();

		Allocator.deallocate(&StackAllocator);
	}
}

#endif // KSTACKALLOCATOR_H