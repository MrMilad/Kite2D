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
#include "Kite/Core/memory/kproxystorage.h"

namespace Kite {
	KProxyStorage::KProxyStorage(KBaseStorage& Allocator)
		: KBaseStorage(Allocator.getSize(), Allocator.getStart()), _kallocator(Allocator) {

	}

	KProxyStorage::~KProxyStorage() {

	}

	void* KProxyStorage::allocate(SIZE size, U8 alignment) {
		KDEBUG_ASSERT(size != 0);
		_knumAlloc++;

		size_t mem = _kallocator.getUsedMemory();

		void* p = _kallocator.allocate(size, alignment);

		_kusedMem += _kallocator.getUsedMemory() - mem;

		return p;
	}

	void KProxyStorage::deallocate(void* p) {
		_knumAlloc--;

		size_t mem = _kallocator.getUsedMemory();

		_kallocator.deallocate(p);

		_kusedMem -= mem - _kallocator.getUsedMemory();
	}
}