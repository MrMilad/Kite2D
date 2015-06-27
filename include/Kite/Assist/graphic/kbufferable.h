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
        inline void setPreVBuffer(KVertexVector<T> *PreVertexBuffer) {_kpreVBuffer = PreVertexBuffer;}

        /// set orginal vertex buffer (GPU side buffer)
        /// use in draw operation
        inline void setVBuffer(const KVertexBuffer<T> *VertexBuffer) {_kvbuffer = VertexBuffer;}

        /// push the vertex to the pre vertex buffer (CPU side buffer)
        virtual void pushVertex() = 0;

    protected:
        KVertexVector *_kpreVBuffer;
        const KVertexBuffer *_kvbuffer;
    };

}

#endif // KBUFFERABLE_H
