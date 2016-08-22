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
		KM_INFO(KI_CTYPE = "Quad");
		KMETA_KQUADCOM_BODY();
    public:
		KQuadCom(const std::string &Name = "");

		void attached(KEntity *Owner) override;

		void deattached(KEntity *Owner) override;

		RecieveTypes onMessage(KMessage *Message, MessageScope Scope) override;

		KM_PRO_SET(KP_NAME = "border")
		void setBorder(F32 Border);

		KM_PRO_GET(KP_NAME = "border", KP_TYPE = F32, KP_CM = "border tickness")
		inline F32 getBorder() const { return _kborder; }

		KM_PRO_SET(KP_NAME = "borderColor")
		void setBorderColor(const KColor &Color);

		KM_PRO_GET(KP_NAME = "borderColor", KP_TYPE = KColor, KP_CM = "border color")
		inline const KColor &getBorderColor() const { return _kbcolor; }

		KM_PRO_SET(KP_NAME = "width")
		void setWidth(F32 Width);

		KM_PRO_GET(KP_NAME = "width", KP_TYPE = F32, KP_CM = "width of the quad")
		inline F32 getWidth() const { return _kwidth; }

		KM_PRO_SET(KP_NAME = "height")
		void setHeight(F32 Height);

		KM_PRO_GET(KP_NAME = "height", KP_TYPE = F32, KP_CM = "height of the quad")
		inline F32 getHeight() const { return _kheight; }

		//void setUV(const KRectF32 &UV);
		//inline const KRectF32 &getUV() const { return _kuv; }

		KM_PRO_SET(KP_NAME = "tint")
		void setColor(const KColor &Color);

		KM_PRO_GET(KP_NAME = "tint", KP_TYPE = KColor, KP_CM = "tint color")
		inline const KColor &getTintColor() const { return _kcolor; }

		KM_PRO_GET(KP_NAME = "atlasID", KP_TYPE = U32, KP_CM = "atlas id")
		inline U32 getAtlasID() const { return _katlasid; }

		KM_PRO_SET(KP_NAME = "atlasID")
		inline void setAtlasID(U32 AtlasID) { _katlasid = AtlasID; }

	protected:
		/// render system interface
		inline const std::vector<KVertex> *getVertex() const override { return &_kvertex; }
		inline const U32 getIndexSize() const override { return _kindexsize; }
		inline const std::vector<KPointSprite> *getPoint() const override { return nullptr; }

		inline GLPrimitive getGeoType() const override { return GLPrimitive::TRIANGLES; }

		inline bool isIndexed() const override { return true; }
		inline bool isPoint() const override { return false; }
		inline bool isReverse() const override { return _kreverse; }

    private:
		void _setDim();
		KM_VAR() bool _kreverse;
		KM_VAR() U32 _kindexsize;
		KM_VAR() std::vector<KVertex> _kvertex;
		KM_VAR() U32 _katlasid;
		KM_VAR() F32 _kwidth;
		KM_VAR() F32 _kheight;
		KM_VAR() F32 _kborder;
		KM_VAR() KColor _kcolor;
		KM_VAR() KColor _kbcolor;
    };
}

#endif // KQUADCOM_H