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

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/core/system/kresources.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/kgraphictypes.h"

namespace Kite{
    class KITE_FUNC_EXPORT KVertexBuffer : public KResources{
    public:
		KVertexBuffer(KBufferTargetTypes TargetType);
        ~KVertexBuffer();

        /// create vertex buffer
        /// DataSize: sizeof(DataType) * DataArrayLenght
        void fill(const void *Data, U32 DataSize, KVertexBufferTypes BufferType);

        /// update buffer.
        /// map entire set of data in memory
        /// and send its pointer to handle function.
        /// inefficeint if you have a large buffer
        /// and need to update a small portion of the values.
        void update(void *Sender);

        /// update buffer.
        /// map a section of data in memory
        /// and send its pointer to handle function.
        /// Offset = sizeof(DataType) * Offset
        /// Size = sizeof(DataType) * DataArrayLenght
        /// best methode for streaming buffer
        void update(U32 Offset, U32 Size, bool Discarded, void *Sender);

        /// update buffer.
        /// replace some subset of the data with new data.
        /// Offset = sizeof(DataType) * Offset
        /// Size = sizeof(DataType) * DataArrayLenght
        void update(U32 Offset, U32 Size, const void *Data);

        /// bind buffer.
        /// (handle autimatic by internal render system)
        void bind() const;

        /// unbind the vertex buffer if it is currently in use
        void unbind();

        /// unbind currently vertex buffer
        static void unbindVertexBuffer();

        /// set function handle for mapping buffer.
        inline void setUpdateHandle(KCallVBUpdate *UpdateHandle) {_kupdateHnd = UpdateHandle;}

        inline KVertexBufferTypes getType() const {return _kbufType;}
		inline U32 getID() const { return _kbufId; }

		U64 resourcesGetSize() const;

    private:
        KVertexBufferTypes _kbufType;
		KBufferTargetTypes _kbufTarget;
        U32 _ksize;
        U32 _kbufId;
        KCallVBUpdate *_kupdateHnd;
		static I32 _ktargets[2];
        static U32 _klastBufId;
    };
}

#endif // KVERTEXBUFFER_H
