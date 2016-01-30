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
#ifndef KSTDALLOCATOR_H
#define KSTDALLOCATOR_H

#include "Kite/Core/system/ksystemdef.h"
#include "kite/Core/memory/kbaseallocator.h"
#include <cstdlib>

namespace Kite {
	template <class T>
	class KSTDAllocator {
	public:
		typedef size_t    size_type;
		typedef ptrdiff_t difference_type;
		typedef T*        pointer;
		typedef const T*  const_pointer;
		typedef T&        reference;
		typedef const T&  const_reference;
		typedef T         value_type;

		// usage:
		// beware most vexing parse (two extra parenthesis in contructure)
		// eg: std::vector<int, KSTDAllocator<int>> myVec((KSTDAllocator<int>(BaseAlloc)));
		// or pass nullptr for using default c allocator (malloc/free)
		KSTDAllocator(KBaseAllocator *Allocator = nullptr):
			_kalloc(Allocator)
		{
			size_t st = sizeof(T);
		}

		template <class U>
		KSTDAllocator(const KSTDAllocator<U> &Alloc):
			_kalloc(Alloc.getKAllocator())
		{
			size_t st = sizeof(T);
			size_t su = sizeof(U);
		}

		pointer allocate(size_type n, const void * = 0) {
			T* t = nullptr;
			if (_kalloc != nullptr) {
				t = (T*)_kalloc->allocate(n * sizeof(T), alignof(T));
			} else {
				t = (T*)malloc(n * sizeof(T));
			}
			return t;
		}

		void deallocate(void* p, size_type) {
			if (p) {
				if (_kalloc != nullptr) {
					_kalloc->deallocate(p);
				} else {
					free(p);
				}
			}
		}

		pointer address(reference x) const { return &x; }
		const_pointer address(const_reference x) const { return &x; }
		KSTDAllocator<T>&  operator=(const KSTDAllocator&) { return *this; }
		void construct(pointer p, const T& val) {
			new ((T*)p) T(val);
		}

		void destroy(pointer p) { p->~T(); }

		template <class U>
		struct rebind { typedef KSTDAllocator<U> other; };

		inline KBaseAllocator *getKAllocator() const { return _kalloc; }

	private:
		KBaseAllocator *_kalloc;
	};
}
#endif // KSTDALLOCATOR_H