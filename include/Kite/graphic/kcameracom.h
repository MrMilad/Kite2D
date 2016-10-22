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
#ifndef KCAMERA_H
#define KCAMERA_H

#include "Kite/core/kcoredef.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/core/kcomponent.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/math/ktransform.h"
#include "Kite/graphic/kgraphicstructs.h"
#include <vector>
#include <bitset>
#include "kcameracom.khgen.h"

KMETA
namespace Kite{

	//class KTexture;
	class KAtlasTextureArray;

	KM_CLASS(COMPONENT)
	class KITE_FUNC_EXPORT KCameraCom : public KComponent{
		friend class KRenderSys;
		friend class KGCullingSys;
		KM_INFO(KI_NAME = "Camera");
		KMETA_KCAMERACOM_BODY();
    public:
        KCameraCom(const std::string &Name = "");

		void attached(KEntity *Entity) override;

		void deattached(KEntity *Entity) override;

		RecieveTypes onMessage(KMessage *Message, MessageScope Scope) override;

		bool updateRes() override;

		/// default: 0.0, 0.0
		KM_PRO_SET(KP_NAME = "viewSize")
		void setSize(const KVector2U32 &Size); 

		KM_PRO_GET(KP_NAME = "viewSize", KP_TYPE = KVector2U32, KP_CM = "size of the viewport")
		inline KVector2U32 getSize() const { return _ksize; }

		KM_PRO_SET(KP_NAME = "center")
		void setCenter(const KVector2F32 &Center);

		KM_PRO_GET(KP_NAME = "center", KP_TYPE = KVector2F32, KP_CM = "center of the viewport")
		inline KVector2F32 getCenter() const { return _kcenter + _kmoveDelta; }

		/// default: 0.0, 0.0
		KM_PRO_SET(KP_NAME = "viewPosition")
		void setPosition(const KVector2I32 &Position);

		KM_PRO_GET(KP_NAME = "viewPosition", KP_TYPE = KVector2I32, KP_CM = "position of the viewport")
		inline KVector2I32 getPosition() const { return _kposition; }

		/// normal move
		KM_FUN()
		void move(const KVector2F32 &Move); // { _kcenter += Move;  _kneedUpdate = true; }

		/// parallax move
		KM_FUN()
		void parallaxMove(const KVector2F32 &Move);

		/// default: 0.0
		KM_PRO_SET(KP_NAME = "rotation")
		void setRotation(F32 Angle); // { _krotation = Angle; _kneedUpdate = true; }

		KM_PRO_GET(KP_NAME = "rotation", KP_TYPE = F32, KP_CM = "rotation of the camera")
		inline F32 getRotation() const { return _krotation + _krotateDelta; }

		///normal rotate
		KM_FUN()
		void rotate(F32 Angle); // { _krotation += Angle;  _kneedUpdate = true; }

		/// parallax rotate
		KM_FUN()
		void parallaxRotate(F32 Angle);

		/// set zoom factor
		/// default: 1.0
		KM_PRO_SET(KP_NAME = "zoom")
		void setZoom(F32 Factor); // { _kzoom = Factor; _kneedUpdate = true; }

		KM_PRO_GET(KP_NAME = "zoom", KP_TYPE = F32, KP_CM = "zoom in/out factor")
		inline F32 getZoom() const { return _kzoomFactor + _kzoomDelta; }

		/// normal zoom
		KM_FUN()
		void zoom(F32 Value);

		/// paralax zoom
		KM_FUN()
		void parallaxZoom(F32 Value);

		KM_PRO_GET(KP_NAME = "depth", KP_TYPE = U32, KP_CM = "camera render order")
		inline U32 getDepth() const { return _kdepth; }

		KM_PRO_SET(KP_NAME = "depth")
		inline void setDepth(U32 Depth) { _kdepth = Depth; }

		KM_PRO_SET(KP_NAME = "clearViewport")
		inline void setClearViewport(bool ClearViewport) { _kclearView = ClearViewport; }

		KM_PRO_GET(KP_NAME = "clearViewport", KP_TYPE = bool, KP_CM = "clear viewport with clear color")
		inline bool getClearViewport() const { return _kclearView; }

		KM_PRO_SET(KP_NAME = "clearColor")
		inline void setClearColor(const KColor &Color) { _kclearCol = Color; }

