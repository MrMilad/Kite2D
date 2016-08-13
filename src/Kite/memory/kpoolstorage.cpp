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
#include "Kite/memory/kpoolstorage.h"

namespace Kite {
	KPoolStorage::KPoolStorage(SIZE ObjectSize, U8 ObjectAlignment, SIZE Size, void *Mem)
		: KBaseStorage(Size, Mem), _kobjectSize(ObjectSize), _kobjectAlignment(ObjectAlignment) {
		KD_ASSERT(ObjectSize >= sizeof(void*));

		// Calculate adjustment needed to keep object correctly aligned
		U8 adjustment = Internal::alignForwardAdjustment(Mem, ObjectAlignment);

		_kfreeList = (void**)Internal::add(Mem, adjustment);

		_kmaxpsize = (Size - adjustment) / ObjectSize;

		void** p = _kfreeList;

		//Initialize free blocks list
		for (size_t i = 0; i < _kmaxpsize - 1; i++) {
			*p = Internal::add(p, ObjectSize);
			p = (void**)*p;
		}

		*p = nullptr;
	}

	KPoolStorage::~KPoolStorage() {
		_kfreeList = nullptr;
	}

	void* KPoolStorage::allocate(SIZE Size, U8 Alignment) {
		KD_ASSERT(Size == _kobjectSize && Alignment == _kobjectAlignment);

		if (_kfreeList == nullptr)
			return nullptr;

		void* p = _kfreeList;

		_kfreeList = (void**)(*_kfreeList);

		_kusedMem += Size;
		_knumAlloc++;

		return p;
	}

	void KPoolStorage::deallocate(void* Pointer) {
		*((void**)Pointer) = _kfreeList;

		_kfreeList = (void**)Pointer;

		_kusedMem -= _kobjectSize;
		_knumAlloc--;
	}
}