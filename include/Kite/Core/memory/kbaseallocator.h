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
#ifndef KBASEALLOCATOR_H
#define KBASEALLOCATOR_H

#include "Kite/Core/system/ksystemdef.h"
#include <new>

namespace Kite {
	class KITE_FUNC_EXPORT KBaseAllocator {
	public:
		KBaseAllocator(SIZE Size, void * Start) {
			_kstart = Start;
			_ksize = Size;

			_kusedMem = 0;
			_knumAlloc = 0;
		}

		virtual ~KBaseAllocator() {
			KDEBUG_ASSERT(_knumAlloc == 0 && _kusedMem == 0);

			_kstart = nullptr;
			_ksize = 0;
		}

		virtual void * allocate(SIZE Size, U8 Alignment = 4) = 0;

		virtual void deallocate(void *Pointer) = 0;

		inline void * getStart() const { return _kstart; }

		inline SIZE getSize() const { return _ksize; }

		inline SIZE getUsedMemory() const { return _kusedMem; }

		inline SIZE getNumAllocations() const { return _knumAlloc; }

		template <typename T>
		static void customDeleter(T *Object) {
			Object->~T();
		}

	protected:
		void * _kstart;
		SIZE _ksize;
		SIZE _kusedMem;
		SIZE _knumAlloc;
	};

	template <class T> T* allocateNew(KBaseAllocator& allocator) {
		return new (allocator.allocate(sizeof(T), __alignof(T))) T;
	}

	template <class T> T* allocateNew(KBaseAllocator& allocator, const T& t) {
		return new (allocator.allocate(sizeof(T), __alignof(T))) T(t);
	}

	template<class T> void deallocateDelete(KBaseAllocator& allocator, T& object) {
		object.~T();
		allocator.deallocate(&object);
	}

	template<class T> T* allocateArray(KBaseAllocator& allocator, SIZE length) {
		KDEBUG_ASSERT(length != 0);

		U8 headerSize = sizeof(SIZE) / sizeof(T);

		if (sizeof(SIZE) % sizeof(T) > 0)
			headerSize += 1;

		// Allocate extra space to store array length in the bytes before the array
		T * p = ((T*)allocator.allocate(sizeof(T)*(length + headerSize), __alignof(T))) + headerSize;

		*(((SIZE *)p) - 1) = length;

		for (SIZE i = 0; i < length; i++)
			new (&p[i]) T;

		return p;
	}

	template<class T> void deallocateArray(KBaseAllocator &allocator, T *array) {
		KDEBUG_ASSERT(array != nullptr);

		SIZE length = *(((SIZE *)array) - 1);

		for (SIZE i = 0; i < length; i++)
			array[i].~T();

		// Calculate how much extra memory was allocated to store the length before the array
		U8 headerSize = sizeof(SIZE) / sizeof(T);

		if (sizeof(SIZE) % sizeof(T) > 0)
			headerSize += 1;

		allocator.deallocate(array - headerSize);
	}

	namespace Internal {
		inline void *alignForward(void* Address, U8 Alignment) {
			return (void*)((reinterpret_cast<UPTR>(Address) + static_cast<UPTR>(Alignment - 1)) & static_cast<UPTR>(~(Alignment - 1)));
		}

		inline const void *alignForward(const void* Address, U8 Alignment) {
			return (void*)((reinterpret_cast<UPTR>(Address) + static_cast<UPTR>(Alignment - 1)) & static_cast<UPTR>(~(Alignment - 1)));
		}

		inline void* alignBackward(void *Address, U8 Alignment) {
			return (void*)(reinterpret_cast<UPTR>(Address) & static_cast<UPTR>(~(Alignment - 1)));
		}

		inline const void* alignBackward(const void *Address, U8 Alignment) {
			return (void*)(reinterpret_cast<UPTR>(Address) & static_cast<UPTR>(~(Alignment - 1)));
		}

		inline U8 alignForwardAdjustment(const void *Address, U8 Alignment) {
			U8 adjustment = Alignment - (reinterpret_cast<UPTR>(Address) & static_cast<UPTR>(Alignment - 1));

			if (adjustment == Alignment)
				return 0; //already aligned

			return adjustment;
		}

		inline U8 alignForwardAdjustmentWithHeader(const void *Address, U8 Alignment, U8 headerSize) {
			U8 adjustment = alignForwardAdjustment(Address, Alignment);

			U8 neededSpace = headerSize;

			if (adjustment < neededSpace) {
				neededSpace -= adjustment;

				//Increase adjustment to fit header
				adjustment += Alignment * (neededSpace / Alignment);

				if (neededSpace % Alignment > 0)
					adjustment += Alignment;
			}

			return adjustment;
		}

		inline U8 alignBackwardAdjustment(const void *Address, U8 Alignment) {
			U8 adjustment = reinterpret_cast<UPTR>(Address) & static_cast<UPTR>(Alignment - 1);

			if (adjustment == Alignment)
				return 0; //already aligned

			return adjustment;
		}

		inline void *add(void *P, SIZE X) {
			return (void*)(reinterpret_cast<UPTR>(P) + X);
		}

		inline const void *add(const void *P, SIZE X) {
			return (const void*)(reinterpret_cast<UPTR>(P) + X);
		}

		inline void *subtract(void *P, SIZE X) {
			return (void*)(reinterpret_cast<UPTR>(P) - X);
		}

		inline const void *subtract(const void *P, SIZE X) {
			return (const void*)(reinterpret_cast<UPTR>(P) - X);
		}
	}
}

#endif // KBASEALLOCATOR_H