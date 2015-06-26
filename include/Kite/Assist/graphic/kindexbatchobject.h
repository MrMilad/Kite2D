/*
 Kite2D Game Framework.
 Copyright (C) 2010-2014  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef KINDEXBATCHOBJECT_H
#define KINDEXBATCHOBJECT_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/ktexture.h"
#include "Kite/Core/graphic/kshaderprogram.h"
#include "Kite/core/math/ktransform.h"

namespace Kite{
    class KITE_FUNC_EXPORT KIndexBatchObject{
		friend class KIndexBatch;
    public:
		KIndexBatchObject(U32 VertexSize, U32 IndexSize);
		~KIndexBatchObject();

		inline const KVertex *getVertex() const { return _kvertex; }
        inline const U16 *getIndex() const {return _kindex;}

		inline void setShader(const KShaderProgram *Shader) { _kshader = Shader; }
		inline const KShaderProgram *getShader() const { return _kshader; }

		inline void setTexture(const KTexture *Texture) { _ktexture = Texture; }
		inline const KTexture *getTexture() const { return _ktexture; }

		inline void setGeoType(KGeoPrimitiveTypes GeoType) { _kgtype = GeoType; }
		inline KGeoPrimitiveTypes getGeoType() const { return _kgtype; }

        inline U32 getVertexSize() const { return _kvsize; }
        inline U32 getIndexSize() const { return _kisize; }

    protected:
		/// resize only before batching (not safe)
		//inline void resizeVertex(U32 VertexSize){ delete[] _kvertex, _kvertex = new KVertex[VertexSize]; _kvsize = VertexSize; }
		//inline void resizeIndex(U32 IndexSize){ delete[] _kindex, _kindex = new U16[IndexSize]; _kisize = IndexSize; }
        KVertex *_kvertex;
        U16 *_kindex;

		// return model-view teansform
		virtual const KTransform &getModelViewTransform() const = 0;

    private:
        const U32 _kvsize;
        const U32 _kisize;
		const KShaderProgram *_kshader;
		const KTexture *_ktexture;
		KGeoPrimitiveTypes _kgtype;
    };
}

#endif // KINDEXBATCHOBJECT_H
