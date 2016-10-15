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
#include "Kite/graphic/kreggcullingcom.h"
#include "Kite/graphic/krendercom.h"
#include "Kite/graphic/kquadcom.h"
#include "Kite/graphic/korthomapcom.h"
#include <LooseQuadtree.h>

namespace Kite {

	KGCullingSys::KGCullingSys():
		_klastEman(nullptr),
		_kobjpool(nullptr),
		_kqtree(nullptr)
	{}
	
	bool KGCullingSys::update(F32 Delta, KEntityManager *EManager, KResourceManager *RManager) {
		EDITOR_STATIC const bool isregist = EManager->isRegisteredComponent(CTypes::RegisterGCulling);

		_krman = RManager;

		if (isregist) {
			// check if scene changed. so we need to clear quad tree
			if (_klastEman != nullptr && _klastEman != EManager) {
				_kqtree->Clear();
				delete _kobjpool;
				_kobjpool = new memory::memory_pool<>(sizeof(KQTreeObject), KGCULLING_MEM_CHUNK * sizeof(KQTreeObject));
			}
			_klastEman = EManager;

			// registering phase. register new objects to system (static -> qtree and dynamic -> bf list)
			// we cant use static pointer to the continer because every scene has its own entity manager and continers.
			auto continer = EManager->getComponentStorage<KRegGCullingCom>(CTypes::RegisterGCulling);
			bool haveReg = false;
			for (auto it = continer->begin(); it != continer->end(); ++it) {
				haveReg = true;
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
			if (haveReg) EManager->clearComponentStorage<KRegGCullingCom>(CTypes::RegisterGCulling);
		}
		return true;
	}

	bool KGCullingSys::inite(void *Data) {
		_kquadChanged = true;
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

		// check renderables
		auto com = (KRenderCom *)Ent->getComponent(CTypes::RenderInstance);
		
		// ignore tiles
		if (com->getRenderable() == CTypes::OrthogonalMapView) return;

		// restrive renderable component
		auto renderCom = Ent->getComponent(com->getRenderable());
		if (renderCom != nullptr) {
			auto obj = new(_kobjpool->allocate_node()) KQTreeObject;
			dynamic_cast<KRenderable *>(renderCom)->getBoundingRect(obj->bound);
			_computeParentPosition(Ent, obj->bound);
			obj->ent = Ent->getHandle();
			obj->com = renderCom->getHandle();

			// set remove callback
			gcom->_kcallb = _removeQTreeObject;
			gcom->_ksysptr = this;
			gcom->_kobjptr = obj;

			_kqtree->Insert(obj);
			_kquadChanged = true;
		}

		// check other components ...
	}

	void KGCullingSys::_addToBFList(KEntity *Ent) {
		
		auto gcom = (KGCullingCom *)Ent->getComponent(CTypes::GCulling);

		// check renderables
		auto com = (KRenderCom *)Ent->getComponent(CTypes::RenderInstance);

		// ignore tiles
		if (com->getRenderable() == CTypes::OrthogonalMapView) return;

		// restrive renderable component
		auto renderCom = Ent->getComponent(com->getRenderable());
		if (renderCom != nullptr) {
			KBFObject obj;
			obj.ent = Ent->getHandle();
			obj.com = renderCom->getHandle();

			// set remove callback
			gcom->_kcallb = _removeBFObject;
			gcom->_ksysptr = this;
			gcom->_kobjptr = nullptr;
			gcom->_kobjIndex = _kbflist.size() - 1;
			_kbflist.push_back(obj);
		}
	}

	void KGCullingSys::_removeQTreeObject(KGCullingCom *Com) {
		auto thisptr = (KGCullingSys *)Com->_ksysptr;
		auto objptr = (KQTreeObject *)Com->_kobjptr;

		thisptr->_kqtree->Remove(objptr);
		thisptr->_kquadChanged = true;
	}

	void KGCullingSys::_removeBFObject(KGCullingCom *Com) {
		auto thisptr = (KGCullingSys *)Com->_ksysptr;
		
		// swap and pop_back methode
		thisptr->_kbflist[Com->_kobjIndex] = thisptr->_kbflist.back();
		thisptr->_kbflist.pop_back();
	}

	void KGCullingSys::_computeParentPosition(KEntity *Entity, KRectF32 &Output){
		auto trcom = (KTransformCom *)Entity->getComponent(CTypes::Transform, "");
		if (Entity->getParentHandle() != _klastEman->getRoot()) {
			_computeParentPosition(_klastEman->getEntity(Entity->getParentHandle()), Output);
		}
		if (trcom) {
			Output += trcom->getPosition();
			Output -= trcom->getCenter();
		}
	}

	void KGCullingSys::queryObjects(const KRectF32 &Area, const std::bitset<KENTITY_LAYER_SIZE> &Layers, GCullingObjectsFilter Filter,
									const KEntityManager *EMan, std::vector<std::pair<KEntity *, KRenderable *>> &Output) {
		Output.resize(0);
		if (_klastEman != nullptr && _klastEman == EMan) {
			// tilemaps
			if (((U8)Filter & (U8)GCullingObjectsFilter::TILE) == (U8)GCullingObjectsFilter::TILE) {

				// iterate over all tilemaps components
				auto continer = _klastEman->getComponentStorage<KOrthoMapCom>(CTypes::OrthogonalMapView);
				for (auto it = continer->begin(); it != continer->end(); ++it) {
					auto owner = _klastEman->getEntity(it->getOwnerHandle());
					if (owner->isActive() && it->isVisible()) {
						// check layer
						if (Layers.test(owner->getLayer())) {

							it->updateRes();

							// check bounding rect
							KRectF32 brect;
							it->getBoundingRect(brect);
							_computeParentPosition(owner, brect);
							if (Area.overlapRect(brect)) {
								auto trcom = (KTransformCom *)owner->getComponent(CTypes::Transform);

								// set culling area
								it->setCullingArea(Area - trcom->getPosition());

								Output.push_back({ owner, dynamic_cast<KRenderable *>(&(*it)) });
							}
						}
					}
				}
			}

			// static objects
			if (((U8)Filter & (U8)GCullingObjectsFilter::STATIC) == (U8)GCullingObjectsFilter::STATIC &&
				!_kqtree->IsEmpty()) {
				// inite query area
				static auto lastArea = Area;
				static std::vector<std::pair<KEntity *, KRenderable *>> lastCatch(100);

				loose_quadtree::BoundingBox<F32> region(Area.left, Area.bottom, Area.right - Area.left, Area.top - Area.bottom);

				
				if (lastArea == Area && !_kquadChanged) {
					// using catched objects if there are no changes
					for (auto it = lastCatch.begin(); it != lastCatch.end(); ++it) {
						Output.push_back(*it);
					}
					return;
				} 

				// query tree
				auto query = _kqtree->QueryIntersectsRegion(region);
				_kquadChanged = false;
				lastArea = Area;
				lastCatch.resize(0);

				// iterate over queried objects
				while (!query.EndOfQuery()) {
					auto it = query.GetCurrent();
					auto ent = _klastEman->getEntity(it->ent);

					if (ent->isActive()) {
						// check layer
						if (Layers.test(ent->getLayer())) {
							auto com = ent->getComponentByHandle(it->com);
							auto graphicCom = dynamic_cast<KRenderable *>(com);
							auto transformCom = (KTransformCom *)ent->getComponent(CTypes::Transform);

							if (graphicCom->isVisible() && graphicCom->getIndexSize() > 0) {
								com->updateRes();
								Output.push_back({ ent, graphicCom });
								lastCatch.push_back({ ent, graphicCom });
							}
						}
					}
					query.Next();
				}
			}

			// dynamic objects
			if (((U8)Filter & (U8)GCullingObjectsFilter::DYNAMIC) == (U8)GCullingObjectsFilter::DYNAMIC) {
				auto end = _kbflist.end();
				for (auto it = _kbflist.begin(); it != end; ++it) {
					// bounding rect of dynamic objects
					auto ent = _klastEman->getEntity(it->ent);
					if (ent->isActive()) {

						// check layer
						if (Layers.test(ent->getLayer())) {
							auto com = ent->getComponentByHandle(it->com);
							auto graphicCom = dynamic_cast<KRenderable *>(com);
							auto transformCom = (KTransformCom *)ent->getComponent(CTypes::Transform);

							if (graphicCom->isVisible() && graphicCom->getIndexSize() > 0) {
								com->updateRes();

								// check bouning rect
								KRectF32 brect;
								graphicCom->getBoundingRect(brect);
								_computeParentPosition(ent, brect);
								if (Area.overlapRect(brect)) {
									Output.push_back({ ent, graphicCom });
								}
							}
						}
					}
				}
			}
			return;
		}
		KD_PRINT("gculling query failed. a new (or null) entity manager detected and system need update");
	}
}