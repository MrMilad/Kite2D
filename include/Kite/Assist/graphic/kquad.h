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
#include "Kite/Core/math/kvector2.h"
#include "kite/Assist/graphic/kindexbatchobject.h"
#include "kite/Assist/graphic/kanime.h"

namespace Kite{
    class KITE_FUNC_EXPORT KQuad : public KIndexBatchObject, public KAnime{
    public:
		KQuad();
		KQuad(const KRectF32 &Dimension, const KRectF32 &UV, const KColor &Color);

		void setDimension(const KRectF32 &Dimension);
		inline const KRectF32 &getDimension() const { return _kdim; }

		void setUV(const KRectF32 &UV);
		inline const KRectF32 &getUV() const { return _kuv; }

		void setColor(const KColor &Color);
		inline const KColor &getColor() const { return _kcolor; }

	protected:
		void _getJointsState(const KAnimeState *States, U32 Count, U16 Frame);

    private:
		void _fillIndex();
		KRectF32 _kdim;
		KRectF32 _kuv;
		KColor _kcolor;
    };
}

#endif // KQUAD_H
