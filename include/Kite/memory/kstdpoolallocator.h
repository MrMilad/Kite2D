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
#ifndef KSTDPOOLALLOCATOR_H
#define KSTDPOOLALLOCATOR_H

#include "Kite/Core/system/ksystemdef.h"
#include "kite/Core/memory/kbasestorage.h"
#include <cstdlib>
#include <memory>
#include <utility>

namespace Kite {
	template <class T, bool deallocationFlag = false>
	class KSTDPoolAllocator {
		template<class T, Bool deallocationFlag>
		friend class KSTDPoolAllocator;
	public:
		typedef size_t    size_type;
		typedef ptrdiff_t difference_type;
		typedef T*        pointer;
		typedef const T*  const_pointer;
		typedef T&        reference;
		typedef const T&  const_reference;
		typedef T         value_type;

		/// Default constructor
		KSTDPoolAllocator(){}

		/// Copy constructor
		KSTDPoolAllocator(const KSTDPoolAllocator & Copy) {
			*this = Copy;
		}

		template <class U>
		KSTDPoolAllocator(const KSTDPoolAllocator<U, deallocationFlag> & Copy) {
			_kpool = Copy._kpool;
			*this = Copy;
		}

		/// usage:
		/// beware of most vexing parse (two extra parenthesis in contructure)
		/// eg: std::vector<int, KSTDAllocator<int>> myVec((KSTDAllocator<int>(BaseAlloc)));
		KSTDPoolAllocator(KBaseStorage &PoolStorage)
		{
			_kpool.reset(&PoolStorage);
		}

		~KSTDPoolAllocator() {
			reset();
		}

		pointer allocate(size_type n, const void * = 0) {
			KDEBUG_ASSERT(_kpool != nullptr);
			size_t size = sizeof(T) * n;
			T* t = (T *)_kpool->allocate(size, alignof(T));
			if (t == nullptr) {
				KDEBUG_PRINT("allocation failed. There is not enough room")
			}
			return t;
		}

		void deallocate(void* p, size_type) {
			KDEBUG_ASSERT(_kpool);
			if (p && deallocationFlag) {
				_kpool->deallocate(p);
			}
		}

		void construct(pointer p, const T& val) {
			new ((T*)p) T(val);
		}

		/// Call constructor with many arguments
		template<typename U, typename... Args>
		void construct(U* p, Args&&... args) {
			// Placement new
			::new((void *)p) U(std::forward<Args>(args)...);
		}

		void destroy(pointer p) { p->~T(); }

		pointer address(reference x) const { return &x; }

		const_pointer address(const_reference x) const { return &x; }

		size_type max_size() const { 
			KDEBUG_ASSERT(_kpool != nullptr);
			return (_kpool->getSize() \ sizeof(T));
		}

		template <class U>
		struct rebind { typedef KSTDPoolAllocator<U, deallocationFlag> other; };

		KSTDPoolAllocator<T>&  operator=(const KSTDPoolAllocator& Copy) { 
			_kpool = Copy._kpool;
			return *this;
		}

		template <class U>
		KSTDPoolAllocator& operator=(const KSTDPoolAllocator<U, deallocationFlag> & Copy) { 
			_kpool = Copy._kpool;
			return *this;
		}

		/// Reinit the allocator. All existing allocated memory will be lost
		void reset() {
			if (_kpool != nullptr) {
				_kpool.reset();
			}
		}

		const KBaseStorage &getMemoryPool() const {
			return *_kpool;
		}

	private:
		std::shared_ptr<KBaseStorage> _kpool;
	};

	template <typename T, typename U>
	inline bool operator==(const KSTDPoolAllocator<T>&, const KSTDPoolAllocator<U>) { return true; }

	template <typename T, typename U>
	inline bool operator!=(const KSTDPoolAllocator<T>&, const KSTDPoolAllocator<U>) { return false; }
}
#endif // KSTDPOOLALLOCATOR_H