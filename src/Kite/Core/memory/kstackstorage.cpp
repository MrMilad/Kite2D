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
#include "Kite/Core/memory/kstackstorage.h"

namespace Kite {
	KStackStorage::KStackStorage(SIZE Size, void *Start)
		: KBaseStorage(Size, Start), _kcurPos(Start) {
		KDEBUG_ASSERT(Size > 0);

#if _DEBUG
		_kprevPos = nullptr;
#endif
	}

	KStackStorage::~KStackStorage() {
#if _DEBUG
		_kprevPos = nullptr;
#endif

		_kcurPos = nullptr;
	}

	void* KStackStorage::allocate(SIZE Size, U8 Alignment) {
		KDEBUG_ASSERT(Size != 0);

		U8 adjustment = Internal::alignForwardAdjustmentWithHeader(_kcurPos, Alignment, sizeof(AllocationHeader));

		// not enough size
		if (_kusedMem + adjustment + Size > _ksize)
			return nullptr;

		void* aligned_address = Internal::add(_kcurPos, adjustment);

		//Add Allocation Header
		AllocationHeader* header = (AllocationHeader*)(Internal::subtract(aligned_address, sizeof(AllocationHeader)));

		header->adjustment = adjustment;

#if _DEBUG
		header->prev_address = _kprevPos;

		_kprevPos = aligned_address;
#endif

		_kcurPos = Internal::add(aligned_address, Size);

		_kusedMem += Size + adjustment;
		_knumAlloc++;

		return aligned_address;
	}

	void KStackStorage::deallocate(void *Pointer) {
		KDEBUG_ASSERT(Pointer == _kprevPos);

		//Access the AllocationHeader in the bytes before Pointer
		AllocationHeader* header = (AllocationHeader*)(Internal::subtract(Pointer, sizeof(AllocationHeader)));

		_kusedMem -= (UPTR)_kcurPos - (UPTR)Pointer + header->adjustment;

		_kcurPos = Internal::subtract(Pointer, header->adjustment);

#if _DEBUG
		_kprevPos = header->prev_address;
#endif

		_knumAlloc--;
	}
}