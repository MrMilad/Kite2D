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
#include "Kite/core/kcomponent.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/math/kmathstructs.h"
#include "krenderable.h"
#include <string>
#include <vector>
#include <kquadcom.khgen.h>

KMETA
namespace Kite{
	KM_CLASS(COMPONENT)
    class KITE_FUNC_EXPORT KQuadCom : public KComponent, public KRenderable{
		friend class KRenderSys;
		KM_INFO(KI_NAME = "Quad");
		KMETA_KQUADCOM_BODY();
    public:
		KQuadCom(const std::string &Name = "");

		void attached(KEntity *Owner) override;

		void deattached(KEntity *Owner) override;

		RecieveTypes onMessage(KMessage *Message, MessageScope Scope) override;

		KM_PRO_SET(KP_NAME = "width")
		void setWidth(F32 Width);

		KM_PRO_GET(KP_NAME = "width", KP_TYPE = F32, KP_CM = "width of the quad")
		inline F32 getWidth() const { return _kwidth; }

		KM_PRO_SET(KP_NAME = "height")
		void setHeight(F32 Height);

		KM_PRO_GET(KP_NAME = "height", KP_TYPE = F32, KP_CM = "height of the quad")
		inline F32 getHeight() const { return _kheight; }

		KM_PRO_SET(KP_NAME = "tint")
		void setColor(const KColor &Color);

		KM_PRO_GET(KP_NAME = "tint", KP_TYPE = KColor, KP_CM = "tint color")
		inline const KColor &getTintColor() const { return _kcolor; }

		KM_PRO_SET(KP_NAME = "shaderProgram")
		void setShader(const KStringID &ShaderProgram);

		KM_PRO_GET(KP_NAME = "shaderProgram", KP_TYPE = KStringID, KP_CM = "name of the shader program", KP_RES = RTypes::ShaderProgram)
		inline const KStringID &getShader() const override { return _kshprog; }

		KM_PRO_SET(KP_NAME = "textureGroup")
		void setAtlasTextureArraye(const KStringID &TextureArrayName);

		KM_PRO_GET(KP_NAME = "textureGroup", KP_TYPE = KStringID, KP_CM = "name of the atlas texture group",
				   KP_RES = RTypes::TextureGroup)
		inline const KStringID &getAtlasTextureArray() const override { return _ktextureArrayName; }

		KM_PRO_SET(KP_NAME = "textureIndex")
		void setTextureArrayIndex(U16 Index);

		KM_PRO_GET(KP_NAME = "textureIndex", KP_TYPE = U16, KP_CM = "index of the texture group")
		inline U16 getTextureArrayIndex() const { return _ktindex; }

		KM_PRO_GET(KP_NAME = "atlasIndex", KP_TYPE = KAtlasItem, KP_CM = "atlas item")
		inline const KAtlasItem &getAtlasItem() const { return _katlasItem; }

		KM_PRO_SET(KP_NAME = "atlasIndex")
		void setAtlasItem(const KAtlasItem &AtlasItem);

		KM_PRO_SET(KP_NAME = "visible")
		inline void setVisible(bool Visible) { _kisVisible = Visible; }

		KM_PRO_GET(KP_NAME = "visible", KP_TYPE = bool, KP_CM = "object visibility")
		inline bool isVisible() const override { return _kisVisible; }

		KM_FUN()
		inline void getBoundingRect(KRectF32 &Output) const override;

		// shuld defined in particle component
		//inline void setRelativeTransform(bool Relative) { _krelTrans = Relative; }
		//inline bool getRelativeTransform() const { return _krelTrans; }
		//inline void setReverseRender(bool Reverse) { _kreverse = Reverse; }
		//inline bool getReverseRender() const { return _kreverse; }

	protected:
		/// renderable interface
		inline const std::vector<KGLVertex> *getVertex() const override { return &_kvertex; }
		inline const U32 getIndexSize() const override { return _kindexsize; }
		inline const std::vector<KPointSprite> *getPoint() const override { return nullptr; }
		inline GLPrimitive getGeoType() const override { return GLPrimitive::TRIANGLES; }
		inline bool isIndexed() const override { return true; }
		inline bool isPoint() const override { return false; }

    private:
		void _setDim();
		KM_VAR() U16 _ktindex;
		KM_VAR() KAtlasItem _katlasItem;
		KM_VAR() bool _kisVisible;
		KM_VAR() U32 _kindexsize;
		KM_VAR() std::vector<KGLVertex> _kvertex;
		KM_VAR() F32 _kwidth;
		KM_VAR() F32 _kheight;
		KM_VAR() KColor _kcolor;
		KM_VAR() KStringID _kshprog;
		KM_VAR() KStringID _ktextureArrayName;
    };
}

#endif // KQUADCOM_H
