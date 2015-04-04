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
#ifndef KQUAD_H
#define KQUAD_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/math/kmathstructs.h"
#include "Kite/Assist/graphic/ktransformable.h"
#include "kite/Assist/graphic/kindexbatchobject.h"
#include "kite/Assist/graphic/kanimecontroller.h"

namespace Kite{
    class KITE_FUNC_EXPORT KQuad : public KIndexBatchObject, public KTransformable{
    public:
		KQuad();
		KQuad(const KRectF32 &Dimension, const KRectF32 &UV, const KColor &Color);
		KQuad(const KRect2F32 &Dimension, const KRectF32 &UV, const KColor &Color);

		void setDimension(const KRect2F32 &Dimension);
		void setDimension(const KRectF32 &Dimension);
		inline const KRect2F32 &getDimension() const { return _kdim; }

		void setUV(const KRectF32 &UV);
		inline const KRectF32 &getUV() const { return _kuv; }

		void setColor(const KColor &Color);
		inline const KColor &getColor() const { return _kcolor; }

	protected:
		// model-view matrix callback
		const KTransform &getModelViewTransform() const;

    private:
		void _fillIndex();
		KRect2F32 _kdim;
		KRectF32 _kuv;
		KColor _kcolor;
    };
}

#endif // KQUAD_H
