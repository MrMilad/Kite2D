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
#ifndef KPROXYSTORAGE_H
#define KPROXYSTORAGE_H

#include "Kite/core/kcoredef.h"
#include "Kite/memory/kbasestorage.h"

namespace Kite {
	class KITE_FUNC_EXPORT KProxyStorage : public KBaseStorage {
	public:
		KProxyStorage(KBaseStorage& Allocator);
		~KProxyStorage();

		void* allocate(SIZE Size, U8 Alignment) override;

		void deallocate(void *Pointer) override;

	private:
		KBaseStorage& _kallocator;
	};

	inline KProxyStorage* newKProxyStorage(KBaseStorage& Storage) {
		void* p = Storage.allocate(sizeof(KProxyStorage), __alignof(KProxyStorage));
		return new (p) KProxyStorage(Storage);
	}

	inline void deleteProxyAllocator(KProxyStorage& ProxyStorage, KBaseStorage& Storage) {
		ProxyStorage.~KProxyStorage();

		Storage.deallocate(&ProxyStorage);
	}
}

#endif // KPROXYSTORAGE_H