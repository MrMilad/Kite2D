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
        ~KVertexBuffer();

        // create buffer
        void create(const KVertex *VertexArray, U32 ArrayLenght, KVertexBufferTypes BufferType);

        // update buffer
        void update(KMapAccessTypes AccessType);

        // bind buffer
        // (handle autimatic by internal render system)
        void bind();

        // set update handle for updating buffer
        inline void setUpdateHandle(KCallVBUpdate *UpdateHandle) {_kupdateHnd = UpdateHandle;}

        inline KVertexBufferTypes getType() const {return _kbufType;}
        inline U32 getLength() const {return _karrLen;}
        inline U32 getID() const {return _kbufId;}

    private:
        KVertexBufferTypes _kbufType;
        U32 _karrLen;
        U32 _kbufId;
        KCallVBUpdate *_kupdateHnd;
        static U32 _klastBufId;
    };
}

#endif // KVERTEXBUFFER_H
