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
#ifndef KARRAYBATCHOBJECT_H
#define KARRAYBATCHOBJECT_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/kshaderprogram.h"
#include "Kite/Core/graphic/ktexture.h"
#include "Kite/Core/math/ktransformable.h"
#include "kite/Assist/graphic/kanimeobject.h"
#include "kite/Assist/map/ktilemapobject.h"
#include <cstring>

namespace Kite{
	class KITE_FUNC_EXPORT KArrayBatchObject : public KTransformable, public KAnimeObject, public KTileMapObject {
		friend class KArrayBatch;
    public:
		KArrayBatchObject(U32 VertexSize, bool PointSprite, const std::string &Name = "");
		virtual ~KArrayBatchObject() {}

		inline const KVertex *getVertex() const { return _kvertex; }
		inline const KPointSprite *getPoint() const { return _kpsprite; }

		inline void setShader(const KShaderProgram *Shader) { _kshader = Shader; }
		inline const KShaderProgram *getShader() const { return _kshader; }

		inline void setTexture(const KTexture *Texture) { _ktexture = Texture; }
		inline const KTexture *getTexture() const { return _ktexture; }

		inline void setGeoType(KGeoPrimitiveTypes GeoType) { _kgtype = GeoType; }
		inline KGeoPrimitiveTypes getGeoType() const { return _kgtype; }

		inline void setVisible(bool Visible) { _kvisible = Visible; }
		inline bool getVisible() const { return _kvisible; }

		inline U32 getVertexSize() const { return _kusedSize; }
		inline U32 getPointSize() const { return _kusedSize; }

		inline U32 getRealVertexSize() const { return _krealSize; }
		inline U32 getRealPointSize() const { return _krealSize; }

		inline void setRelativeTransform(bool Relative) { _krelTrans = Relative; }
		inline bool getRelativeTransform() const { return _krelTrans; }

		inline bool isPointSpriteEnabled() const { return _kpenable; }

		// return model-view teansform
		virtual const KTransform &getModelViewTransform() const = 0;

	protected:
		void setUseSize(U32 Size);

		KVertex *_kvertex;
		KPointSprite *_kpsprite;

	private:
		const KShaderProgram *_kshader;
		const KTexture *_ktexture;
		KGeoPrimitiveTypes _kgtype;
		const U32 _krealSize;
		U32 _kusedSize;
		bool _kvisible;
		bool _kpenable;
		bool _krelTrans;
		std::string _kname;
    };
}

#endif // KARRAYBATCHOBJECT_H
