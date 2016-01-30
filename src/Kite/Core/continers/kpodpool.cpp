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

#include "kite/Core/continers/kpodpool.h"
#include "kite/Core/system/ksystemutil.h"
#include "Kite/Core/meta/kmetamanager.h"
#include "Kite/Core/meta/kmetaclass.h"
#include <luaintf\LuaIntf.h>
#include <algorithm>

namespace Kite {
	KPODPool::KPODPool(SIZE MaxPoolSize):
		_klist(nullptr), _kbaseAlloc(nullptr), _kalloc(nullptr),
		_kmaxSize(MaxPoolSize), _kusedSize(0), _kdata(nullptr)
	{
		KDEBUG_ASSERT(MaxPoolSize > 0);

		SIZE pSize = MaxPoolSize * sizeof(Internal::KPoolDictElement);
		_kdata = malloc(pSize);
		_kalloc = new KPoolAllocator(sizeof(Internal::KPoolDictElement), alignof(Internal::KPoolDictElement), pSize, _kdata);
		KDEBUG_ASSERT(_kalloc != nullptr);

		_klist = new std::list<Internal::KPoolDictElement, KSTDAllocator<Internal::KPoolDictElement>>(_kalloc);
		KDEBUG_ASSERT(_klist != nullptr);
	}

	KPODPool::KPODPool(SIZE MaxPoolSize, KBaseAllocator &Allocator):
		_klist(nullptr), _kbaseAlloc(&Allocator), _kalloc(nullptr),
		_kmaxSize(MaxPoolSize), _kusedSize(0)
	{
		KDEBUG_ASSERT(MaxPoolSize > 0);
		SIZE pSize = MaxPoolSize * sizeof( Internal::KPoolDictElement);

		_kalloc = newPoolAllocator(sizeof(Internal::KPoolDictElement), alignof(Internal::KPoolDictElement), pSize, Allocator);
		KDEBUG_ASSERT(_kalloc != nullptr);

		void *lData = Allocator.allocate(sizeof(std::list<Internal::KPoolDictElement, KSTDAllocator<Internal::KPODUnion>>),
										 alignof(std::list<Internal::KPoolDictElement, KSTDAllocator<Internal::KPODUnion>>));

		_klist = new (lData) std::list<Internal::KPoolDictElement, KSTDAllocator<Internal::KPoolDictElement>>(_kalloc);
		KDEBUG_ASSERT(_klist != nullptr);
	}

	KPODPool::~KPODPool(){
		// custom allocator
		if (_kbaseAlloc != nullptr) {
			deletePoolAllocator(*_kalloc, *_kbaseAlloc);

			_klist->~list();
			_kbaseAlloc->deallocate((void *)_klist);
			
		// default allocator
		} else {
			free(_kdata);
			delete _klist;
			delete _kalloc;
		}
	}

	void KPODPool::setInt(const std::string &Key, I32 Data){
		KDEBUG_ASSERT(_kmaxSize > _kusedSize);
		U32 hash;
		getHash32((void *) Key.c_str(), Key.size(), KHASH_SEED, (void *)&hash);

		Internal::KPoolDictElement elem;
		elem.data.i32 = Data;
		elem.hash = hash;
		elem.dataType = Internal::KPD_INT;

		_klist->push_back(elem);
		++_kusedSize;
	}

	void KPODPool::setFloat(const std::string &Key, F32 Data){
		KDEBUG_ASSERT(_kmaxSize > _kusedSize);
		U32 hash;
		getHash32((void *)Key.c_str(), Key.size(), KHASH_SEED, (void *)&hash);

		Internal::KPoolDictElement elem;
		elem.data.f32 = Data;
		elem.hash = hash;
		elem.dataType = Internal::KPD_FLT;

		_klist->push_back(elem);
		++_kusedSize;
	}

	void KPODPool::setBool(const std::string &Key, bool Data){
		KDEBUG_ASSERT(_kmaxSize > _kusedSize);
		U32 hash;
		getHash32((void *)Key.c_str(), Key.size(), KHASH_SEED, (void *)&hash);

		Internal::KPoolDictElement elem;
		elem.data.bl = Data;
		elem.hash = hash;
		elem.dataType = Internal::KPD_BOL;

		_klist->push_back(elem);
		++_kusedSize;
	}

	I32 KPODPool::getInt(const std::string &Key) const{
		U32 hash;
		getHash32((void *)Key.c_str(), Key.size(), KHASH_SEED, (void *)&hash);
		Internal::KPoolDictElement elem;
		elem.hash = hash;

		auto found = std::find(_klist->begin(), _klist->end(), elem);

		if (found != _klist->end()) {
			if (found->dataType == Internal::KPD_INT) {
				return found->data.i32;
			} else {
				KDEBUG_PRINT("missmatch data type");
			}
		} else {
			KDEBUG_PRINT("key not found");
		}

		return 0;
	}

	F32 KPODPool::getFloat(const std::string &Key) const{
		U32 hash;
		getHash32((void *)Key.c_str(), Key.size(), KHASH_SEED, (void *)&hash);
		Internal::KPoolDictElement elem;
		elem.hash = hash;

		auto found = std::find(_klist->begin(), _klist->end(), elem);

		if (found != _klist->end()) {
			if (found->dataType == Internal::KPD_FLT) {
				return found->data.f32;
			} else {
				KDEBUG_PRINT("missmatch data type");
			}
		} else {
			KDEBUG_PRINT("key not found");
		}

		return 0.0f;
	}

	bool KPODPool::getBool(const std::string &Key) const{
		U32 hash;
		getHash32((void *)Key.c_str(), Key.size(), KHASH_SEED, (void *)&hash);
		Internal::KPoolDictElement elem;
		elem.hash = hash;

		auto found = std::find(_klist->begin(), _klist->end(), elem);

		if (found != _klist->end()) {
			if (found->dataType == Internal::KPD_BOL) {
				return found->data.bl;
			} else {
				KDEBUG_PRINT("missmatch data type");
			}
		} else {
			KDEBUG_PRINT("key not found");
		}

		return false;
	}

	void KPODPool::clear(){
		_klist->clear();
		_kusedSize = 0;
	}

	KMETA_KPODPOOL_SOURCE();
}