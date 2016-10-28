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
#ifndef KBITMAPTEXTCOM_H
#define KBITMAPTEXTCOM_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcomponent.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/math/kmathstructs.h"
#include "krenderable.h"
#include <string>
#include <vector>
#include "kbitmaptextcom.khgen.h"

KMETA
namespace Kite{
	KM_CLASS(COMPONENT)
    class KITE_FUNC_EXPORT KBitmapTextCom : public KComponent, public KRenderable {
		friend class KRenderSys;
		KM_INFO(KI_NAME = "BitmapText"); 
		KMETA_KBITMAPTEXTCOM_BODY();
	public:
		KBitmapTextCom(const std::string &Name = "");

		void attached(KEntity *Owner) override;

		void deattached(KEntity *Owner) override;

		RecieveTypes onMessage(KMessage *Message, MessageScope Scope) override;

		bool updateRes() override;

		/// can use color index with this format:
		/// #n>     (n = index of color)
		/// double sharp will ignored and treated as single sharp for rendering
		KM_PRO_SET(KP_NAME = "text")
		void setText(const std::string &Text);

		KM_PRO_GET(KP_NAME = "text", KP_TYPE = std::string, KP_CM = "text")
		inline const std::string &getText() const { return _ktext; }

		/// 30 by default
		KM_PRO_SET(KP_NAME = "paragraphSize")
		void setParagraphSize(U32 Size);

		KM_PRO_GET(KP_NAME = "paragraphSize", KP_TYPE = U32, KP_CM = "empty space between two paragraph")
		inline U32 getParagraphSize() const { return _kpgraph; }

		/// 0 by default
		KM_PRO_SET(KP_NAME = "strideSize")
		void setStrideSize(U32 Size);

		KM_PRO_GET(KP_NAME = "strideSize", KP_TYPE = U32, KP_CM = "size ofempty space between two digits")
		inline U32 getStrideSize() const { return _kmid; }

		/// 15 by default
		KM_PRO_SET(KP_NAME = "spaceSize")
		void setSpaceSize(U32 Size);

		KM_PRO_GET(KP_NAME = "spaceSize", KP_TYPE = U32, KP_CM = "size of space character")
		inline U32 getSpaceSize() const { return _kspace; }

		KM_PRO_SET(KP_NAME = "shaderProgram")
		void setShader(const KStringID &ShaderProgram);

		KM_PRO_GET(KP_NAME = "shaderProgram", KP_TYPE = KStringID, KP_CM = "name of the shader program", KP_RES = RTypes::ShaderProgram)
		inline const KStringID &getShader() const { return _kshprogName; }

		KM_PRO_SET(KP_NAME = "textureGroup")
		void setAtlasTextureArraye(const KStringID &TextureArrayName);

		KM_PRO_GET(KP_NAME = "textureGroup", KP_TYPE = KStringID, KP_CM = "name of the atlas texture group",
				   KP_RES = RTypes::TextureGroup)
		inline const KStringID &getAtlasTextureArray() const { return _ktextureArrayName; }

		KM_PRO_SET(KP_NAME = "textureIndex")
		void setTextureArrayIndex(U16 Index);

		KM_PRO_GET(KP_NAME = "textureIndex", KP_TYPE = U16, KP_CM = "index of the texture group")
		inline U16 getTextureArrayIndex() const { return _ktindex; }

		KM_PRO_SET(KP_NAME = "glyphMarker")
		void setGlyphMarker(const KGlyphMarker &Marker);

		KM_PRO_GET(KP_NAME = "glyphMarker", KP_TYPE = KGlyphMarker, KP_CM = "glyph marker in atlas texture")
		inline const KGlyphMarker &getGlyphMarker() const { return _kmarker; }

		KM_PRO_SET(KP_NAME = "visible")
		inline void setVisible(bool Visible) { _kisVisible = Visible; }

		KM_PRO_GET(KP_NAME = "visible", KP_TYPE = bool, KP_CM = "object visibility")
		inline bool isVisible() const override { return _kisVisible; }

		/// return color index
		KM_FUN()
		U32 addColor(const KColor &Color);

		KM_FUN()
		void clearColors();

		KM_FUN()
		void getBoundingRect(KRectF32 &Output) const override;

	protected:
		/// renderable interface
		inline const std::vector<KGLVertex> *getVertex() const override { return &_kvertex; }
		inline const U32 getIndexSize() const override { return _kindexsize; }
		inline const std::vector<KPointSprite> *getPoint() const override { return nullptr; }
		inline GLPrimitive getGeoType() const override { return GLPrimitive::TRIANGLES; }
		inline bool isIndexed() const override { return true; }
		inline bool isPoint() const override { return false; }
		inline KShaderProgram *getShaderProg() const override { return _kshprog; }
		inline KAtlasTextureArray *getATextureArray() const override { return _katarray; }

    private:
		KM_VAR() bool _kisVisible;
		KM_VAR() KStringID _kshprogName;
		KM_VAR() KStringID _ktextureArrayName;
		KM_VAR() U16 _ktindex;
		KM_VAR() KGlyphMarker _kmarker;
        KM_VAR() std::string _ktext;
		KM_VAR() F32 _kwidth;
		KM_VAR() F32 _kheight;
		KM_VAR() U32 _kmid;
		KM_VAR() U32 _kspace;
		KM_VAR() U32 _kindexsize;
		KM_VAR() U32 _kpgraph;
		KM_VAR() std::vector<KColor> _kcolors;
		KM_VAR() std::vector<KGLVertex> _kvertex;

		bool _kneedRedraw;
		KShaderProgram *_kshprog;
		KAtlasTextureArray *_katarray;
    };
}

#endif // KBITMAPTEXTCOM_H
