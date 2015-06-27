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
#include "Kite/Core/graphic/kshaderprogram.h"
#include "Kite/Core/graphic/ktexture.h"
#include "Kite/Assist/graphic/ktransformable.h"

namespace Kite{
    class KITE_FUNC_EXPORT KBatchObject{
		friend  class KBatch;
    public:
		KBatchObject(U32 VertexSize);
		~KBatchObject();

		inline const KVertex *getVertex() const { return _kvertex; }

		inline void setShader(const KShaderProgram *Shader) { _kshader = Shader; }
		inline const KShaderProgram *getShader() const { return _kshader; }

		inline void setTexture(const KTexture *Texture) { _ktexture = Texture; }
		inline const KTexture *getTexture() const { return _ktexture; }

		inline void setGeoType(KGeoPrimitiveTypes GeoType) { _kgtype = GeoType; }
		inline KGeoPrimitiveTypes getGeoType() const { return _kgtype; }

		inline U32 getVertexSize() const { return _kvsize; }

	protected:
		KVertex *_kvertex;

		// return model-view teansform
		virtual const KTransform &getModelViewTransform() const = 0;

	private:
		const U32 _kvsize;
		const KShaderProgram *_kshader;
		const KTexture *_ktexture;
		KGeoPrimitiveTypes _kgtype;
    };
}

#endif // KBATCHOBJECT_H
