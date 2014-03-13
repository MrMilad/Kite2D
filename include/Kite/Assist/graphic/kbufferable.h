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
#ifndef KBUFFERABLE_H
#define KBUFFERABLE_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kvertexbuffer.h"
#include "Kite/Assist/graphic/kvertexvector.h"

namespace Kite{
    class KITE_FUNC_EXPORT KBufferable{
    public:
        KBufferable():
            _kpreVBuffer(0),
            _kvbuffer(0)
        {}
        virtual ~KBufferable(){}

        /// set pre vertex buffer (CPU side buffer)
        /// use pre buffer for edit and update vertex data
        inline void setPreVBuffer(KVertexVector *PreVertexBuffer) {_kpreVBuffer = PreVertexBuffer;}

        /// set orginal vertex buffer (GPU side buffer)
        /// use in draw operation
        inline void setVBuffer(const KVertexBuffer *VertexBuffer) {_kvbuffer = VertexBuffer;}

        /// push the vertex to the pre vertex buffer (CPU side buffer)
        virtual void pushVertex() = 0;

    protected:
        KVertexVector *_kpreVBuffer;
        const KVertexBuffer *_kvbuffer;
    };

}

#endif // KBUFFERABLE_H
