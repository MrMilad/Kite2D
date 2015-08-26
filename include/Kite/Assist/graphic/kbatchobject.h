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
#ifndef KBATCHOBJECT_H
#define KBATCHOBJECT_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Core/graphic/ktexture.h"
#include "Kite/Core/graphic/kshaderprogram.h"
#include "Kite/core/math/ktransformable.h"
#include "kite/Assist/graphic/kanimeobject.h"
#include "kite/Assist/map/ktilemapobject.h"
#include <cstring>

namespace Kite{
	class KITE_FUNC_EXPORT KBatchObject : public KTransformable, public KAnimeObject, public KTileMapObject {
		friend class KBatch;
    public:
		// array rendering (without index)
		KBatchObject(U32 VertexSize, const std::string &Name, bool PointSprite);
		// index rendering
		KBatchObject(U32 VertexSize, U32 IndexSize, const std::string &Name);
		virtual ~KBatchObject() {}

		inline const KVertex *getVertex() const { return _kvertex; }
        inline const U16 *getIndex() const {return _kindex;}
		inline const KPointSprite *getPoint() const { return _kpsprite; }

		inline const std::string &getName() const { return _kname; }
		inline void setName(const std::string &Name) { _kname = Name; }

		inline void setShader(const KShaderProgram *Shader) { _kshader = Shader; }
		inline const KShaderProgram *getShader() const { return _kshader; }

		inline void setTexture(const KTexture *Texture) { _ktexture = Texture; }
		inline const KTexture *getTexture() const { return _ktexture; }

		inline void setGeoType(KGeoPrimitiveTypes GeoType) { _kgtype = GeoType; }
		inline KGeoPrimitiveTypes getGeoType() const { return _kgtype; }

		inline void setVisible(bool Visible) { _kvisible = Visible; }
		inline bool getVisible() const { return _kvisible; }

        inline U32 getVertexSize() const { return _kusedVSize; }
		inline U32 getIndexSize() const { return _kusedISize; }
		inline U32 getPointSize() const { if (_kpoint) return _kusedVSize; return 0; }

		inline U32 getRealVertexSize() const { return _krealVSize; }
		inline U32 getRealIndexSize() const { return _krealISize; }
		inline U32 getRealPointSize() const { return _krealVSize; }

		inline void setRelativeTransform(bool Relative) { _krelTrans = Relative; }
		inline bool getRelativeTransform() const { return _krelTrans; }

		inline bool getIndexed() const { return _kindexed; }
		inline bool getSpriteEnabled() const { return _kpoint; }

		inline void setReverseRender(bool Reverse) { _kreverse = Reverse; }
		inline bool getReverseRender() const { return _kreverse; }

		// return model-view transform
		virtual const KTransform &getModelViewTransform() const = 0;

    protected:
		// vertex/point catch size
		void setUseVertexSize(U32 Size);
		// index catch size
		void setUseIndexSize(U32 Size);

        KVertex *_kvertex;
		U16 *_kindex;
		KPointSprite *_kpsprite;

    private:
		const KShaderProgram *_kshader;
		const KTexture *_ktexture;
		KGeoPrimitiveTypes _kgtype;
		const U32 _krealVSize;
		const U32 _krealISize;
		U32 _kusedVSize;
		U32 _kusedISize;
		bool _kvisible;
		bool _krelTrans;
		std::string _kname;
		bool _kindexed;
		bool _kpoint;
		bool _kreverse;
    };
}

#endif // KBATCHOBJECT_H
