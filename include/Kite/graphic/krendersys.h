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
#ifndef KRENDERSYS_H
#define KRENDERSYS_H

#include "Kite/core/kcoredef.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/ecs/ksystem.h"
#include <vector>
#include "krendersys.khgen.h"

//#define KRENDER_VBUFF_SIZE 40000	// item (KGLVertext)
//#define KRENDER_IBUFF_SIZE 60000	// item (U16)
//#define KRENDER_BUFF_SIZE 10000		// item (KRenderable)
#define KRENDER_CAMERA_SIZE 10		// item (size of pre-defined camera)

KMETA
namespace Kite{
	class KCameraCom;
	class KRenderable;
	class KShaderProgram;
	class KGCullingSys;
	class KVertexArray;
	class KVertexBuffer;
	class KFrameBuffer;
	class KTexture;
	class KAtlasTextureArray;

	KM_CLASS(SYSTEM)
	class KITE_FUNC_EXPORT KRenderSys : public KSystem{
		KM_INFO(KI_NAME = "Render");
		KRENDERSYS_BODY();
	public:
		KRenderSys();
		~KRenderSys();

		void reset(KEngine *Engine) override;

		bool update(F64 Delta) override;

		void nodeAdded(KNode *Node) override;

		void nodeRemoved(KNode *Node) override;

		void componentAdded(KComponent *Com) override;

		void componentRemoved(KComponent *Com) override;

		KM_PRO_GET(KP_NAME = "activeCulling", KP_TYPE = bool, KP_CM = "enable\disable object culling")
		inline bool isActiveCulling() const { return _kuseCull; }

		KM_PRO_SET(KP_NAME = "activeCulling")
		inline void setActiveCulling(bool Culling) { _kuseCull = Culling; }

	private:
		void _initeFrameBuffer(KAtlasTextureArray *Texture, U32 Index); // render to texture
		void _fillRenderList(KEntityManager *Eman, std::vector<std::pair<KEntity *, KRenderable *>> &Output);
		bool _initeMaterials(KRenderable *Com);
		bool _checkState(KRenderable *Rendeable);
		void _initeQuadIndex(std::vector<U16> *Buffer);
		void _computeParentsTransform(KEntityManager *Eman, KEntity *Entity, KMatrix3 *Matrix);
		//static void _updateInd(void *Data, U32 Offset, U32 DataSize, void *Sender);
		static void _updateVer(void *Data, U32 Offset, U32 DataSize, void *Sender);
		//static void _updatePar(void *Data, U32 Offset, U32 DataSize, void *Sender);
		
		bool _kuseCull;
		KVertexArray *_kvao;
		KVertexBuffer *_kvboInd;		/// index
		KVertexBuffer *_kvboVer;		/// xy (position)
		KVertexBuffer *_kvboPnt;		/// point sprite: point size and texture size (usage: Particle)
		KFrameBuffer *_kfbo;			/// render to texture
		KRenderState _kconfig;
		Internal::KGLState _klastState;
		KGCullingSys *_kcullSys;
		std::vector<KCameraCom *> _kcamList;

		struct updateData {
			std::vector<KRenderable *> objects;
			std::vector<KMatrix3> matrix;

			void clear() {
				objects.resize(0);
				matrix.resize(0);
			}
		};

		updateData _kupdata;
	};
}

#endif // KRENDERSYS_H
