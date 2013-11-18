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
#include "Kite/graphic/kvertexbuffer.h"
#include "Kite/graphic/glcall.h"

namespace Kite{

    U32 KVertexBuffer::_klastBufId = 0;

    KVertexBuffer::KVertexBuffer():
        _kbufType(KVERTEX_BUFFER_DYNAMIC),
        _kvertexPointer(0),
        _karrLen(0),
        _kbufId(0),
        _kgenerated(false),
        _kfilled(false)
    {}

    KVertexBuffer::KVertexBuffer(KVertexBufferTypes BufferType):
        _kbufType(BufferType),
        _kvertexPointer(0),
        _karrLen(0),
        _kbufId(0),
        _kgenerated(false),
        _kfilled(false)
    {}

    KVertexBuffer::KVertexBuffer(KVertexBufferTypes BufferType, const KVertex *VertexArray, U32 ArrayLenght):
        _kbufType(BufferType),
        _kvertexPointer(VertexArray),
        _karrLen(ArrayLenght),
        _kbufId(0),
        _kgenerated(false),
        _kfilled(false)
    {}

    KVertexBuffer::~KVertexBuffer(){
        if (_kbufId != 0){
            DGL_CALL(glDeleteBuffers(1, &_kbufId));
            if (_klastBufId == _kbufId){
                _klastBufId = 0;
            }
        }
    }

    void KVertexBuffer::bind(){
        // first, we must generate a buffer.
        if (!_kgenerated){
            DGL_CALL(glGenBuffers(1, &_kbufId));
            _kgenerated = true;
        }

        // bind buffer
        DGL_CALL(glBindBuffer(GL_ARRAY_BUFFER_ARB, _kbufId));
        _klastBufId = _kbufId;
    }

    void KVertexBuffer::fillData(){
        if (_kfilled){
            KDEBUG_PRINT("filled buffer.");
            return;
        }

        // before filling buffer we check our data
        if (_kvertexPointer == 0 || _karrLen <= 0){
            KDEBUG_PRINT("empty array.");
            return;
        }

        int dataHint[] = {GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW};

        // bind and fill buffer with our vertex data
        bind();
        DGL_CALL(glBufferData(GL_ARRAY_BUFFER_ARB, sizeof(KVertex)*_karrLen,
                              (const void *) &_kvertexPointer[0].x, dataHint[_kbufType]));
        _kfilled = true;
    }

    KVertex *KVertexBuffer::map(KMapAccessTypes AccessType){
        if (!_kfilled){
            KDEBUG_PRINT("try to map empty buffer.");
            return 0;
        }
        int accType[] = {GL_READ_ONLY_ARB, GL_WRITE_ONLY_ARB, GL_READ_WRITE_ARB};
        this->bind();
        DGL_CALL(return (KVertex *)glMapBuffer(GL_ARRAY_BUFFER_ARB, accType[AccessType]));
    }

    void KVertexBuffer::unmap(){
        if (_kfilled && _klastBufId == _kbufId){
            DGL_CALL(glUnmapBuffer(GL_ARRAY_BUFFER_ARB));
        }
    }

}
