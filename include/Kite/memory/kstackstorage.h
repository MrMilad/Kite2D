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
#ifndef KSTACKSTORAGE_H
#define KSTACKSTORAGE_H

#include "Kite/core/kcoredef.h"
#include "kite/core/knoncopyable.h"
#include "Kite/memory/kbasestorage.h"

namespace Kite {
	class KITE_FUNC_EXPORT KStackStorage : public KBaseStorage, KNonCopyable {
	public:
		KStackStorage(SIZE Size, void *Start);
		~KStackStorage();

		// New allocations move the pointer up by the requested number of bytes
		// plus the adjustment needed to align the address and store the allocation header.
		void* allocate(SIZE Size, U8 Alignment) override;

		// Memory must be deallocated in inverse order it was allocated!
		// So if you allocate object A and then object B you must free object B memory before you can free object A memory. 
		void deallocate(void *Pointer) override;

	private:
		struct AllocationHeader {
			#if _DEBUG
			void *prev_address;
			#endif
			U8 adjustment;
		};

		#if _DEBUG
		void* _kprevPos;
		#endif

		void*  _kcurPos;
	};

	inline KStackStorage *newKStackStorage(SIZE Size, KBaseStorage& Storage) {
		void *p = Storage.allocate(Size + sizeof(KStackStorage), __alignof(KStackStorage));
		return new (p) KStackStorage(Size, Internal::add(p, sizeof(KStackStorage)));
	}

	inline void deleteKStackStorage(KStackStorage &StackStorage, KBaseStorage &Storage) {
		StackStorage.~KStackStorage();

		Storage.deallocate(&StackStorage);
	}
}

#endif // KSTACKSTORAGE_H