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
#ifndef KINDEXBATCHOBJECT_H
#define KINDEXBATCHOBJECT_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Core/graphic/ktexture.h"
#include "Kite/Core/graphic/kshaderprogram.h"
#include "Kite/core/math/ktransform.h"

namespace Kite{
    class KITE_FUNC_EXPORT KIndexBatchObject{
		friend class KIndexBatch;
    public:
		KIndexBatchObject(U32 VertexSize, U32 IndexSize);
		virtual ~KIndexBatchObject() {}

		inline const KVertex *getVertex() const { return _kvertex; }
        inline const U16 *getIndex() const {return _kindex;}

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

		inline U32 getRealVertexSize() const { return _krealVSize; }
		inline U32 getRealIndexSize() const { return _krealISize; }

		inline void setRelativeTransform(bool Relative) { _krelTrans = Relative; }
		inline bool getRelativeTransform() const { return _krelTrans; }

		// return model-view teansform
		virtual const KTransform &getModelViewTransform() const = 0;

    protected:
		void setUseVertexSize(U32 Size);
		void setUseIndexSize(U32 Size);
        KVertex *_kvertex;
		U16 *_kindex;

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
    };
}

#endif // KINDEXBATCHOBJECT_H
