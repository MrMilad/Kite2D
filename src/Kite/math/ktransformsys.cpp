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
#include "Kite/math/ktransformsys.h"
#include <cmath>
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/math/ktransformcom.h"
#include "Kite/ecs/knode.h"
#include <luaintf/LuaIntf.h>

namespace Kite {

	void KTransformSys::reset(KNode *Hierarchy, KSysInite *IniteData) {
		// cleanup
		_kcomList.resize(0);

		// scan new hierarchy
		scan(Hierarchy->getRoot());
	}

	bool KTransformSys::update(F64 Delta) {
		for (auto it = _kcomList.begin(); it != _kcomList.end(); ++it) {
			if ((*it)->getOwnerNode()->isActive()) {
				(*it)->computeMatrix();
			}
		}

		return true;
	}

	void KTransformSys::componentAdded(KComponent *Com) {
		if (Com->getDrivedType() == Component::TRANSFORM) {
			auto trcom = static_cast<KTransformCom *>(Com);
			trcom->_ksysIndex = _kcomList.size();
			_kcomList.push_back(trcom);
		}
	}

	void KTransformSys::componentRemoved(KComponent *Com) {
		if (Com->getDrivedType() == Component::TRANSFORM) {
			auto trcom = static_cast<KTransformCom *>(Com);

			// remove (swap methode)
			_kcomList.back()->_ksysIndex = trcom->_ksysIndex;
			_kcomList[trcom->_ksysIndex] = _kcomList.back();
			_kcomList.pop_back();
		}
	}

	void KTransformSys::nodeAdded(KNode *Node) {
		if (Node->hasComponent(Component::TRANSFORM)) {
			componentAdded(Node->getComponent(Component::TRANSFORM));
		}
	}

	void KTransformSys::nodeRemoved(KNode *Node) {
		if (Node->hasComponent(Component::TRANSFORM)) {
			componentRemoved(Node->getComponent(Component::TRANSFORM));
		}
	}

	void KTransformSys::scan(KNode *Hierarchy) {
		nodeAdded(Hierarchy);
		for (auto it = Hierarchy->childList()->begin(); it != Hierarchy->childList()->end(); ++it) {
			scan((*it));
		}
	}

	KTRANSFORMSYS_SOURCE();
}
