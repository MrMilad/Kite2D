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
#ifndef KPOOLSTORAGE_H
#define KPOOLSTORAGE_H

#include "Kite/core/kcoredef.h"
#include "kite/core/knoncopyable.h"
#include "Kite/memory/kbasestorage.h"

namespace Kite {
	class KITE_FUNC_EXPORT KPoolStorage : public KBaseStorage, KNonCopyable {
	public:
		KPoolStorage(SIZE ObjectSize, U8 ObjectAlignment, SIZE Size, void *Mem);
		~KPoolStorage();

		/// only 1 object must allocated per call
		void *allocate(SIZE Size, U8 Alignment) override;

		/// only 1 object must deallocated per call
		void deallocate(void *Pointer) override;

		inline U32 getMaxPoolSize() const { return _kmaxpsize; }

		// custom deleter for shared pointers
		template<typename T>
		static void customDeleter(T *Object) {
			Object->~T();
			deallocate((void *)Object);
		}

	private:
		SIZE _kmaxpsize;
		SIZE _kobjectSize;
		U8 _kobjectAlignment;

		void ** _kfreeList;
	};

	inline KPoolStorage* newKPoolStorage(SIZE objectSize, U8 objectAlignment, SIZE size, KBaseStorage& Storage) {
		void *p = Storage.allocate(size + sizeof(KPoolStorage), __alignof(KPoolStorage));
		return new (p) KPoolStorage(objectSize, objectAlignment, size, Internal::add(p, sizeof(KPoolStorage)));
	}

	inline void deleteKPoolStorage(KPoolStorage &PoolStorage, KBaseStorage &Storage) {
		PoolStorage.~KPoolStorage();

		Storage.deallocate(&PoolStorage);
	}
}


#endif // KPOOLSTORAGE_H

