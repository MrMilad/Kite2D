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
    {
        // generate buffer
        DGL_CALL(glGenBuffers(1, &_kbufId));
    }

    KVertexBuffer::~KVertexBuffer(){
        DGL_CALL(glDeleteBuffers(1, &_kbufId));
        if (_klastBufId == _kbufId){
            _klastBufId = 0;
        }
    }

    void KVertexBuffer::fill(const void *Data, U32 DataSize, KVertexBufferTypes BufferType){
        // check array pointer and size of array
        if (Data != 0 && DataSize > 0){

            // save currently binded buffer then bind our buffer temporary
            Internal::GLBindGuard bindGuard(Internal::KBG_VBUFFER, _klastBufId);
            DGL_CALL(glBindBuffer(GL_ARRAY_BUFFER_ARB, _kbufId));

            // fill buffer with vertex data
            int dataHint[] = {GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW};
            DGL_CALL(glBufferData(GL_ARRAY_BUFFER_ARB, DataSize,
                                  (const GLvoid *)Data, dataHint[BufferType]));

            _kbufType = BufferType;
            _karrLen = DataSize;
        }
    }

    void KVertexBuffer::update(void *Sender){
        // check update handle and buffer
        if (_kupdateHnd && _kbufId > 0){

            // save currently binded buffer then bind our buffer temporary
            Internal::GLBindGuard bindGuard(Internal::KBG_VBUFFER, _klastBufId);
            DGL_CALL(glBindBuffer(GL_ARRAY_BUFFER_ARB, _kbufId));

            // map buffer
            void *dataPtr = DGL_CALL(glMapBuffer(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB));

            // call update handle
            (*_kupdateHnd)(dataPtr, _karrLen, Sender);

            // unmap buffer
            DGL_CALL(glUnmapBuffer(GL_ARRAY_BUFFER_ARB));

        }else{
            KDEBUG_PRINT("buffer is not created or update handle not set yet.");
        }
    }

    void KVertexBuffer::update(Kite::I64 Offset, I64 Size, bool Discarded, void *Sender){
        // check update handle and buffer
        if (_kupdateHnd && _kbufId > 0){

            // save currently binded buffer then bind our buffer temporary
            Internal::GLBindGuard bindGuard(Internal::KBG_VBUFFER, _klastBufId);
            DGL_CALL(glBindBuffer(GL_ARRAY_BUFFER_ARB, _kbufId));

            // discarded (streaming purpose only)
            GLbitfield acc;
            if (Discarded){
                acc =   GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT |
                        GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_RANGE_BIT;
            }else{
                acc =   GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT;
            }

            // map a section of buffer
            void *dataPtr = glMapBufferRange(GL_ARRAY_BUFFER_ARB, (GLintptr) Offset, (GLsizeiptr)Size, acc);

            // call update handle
            (*_kupdateHnd)(dataPtr, Size, Sender);

            // unmap buffer
			DGL_CALL(glFlushMappedBufferRange(GL_ARRAY_BUFFER_ARB, (GLintptr)Offset, (GLsizeiptr) Size));

        }else{
            KDEBUG_PRINT("buffer is not created or update handle not set yet.");
        }
    }

    void KVertexBuffer::update(U64 Offset, U64 Size, const void *Data){
        if (_kbufId > 0){

            // save currently binded buffer then bind our buffer temporary
            Internal::GLBindGuard bindGuard(Internal::KBG_VBUFFER, _klastBufId);
            DGL_CALL(glBindBuffer(GL_ARRAY_BUFFER_ARB, _kbufId));

            // replace data
            DGL_CALL(glBufferSubData(GL_ARRAY_BUFFER_ARB, (GLintptr)Offset,
                                     (GLsizeiptr)Size, Data));
        }
    }

    void KVertexBuffer::bind() const{
        // bind buffer
        DGL_CALL(glBindBuffer(GL_ARRAY_BUFFER_ARB, _kbufId));
        _klastBufId = _kbufId;
    }

    void KVertexBuffer::unbind(){
        DGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
        _klastBufId = 0;
    }

}
