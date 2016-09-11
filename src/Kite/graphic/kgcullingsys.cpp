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
#include "Kite/graphic/kgcullingsys.h"
#include "Kite/graphic/kgcullingcom.h"
#include "Kite/graphic/kquadcom.h"
#include "Kite/graphic/korthogonalmap.h"
#include <LooseQuadtree.h>

namespace Kite {

	KGCullingSys::KGCullingSys():
		_klastEman(nullptr),
		_kobjpool(nullptr),
		_kqtree(nullptr)
	{}
	
	bool KGCullingSys::update(F32 Delta, KEntityManager *EManager, KResourceManager *RManager) {
		STATIC_OUT_EDITOR const bool isregist = EManager->isRegisteredComponent(CTypes::RegisterGCulling);

		if (isregist) {
			// check if scene changed. so we need to clear quad tree
			if (_klastEman != nullptr && _klastEman != EManager) {
				_kqtree->Clear();
				delete _kobjpool;
				_kobjpool = new memory::memory_pool<>(sizeof(KQTreeObject), KGCULLING_MEM_CHUNK * sizeof(KQTreeObject));
			}

			// registering phase. register new objects to system (static -> qtree and dynamic -> bf list)
			// we cant use static pointer to the continer because every scnen has its own entity manager and continers.
			auto continer = EManager->getComponentStorage<KGCullingCom>(CTypes::RegisterGCulling);
			for (auto it = continer->begin(); it != continer->end(); ++it) {
				auto EHandle = it->getOwnerHandle();
				auto entity = EManager->getEntity(EHandle);
				if (entity->isActive()) {

					// static objects will be added to quad tree
					if (entity->isStatic()) {
						_addToTree(entity);

					// dynamic objects will be added to brute force culling list
					} else {
						_addToBFList(entity);
					}
				}
			}

			// clear storage after registration
			EManager->clearComponentStorage<KGCullingCom>(CTypes::RegisterGCulling);
			_klastEman = EManager;
		}
		return true;
	}

	bool KGCullingSys::inite(void *Data) {
		_kobjpool = new memory::memory_pool<>(sizeof(KQTreeObject), KGCULLING_MEM_CHUNK * sizeof(KQTreeObject));
		_kqtree = new loose_quadtree::LooseQuadtree<F32, KQTreeObject, KGCullingSys>();

		_kbflist.reserve(KGCULLING_MEM_CHUNK);

		setInite(true);
		return true;
	}

	void KGCullingSys::destroy() {
		_kqtree->Clear();
		_kqtree->ForceCleanup();
		delete _kqtree;
		delete _kobjpool;
	}

	void KGCullingSys::_addToTree(KEntity *Ent) {
		auto gcom = (KGCullingCom *)Ent->getComponent(CTypes::GCulling);

		// check quad
		auto quad = (KQuadCom *)Ent->getComponent(CTypes::Quad);
		if (quad != nullptr) {
			auto obj = new(_kobjpool->allocate_node()) KQTreeObject;
			quad->getBoundingRect(obj->bound);
			obj->ent = Ent->getHandle();
			obj->com = quad->getHandle();

			// set remove callback
			gcom->_kcallb = _removeQTreeObject;
			gcom->_ksysptr = this;
			gcom->_kobjptr = obj;
			return;
		}

		// check other components ...
	}

	void KGCullingSys::_addToBFList(KEntity *Ent) {
		
		auto gcom = (KGCullingCom *)Ent->getComponent(CTypes::GCulling);

		// check quad
		auto quad = (KQuadCom *)Ent->getComponent(CTypes::Quad);
		if (quad != nullptr) {
			_kbflist.push_back(KBFObject());
			auto obj = &_kbflist.back();
			obj->ent = Ent->getHandle();
			obj->com = quad->getHandle();
			obj->index = _kbflist.size() - 1;

			// set remove callback
			gcom->_kcallb = _removeBFObject;
			gcom->_ksysptr = this;
			gcom->_kobjptr = obj;
			return;
		}

		// check other components ...
	}

	void KGCullingSys::_removeQTreeObject(KGCullingCom *Com) {
		auto thisptr = (KGCullingSys *)Com->_ksysptr;
		auto objptr = (KQTreeObject *)Com->_kobjptr;

		thisptr->_kqtree->Remove(objptr);
	}

	void KGCullingSys::_removeBFObject(KGCullingCom *Com) {
		auto thisptr = (KGCullingSys *)Com->_ksysptr;
		auto objptr = (KBFObject *)Com->_kobjptr;
		
		// swap and pop_back methode
		thisptr->_kbflist.back().index = objptr->index;
		thisptr->_kbflist[objptr->index] = thisptr->_kbflist.back();
		thisptr->_kbflist.pop_back();
	}

	void KGCullingSys::queryObjects(const KRectF32 &Area, const std::bitset<KENTITY_LAYER_SIZE> &Layers, GCullingObjectsFilter Filter,
									const KEntityManager *EMan, std::vector<KGCullingObject> &Output) {

		if (_klastEman != nullptr && _klastEman == EMan) {

			// tilemaps
			
		}

		KD_PRINT("gculling query failed. a new (or null) entity manager detected and system need update");
	}
}