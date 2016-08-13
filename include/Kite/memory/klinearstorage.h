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
#ifndef KLINEARSTORAGE_H
#define KLINEARSTORAGE_H

#include "Kite/core/kcoredef.h"
#include "kite/core/knoncopyable.h"
#include "Kite/memory/kbasestorage.h"

namespace Kite {
	class KITE_FUNC_EXPORT KLinearStorage : public KBaseStorage, KNonCopyable {
	public:
		KLinearStorage(SIZE Size, void *Start);
		~KLinearStorage();

		// New allocations simply move the pointer to the first free address forward.
		void* allocate(SIZE Size, U8 Alignment) override;

		// Individual deallocations cannot be made in linear allocators,
		// instead use clear() to completely clear the memory used by the allocator.
		void deallocate(void * Pointer) override;

		void clear();

	private:
		void *_kcurPos;
	};

	inline KLinearStorage* newKLinearStorage(SIZE Size, KBaseStorage& Storage) {
		void* p = Storage.allocate(Size + sizeof(KLinearStorage), __alignof(KLinearStorage));
		return new (p) KLinearStorage(Size, Internal::add(p, sizeof(KLinearStorage)));
	}

	inline void deleteLinearAllocator(KLinearStorage& LinearStorage, KBaseStorage& Storage) {
		LinearStorage.~KLinearStorage();

		Storage.deallocate(&LinearStorage);
	}
}

#endif // KLINEARSTORAGE_H