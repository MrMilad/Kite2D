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
#ifndef KQUADCOM_H
#define KQUADCOM_H

#include "Kite/core/kcoredef.h"
#include "Kite/ecs/kcomponent.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/graphic/krenderable.h"
#include "Kite/math/kmathstructs.h"
#include <string>
#include <vector>
#include <kquadcom.khgen.h>

KMETA
namespace Kite{
	KM_CLASS(COMPONENT)
    class KITE_FUNC_EXPORT KQuadCom : public KComponent, public KRenderable{
		friend class KRenderSys;
		KM_INFO(KI_NAME = "Quad");
		KM_INFO(KI_DEP = KTransformCom);
		KM_INFO(KI_INT = KRenderable);
		KQUADCOM_BODY();
    public:

		KM_PRO_SET(KP_NAME = "width")
		void setWidth(F32 Width);

		KM_PRO_GET(KP_NAME = "width", KP_TYPE = F32, KP_CM = "width of the quad")
		inline F32 getWidth() const { return _kwidth; }

		KM_PRO_SET(KP_NAME = "height")
		void setHeight(F32 Height);

		KM_PRO_GET(KP_NAME = "height", KP_TYPE = F32, KP_CM = "height of the quad")
		inline F32 getHeight() const { return _kheight; }

		KM_PRO_SET(KP_NAME = "blendColor")
		void setBlendColor(const KColor &Color);

		KM_PRO_GET(KP_NAME = "blendColor", KP_TYPE = KColor, KP_CM = "blending color")
		inline const KColor &getBlendColor() const { return _kcolor; }

		KM_PRO_SET(KP_NAME = "shaderProgram")
		void setShader(const KSharedResource &ShaderProgram);
		
		KM_PRO_GET(KP_NAME = "shaderProgram", KP_CM = "shader program", KP_RES = Resource::SHADERPROGRAM)
		inline KSharedResource getShader() const { return _kshprog; }

		KM_PRO_SET(KP_NAME = "textureGroup")
		void setAtlasTextureArraye(const KSharedResource &TextureArray);

		KM_PRO_GET(KP_NAME = "textureGroup", KP_CM = "atlas texture group",
				   KP_RES = Resource::TEXTUREGROUP)
		inline KSharedResource getAtlasTextureArray() const { return _katarray; }

		KM_PRO_SET(KP_NAME = "textureIndex")
		void setTextureArrayIndex(U16 Index);

		KM_PRO_GET(KP_NAME = "textureIndex", KP_TYPE = U16, KP_CM = "index of the texture group")
		inline U16 getTextureArrayIndex() const { return _ktindex; }

		KM_PRO_GET(KP_NAME = "atlasID", KP_TYPE = KAtlasItem, KP_CM = "id of the atlas item")
		inline const KAtlasItem &getAtlasItem() const { return _katlasItem; }

		KM_PRO_SET(KP_NAME = "atlasID")
		void setAtlasItem(const KAtlasItem &AtlasItem);

		/// true by default
		KM_PRO_SET(KP_NAME = "isVisible")
		inline void setVisible(bool Visible) { _kvisible = Visible; }

		KM_PRO_GET(KP_NAME = "isVisible", KP_TYPE = bool, KP_CM = "object visibility")
		inline bool isVisible() const override { return _kvisible; }

		KM_PRO_GET(KP_NAME = "aabb", KP_TYPE = bool)
		KRectI32 getBoundingBox() const;

		// shuld defined in particle component
		//inline void setRelativeTransform(bool Relative) { _krelTrans = Relative; }
		//inline bool getRelativeTransform() const { return _krelTrans; }
		//inline void setReverseRender(bool Reverse) { _kreverse = Reverse; }
		//inline bool getReverseRender() const { return _kreverse; }

	protected:
		void inite() override;
		void attached() override;
		void deattached() override;

		/// renderable interface
		inline const std::vector<KGLVertex> *getVertex() const override { return &_kvertex; }
		inline const U32 getIndexSize() const override { return _kindexsize; }
		inline const std::vector<KPointSprite> *getPoint() const override { return nullptr; }
		inline GLPrimitive getGeoType() const override { return GLPrimitive::TRIANGLES; }
		inline bool isIndexed() const override { return true; }
		inline bool isPoint() const override { return false; }
		inline KSharedResource getShaderProg() const override { return _kshprog; }
		inline KSharedResource getATextureArray() const override { return _katarray; }

    private:
		void _setDim();
		KM_VAR() bool _kvisible;
		KM_VAR() U16 _ktindex;
		KM_VAR() KAtlasItem _katlasItem;
		KM_VAR() U32 _kindexsize;
		KM_VAR() std::vector<KGLVertex> _kvertex;
		KM_VAR() F32 _kwidth;
		KM_VAR() F32 _kheight;
		KM_VAR() KColor _kcolor;
		KM_VAR() KSharedResource _kshprog;
		KM_VAR() KSharedResource _katarray;
    };
}

#endif // KQUADCOM_H
