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
#ifndef KGCULLINGSYS_H
#define KGCULLINGSYS_H

#include "Kite/core/kcoredef.h"
#include "Kite/ecs/ksystem.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/graphic/kgraphictypes.h"
#include <vector>
#include <bitset>
#include <foonathan/memory/memory_pool.hpp>
#include <foonathan/memory/namespace_alias.hpp>
#include "kgcullingsys.khgen.h"

#define KGCULLING_MEM_CHUNK 1000 // items (n x sizeof(KQTreeObject)

namespace  loose_quadtree {
	template <typename NumberT, typename ObjectT, typename BoundingBoxExtractorT>
	class LooseQuadtree;

	template <typename NumberT>
	struct BoundingBox;
}

KMETA
namespace Kite {

	/*struct KGCullingObject {
		KEntity *entity;
		KRenderable *component;

		inline bool operator<(const KGCullingObject& Other) {
			return (entity->getZOrder() < Other.entity->getZOrder());
		}
	};*/

	class KCameraCom;

	KM_CLASS(SYSTEM) // sub system
	class KITE_FUNC_EXPORT KGCullingSys : public KSystem {
		KM_INFO(KI_NAME = "GCulling");
	public:
		KGCullingSys();

		void reset(KNode *Hierarchy, KEngine *Engine) override;

		bool update(F64 Delta) override;

		void nodeAdded(KNode *Node) override;

		void nodeRemoved(KNode *Node) override;

		void componentAdded(KComponent *Com) override;

		void componentRemoved(KComponent *Com) override;

	private:
		void scan(KNode *Root);
		struct KQTreeObject {
			KRectF32 bound;
			KNode *node;
		};

		struct KBFObject {
			KNode *node;
		};

		void _switchType(KCullable *Interface); // switch between static and dynamic
		void _computeParentPosition(KCullable *Node, KRectF32 &Output);

		void _addToTree(KCullable *Node);
		void _addToBFList(KCullable *Node);

		bool _kquadChanged;
		loose_quadtree::LooseQuadtree<F32, KQTreeObject, KGCullingSys> *_kqtree;
		std::vector<KBFObject> _kbflist;
		memory::memory_pool<> *_kobjpool;

	public:
		template <typename NumberT>
		static void ExtractBoundingBox(KQTreeObject *in, loose_quadtree::BoundingBox<NumberT> *out) {
			out->left = in->bound.left;
			out->top = in->bound.bottom;
			out->width = in->bound.right - in->bound.left;
			out->height = in->bound.top - in->bound.bottom;
		}
	};
}

#endif // KGCULLINGSYS_H