		KM_PRO_GET(KP_NAME = "clearColor", KP_TYPE = KColor, KP_CM = "background clear color")
		inline const KColor &getClearColor() const { return _kclearCol; }

		KM_PRO_SET(KP_NAME = "renderTexture")
		void setRenderTexture(const KStringID &Texture);

		KM_PRO_GET(KP_NAME = "renderTexture", KP_TYPE = KStringID, KP_CM = "render to texture", KP_RES = RTypes::TextureGroup)
		inline const KStringID &getRenderTexture() const { return _krtextureName; }

		KM_PRO_SET(KP_NAME = "textureIndex")
		inline void setRenderTextureIndex(U32 Index) { _krtextureIndex = Index; }

		KM_PRO_GET(KP_NAME = "textureIndex", KP_TYPE = U32, KP_CM = "index of the render texture in group")
		U32 getRenderTextureIndex() const { return _krtextureIndex; }

		/// flip screen coordinate
		/// (Micro$oft window system)
		KM_PRO_SET(KP_NAME = "flipY")
		void setFlipY(bool Flip); //{ _kflipy = _kflipy * -1; _kneedUpdate = true; } 

		KM_PRO_GET(KP_NAME = "flipY", KP_TYPE = bool, KP_CM = "flip screen coordinate")
		inline bool getFlipY() const { return _kflipy; }

		KM_PRO_SET(KP_NAME = "flipX")
		void setFlipX(bool Flip); // { _kflipx = _kflipx * -1; _kneedUpdate = true; }

		KM_PRO_GET(KP_NAME = "flipX", KP_TYPE = bool, KP_CM = "flip screen coordinate")
		inline bool getFlipX() const { return _kflipx; }

		/// enable/disable a layer
		KM_FUN()
		void setLayer(U32 Index, bool Enable);

		/// check state of a layer (is enabled/disabled)
		KM_FUN()
		bool checkLayer(U32 Index);

		/// enable/disable all layers
		KM_FUN()
		void setLayers(bool Enable);

		/// for culling purpose
		KM_FUN()
		KRectF32 getViewRect(I32 ParallaxIndex) const;
        
		/// return the combined matrix
		/// return normal matrix if Index was out of range or negative
		KM_FUN()
		const KMatrix3 &getRatioMatrix(I32 Index) const;

		/// return index
		/// duplicate will not be added and return index of same ratio in the list
		KM_FUN()
		I32 addParallaxRatio(const KParallaxRatio &Ratio);

		/// return true on successful
		/// duplicate is not allowd and false will returned
		/// index must be always positive
		KM_FUN()
		bool setParallaxRatio(I32 Index, const KParallaxRatio &Ratio);

		/// return default ratio if index was out of range or negative
		KM_FUN()
		KParallaxRatio getParallaxRatio(I32 Index) const;

		/// delete all items
		KM_FUN()
		void clearRatio();

		KM_FUN()
		void computeMatrixes();

    private:
		void computeMatrix(KMatrix3 &Mat, const KParallaxRatio &Rat);
		KM_VAR() bool _kclearView;
		KM_VAR() bool _kflipy;
		KM_VAR() bool _kflipx;
		KM_VAR() bool _kparaReset;
		KM_VAR() U32 _kdepth;
		KM_VAR() U32 _krtextureIndex;
		KM_VAR() F32 _krotation;
		KM_VAR() F32 _krotateDelta;
		KM_VAR() F32 _kzoomFactor;
		KM_VAR() F32 _kzoomDelta;
		KM_VAR() KVector2U32 _ksize;
		KM_VAR() KVector2F32 _kcenter;
		KM_VAR() KVector2F32 _kmoveDelta;
		KM_VAR() KVector2I32 _kposition;
		KM_VAR() KColor _kclearCol;
		KM_VAR() KStringID _krtextureName;
		KM_VAR() KMatrix3 _kdefMat;
		KM_VAR() std::vector<std::pair<KMatrix3, KParallaxRatio>> _kmatList;
		KM_VAR() std::bitset<KENTITY_LAYER_SIZE> _klayers;

		KAtlasTextureArray *_krtexture;
		bool _kcompute;
    };
}
#endif // KCAMERA_H
