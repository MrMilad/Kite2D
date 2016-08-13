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
#include "Kite/memory/kfreeliststorage.h"

namespace Kite {
	KFreeListStorage::KFreeListStorage(SIZE size, void* start)
		: KBaseStorage(size, start), _kfreeBlocks((FreeBlock*)start) {
		KD_ASSERT(size > sizeof(FreeBlock));

		_kfreeBlocks->size = size;
		_kfreeBlocks->next = nullptr;
	}

	KFreeListStorage::~KFreeListStorage() {
		_kfreeBlocks = nullptr;
	}

	void* KFreeListStorage::allocate(SIZE size, U8 alignment) {
		KD_ASSERT(size != 0 && alignment != 0);

		FreeBlock* prev_free_block = nullptr;
		FreeBlock* free_block = _kfreeBlocks;

		while (free_block != nullptr) {
			//Calculate adjustment needed to keep object correctly aligned
			U8 adjustment = Internal::alignForwardAdjustmentWithHeader(free_block, alignment, sizeof(AllocationHeader));

			SIZE total_size = size + adjustment;

			//If allocation doesn't fit in this FreeBlock, try the next
			if (free_block->size < total_size) {
				prev_free_block = free_block;
				free_block = free_block->next;
				continue;
			}

			static_assert(sizeof(AllocationHeader) >= sizeof(FreeBlock), "sizeof(AllocationHeader) < sizeof(FreeBlock)");

			//If allocations in the remaining memory will be impossible
			if (free_block->size - total_size <= sizeof(AllocationHeader)) {
				//Increase allocation size instead of creating a new FreeBlock
				total_size = free_block->size;

				if (prev_free_block != nullptr)
					prev_free_block->next = free_block->next;
				else
					_kfreeBlocks = free_block->next;
			} else {
				//Else create a new FreeBlock containing remaining memory
				FreeBlock* next_block = (FreeBlock*)(Internal::add(free_block, total_size));
				next_block->size = free_block->size - total_size;
				next_block->next = free_block->next;

				if (prev_free_block != nullptr)
					prev_free_block->next = next_block;
				else
					_kfreeBlocks = next_block;
			}

			uintptr_t aligned_address = (uintptr_t)free_block + adjustment;

			AllocationHeader* header = (AllocationHeader*)(aligned_address - sizeof(AllocationHeader));
			header->size = total_size;
			header->adjustment = adjustment;

			_kusedMem += total_size;
			_knumAlloc++;

			KD_ASSERT(Internal::alignForwardAdjustment((void*)aligned_address, alignment) == 0);

			return (void*)aligned_address;
		}

		KD_PRINT("Couldn't find free block large enough!");

		return nullptr;
	}

	void KFreeListStorage::deallocate(void* p) {
		KD_ASSERT(p != nullptr);

		AllocationHeader* header = (AllocationHeader*)Internal::subtract(p, sizeof(AllocationHeader));

		uintptr_t   block_start = reinterpret_cast<uintptr_t>(p) - header->adjustment;
		SIZE block_size = header->size;
		uintptr_t   block_end = block_start + block_size;

		FreeBlock* prev_free_block = nullptr;
		FreeBlock* free_block = _kfreeBlocks;

		while (free_block != nullptr) {
			if ((uintptr_t)free_block >= block_end)
				break;

			prev_free_block = free_block;
			free_block = free_block->next;
		}

		if (prev_free_block == nullptr) {
			prev_free_block = (FreeBlock*)block_start;
			prev_free_block->size = block_size;
			prev_free_block->next = _kfreeBlocks;

			_kfreeBlocks = prev_free_block;
		} else if ((uintptr_t)prev_free_block + prev_free_block->size == block_start) {
			prev_free_block->size += block_size;
		} else {
			FreeBlock* temp = (FreeBlock*)block_start;
			temp->size = block_size;
			temp->next = prev_free_block->next;
			prev_free_block->next = temp;

			prev_free_block = temp;
		}

		if (free_block != nullptr && (uintptr_t)free_block == block_end) {
			prev_free_block->size += free_block->size;
			prev_free_block->next = free_block->next;
		}

		_knumAlloc--;
		_kusedMem -= block_size;
	}
}