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
#ifndef KVERTEXARRAY_H
#define KVERTEXARRAY_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/knoncopyable.h"
#include "Kite/Core/graphic/kgraphictypes.h"

namespace Kite{
    class KITE_FUNC_EXPORT KVertexArray : KNonCopyable{
    public:
        KVertexArray();
        ~KVertexArray();

        void setAttribute(U16 Index, KAttributeCountTypes Count,
                          KAttributeTypeTypes Type, bool Normalized,
                          I32 StrideSize, void *Offset);

        /// enables the generic vertex attribute
        /// array specified by index.
        /// bind array before calling this function.
        void enableAttribute(U16 Index);

        /// enables the generic vertex attribute
        /// array specified by index.
        /// bind array before calling this function.
        void disableAttribute(U16 Index);

        /// modifies the rate at which generic vertex attributes advance
        /// when rendering multiple instances of primitives in a single draw call
        void AttributeDivisor(U16 Index, U32 Divisor);

        /// bind vertex array.
        /// (handle autimatic by internal render system)
        void bind() const;

        /// unbind the vertex array if it is currently in use.
        void unbind();

        /// unbind currently vertex array.
        static void unbindVertexArray();

        inline U32 getID() const {return _karrayId;}

    private:
        U32 _karrayId;
        static U32 _klastArrayId;
    };
}

#endif // KVERTEXARRAY_H
