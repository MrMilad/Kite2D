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
#include "Kite/Assist/graphic/ktransformable.h"
#include "Kite/Core/math/kvector2.h"
#include <vector>

namespace Kite{
    class KITE_FUNC_EXPORT KQuad : public KTransformable{
    public:
		KQuad() :
			_kquadattr()
		{}

		KQuad(const KQuadAttrib &QuadAttribute) :
			_kquadattr(QuadAttribute)
		{}

        inline void setQuadAttribute(const KQuadAttrib &Attribute) {_kquadattr = Attribute;}
        inline const KQuadAttrib *getQuadAttribute() const {return &_kquadattr;}
    private:
        KQuadAttrib _kquadattr;
    };
}

#endif // KQUAD_H
