/*    
    Kite2D Game Framework.
    Copyright (C) 2010-2013  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef KVERTEXBUFFER_H
#define KVERTEXBUFFER_H

#include "Kite/system/ksystemdef.h"
#include "Kite/system/knoncopyable.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/graphic/kgraphictypes.h"

namespace Kite{
    class KITE_FUNC_EXPORT KVertexBuffer : KNonCopyable{
    friend class KGL2DRender;
    public:
        KVertexBuffer();
        KVertexBuffer(KVertexBufferTypes BufferType);
        KVertexBuffer(KVertexBufferTypes BufferType, const KVertex *VertexArray, U32 ArrayLenght);
        ~KVertexBuffer();

        inline void setType(KVertexBufferTypes BufferType) {_kbufType = BufferType;}
        inline void setVertexArray(const KVertex *VertexArray, U32 ArrayLenght){_kvertexPointer = VertexArray;
                                                                                _karrLen = ArrayLenght;}

        inline KVertexBufferTypes getType() const {return _kbufType;}
        inline U32 getLength() const {return _karrLen;}
        inline U32 getID() const {return _kbufId;}

        // Bind Buffer
        void bind();

        // Fill Buffer
        void fillData();

        // Update Buffer
        KVertex *map(KMapAccessTypes AccessType);
        void unmap();

    private:
        KVertexBufferTypes _kbufType;
        const KVertex *_kvertexPointer;
        U32 _karrLen;
        U32 _kbufId;
        bool _kgenerated;
        bool _kfilled;
        static U32 _klastBufId;
    };
}

#endif // KVERTEXBUFFER_H
