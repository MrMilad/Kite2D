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
	I32 KVertexBuffer::_ktargets[] = { GL_ELEMENT_ARRAY_BUFFER, GL_ARRAY_BUFFER };
    U32 KVertexBuffer::_klastBufId = 0;
	KVertexBuffer::KVertexBuffer(KBufferTargetTypes TargetType) :
        _kbufType(KVB_STATIC),
		_kbufTarget(TargetType),
        _ksize(0),
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
			static int dataHint[] = { GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW };

			// bind buffer
			bind();

            // fill buffer with vertex data
			DGL_CALL(glBufferData(_ktargets[_kbufTarget], DataSize,
                                  (const GLvoid *)Data, dataHint[BufferType]));

            _kbufType = BufferType;
            _ksize = DataSize;
        }
    }

    void KVertexBuffer::update(void *Sender){
        // check update handle and buffer
        if (_kupdateHnd && _kbufId > 0){

            // bind buffer
			bind();

            // map buffer
			void *dataPtr = DGL_CALL(glMapBuffer(_ktargets[_kbufTarget], GL_WRITE_ONLY_ARB));

            // call update handle
            (*_kupdateHnd)(dataPtr, _ksize, Sender);

            // unmap buffer
			DGL_CALL(glUnmapBuffer(_ktargets[_kbufTarget]));

        }else{
            KDEBUG_PRINT("buffer is not created or update handle not set yet.");
        }
    }

    void KVertexBuffer::update(Kite::I64 Offset, I64 Size, bool Discarded, void *Sender){
        // check update handle and buffer
        if (_kupdateHnd && _kbufId > 0 ){

			// bind buffer
			bind();

            // discarded (streaming purpose only)
            GLbitfield acc;
            if (Discarded){
                acc =   GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT |
                        GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_RANGE_BIT;
            }else{
                acc =   GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT;
            }

            // map a section of buffer
			void *dataPtr = glMapBufferRange(_ktargets[_kbufTarget], (GLintptr)Offset, (GLsizeiptr)Size, acc);

            // call update handle
            (*_kupdateHnd)(dataPtr, Size, Sender);

            // unmap buffer
			DGL_CALL(glFlushMappedBufferRange(_ktargets[_kbufTarget], (GLintptr)Offset, (GLsizeiptr)Size));

			// unmap buffer
			DGL_CALL(glUnmapBuffer(_ktargets[_kbufTarget]));

        }else{
            KDEBUG_PRINT("buffer is not created or update handle not set yet.");
        }
    }

    void KVertexBuffer::update(U64 Offset, U64 Size, const void *Data){
        if (_kbufId > 0){

			// bind buffer
			bind();

            // replace data
			DGL_CALL(glBufferSubData(_ktargets[_kbufTarget], (GLintptr)Offset,
                                     (GLsizeiptr)Size, Data));
        }
    }

    void KVertexBuffer::bind() const{
        // bind buffer
        if (_klastBufId != _kbufId){
			DGL_CALL(glBindBuffer(_ktargets[_kbufTarget], _kbufId));
            _klastBufId = _kbufId;
        }
    }

    void KVertexBuffer::unbind(){
        if (_klastBufId == _kbufId){
			DGL_CALL(glBindBuffer(_ktargets[_kbufTarget], 0));
            _klastBufId = 0;
        }
    }

    void KVertexBuffer::unbindVertexBuffer(){
        DGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
		DGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        _klastBufId = 0;
    }

}
