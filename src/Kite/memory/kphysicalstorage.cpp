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
#include "Kite/Core/memory/kphysicalstorage.h"
#include <cstdlib>

namespace Kite {
	KPhysicalStorage::KPhysicalStorage()
		: KBaseStorage(0, nullptr) {}

	KPhysicalStorage::~KPhysicalStorage() {}

	void* KPhysicalStorage::allocate(SIZE size, U8 alignment) {
		KDEBUG_ASSERT(size != 0);

		void *p1 = nullptr;
		void **p2 = nullptr;
		int offset = alignment - 1 + sizeof(void*);
		p1 = malloc(size + offset);               // the line you are missing
		p2 = (void**)(((size_t)(p1)+offset)&~(alignment - 1));  //line 5
		p2[-1] = p1; //line 6

		KDEBUG_ASSERT(p2 != nullptr);

		_kusedMem += size + offset;
		++_knumAlloc;

		return p2;
	}

	void KPhysicalStorage::deallocate(void* p) {
		if (p != nullptr) {
			void* p1 = ((void**)p)[-1];
			free(p1);
		}
	}
}