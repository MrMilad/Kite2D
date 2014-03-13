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
#include "Kite/core/graphic/kvertexbuffer.h"
#include "src/Kite/core/graphic/glcall.h"

namespace Kite{

    U32 KVertexBuffer::_klastBufId = 0;
    KVertexBuffer::KVertexBuffer():
        _kbufType(KVB_STATIC),
        _karrLen(0),
        _kbufId(0),
        _kupdateHnd(0)
    {}

    KVertexBuffer::~KVertexBuffer(){
        if (_kbufId > 0){
            DGL_CALL(glDeleteBuffers(1, &_kbufId));
            if (_klastBufId == _kbufId){
                _klastBufId = 0;
            }
        }
    }

    void KVertexBuffer::create(const KVertex *VertexArray, U32 ArrayLenght, KVertexBufferTypes BufferType){
        // generate buffer
        if (_kbufId <= 0)
            DGL_CALL(glGenBuffers(1, &_kbufId));

        // check array pointer and size of array
        if (VertexArray != 0 && ArrayLenght > 0){

            // save currently binded buffer then bind our buffer temporary
            Internal::GLBindGuard bindGuard(Internal::KBG_BUFFER, _klastBufId);
            DGL_CALL(glBindBuffer(GL_ARRAY_BUFFER_ARB, _kbufId));

            // fill buffer with vertex data
            int dataHint[] = {GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW};
            DGL_CALL(glBufferData(GL_ARRAY_BUFFER_ARB, sizeof(KVertex)*ArrayLenght,
                                  (const void *) &VertexArray[0].x, dataHint[BufferType]));

            _kbufType = BufferType;
            _karrLen = ArrayLenght;
        }
    }

    void KVertexBuffer::update(KMapAccessTypes AccessType, void *Sender){
        // check update handle and buffer
        if (_kupdateHnd && _kbufId > 0){

            // save currently binded buffer then bind our buffer temporary
            Internal::GLBindGuard bindGuard(Internal::KBG_BUFFER, _klastBufId);
            DGL_CALL(glBindBuffer(GL_ARRAY_BUFFER_ARB, _kbufId));

            // map buffer
            int accType[] = {GL_READ_ONLY_ARB, GL_WRITE_ONLY_ARB, GL_READ_WRITE_ARB};
            void *dataPtr = DGL_CALL(glMapBuffer(GL_ARRAY_BUFFER_ARB, accType[AccessType]));

            // call update handle
            (*_kupdateHnd)((KVertex *)dataPtr, _karrLen, Sender);

            // unmap buffer
            DGL_CALL(glUnmapBuffer(GL_ARRAY_BUFFER_ARB));

        }else{
            KDEBUG_PRINT("buffer is not created or update handle not set yet.");
        }
    }

    void KVertexBuffer::update(U64 Offset, U64 Size, const KVertex *Data){
        if (_kbufId > 0){

            // save currently binded buffer then bind our buffer temporary
            Internal::GLBindGuard bindGuard(Internal::KBG_BUFFER, _klastBufId);
            DGL_CALL(glBindBuffer(GL_ARRAY_BUFFER_ARB, _kbufId));

            // replace data
            DGL_CALL(glBufferSubData(GL_ARRAY_BUFFER_ARB, (GLintptr)(sizeof(KVertex)*Offset),
                                     (GLsizeiptr)(sizeof(KVertex)*Size), (void *)Data));
        }
    }

    void KVertexBuffer::bind() const{
        // bind buffer
        if (_kbufId != 0){
            DGL_CALL(glBindBuffer(GL_ARRAY_BUFFER_ARB, _kbufId));
            _klastBufId = _kbufId;
        }
    }

}
