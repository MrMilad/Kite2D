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
#ifndef KPOOLALLOCATOR_H
#define KPOOLALLOCATOR_H

#include "Kite/Core/system/ksystemdef.h"
#include "kite/Core/system/knoncopyable.h"
#include "Kite/Core/memory/kbaseallocator.h"

namespace Kite {
	class KITE_FUNC_EXPORT KPoolAllocator : public KBaseAllocator, KNonCopyable {
	public:
		KPoolAllocator(SIZE ObjectSize, U8 ObjectAlignment, SIZE Size, void *Mem);
		~KPoolAllocator();

		void *allocate(SIZE Size, U8 Alignment) override;

		void deallocate(void *Pointer) override;

		// custom deleter for shared pointers
		template<typename T>
		static void customDeleter(T *Object) {
			Object->~T();
			deallocate((void *)Object);
		}

	private:
		SIZE _kobjectSize;
		U8 _kobjectAlignment;

		void ** _kfreeList;
	};

	inline KPoolAllocator* newPoolAllocator(SIZE objectSize, U8 objectAlignment, SIZE size, KBaseAllocator& allocator) {
		void *p = allocator.allocate(size + sizeof(KPoolAllocator), __alignof(KPoolAllocator));
		return new (p) KPoolAllocator(objectSize, objectAlignment, size, Internal::add(p, sizeof(KPoolAllocator)));
	}

	inline void deletePoolAllocator(KPoolAllocator &poolAllocator, KBaseAllocator &allocator) {
		poolAllocator.~KPoolAllocator();

		allocator.deallocate(&poolAllocator);
	}
}


#endif // KPOOLALLOCATOR_H

