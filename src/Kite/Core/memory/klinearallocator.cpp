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
#include "Kite/Core/memory/KLinearAllocator.h"

namespace Kite {

	KLinearAllocator::KLinearAllocator(SIZE size, void *start)
		: KBaseAllocator(size, start), _kcurPos(start) {
		KDEBUG_ASSERT(size > 0);
	}

	KLinearAllocator::~KLinearAllocator() {
		_kcurPos = nullptr;
	}

	void* KLinearAllocator::allocate(SIZE size, U8 alignment) {
		KDEBUG_ASSERT(size != 0);

		U8 adjustment = Internal::alignForwardAdjustment(_kcurPos, alignment);

		if (_kusedMem + adjustment + size > _ksize)
			return nullptr;

		U32 *aligned_address = (U32 *)_kcurPos + adjustment;

		_kcurPos = (void *)(aligned_address + size);

		_kusedMem += size + adjustment;
		_knumAlloc++;

		return (void *)aligned_address;
	}

	void KLinearAllocator::deallocate(void* p) {
		KDEBUG_ASSERT("Use clear() instead");
	}

	void KLinearAllocator::clear() {
		_knumAlloc = 0;
		_kusedMem = 0;

		_kcurPos = _kstart;
	}
}