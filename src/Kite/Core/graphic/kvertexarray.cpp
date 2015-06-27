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
#include "Kite/Core/graphic/kvertexarray.h"
#include "src/Kite/Core/graphic/glcall.h"

namespace Kite{
    U32 KVertexArray::_klastArrayId = 0;
    KVertexArray::KVertexArray():
		KCoreInstance(KCI_VAO),
        _karrayId(0)
    {
        DGL_CALL(glGenVertexArrays(1, &_karrayId));
    }

    void KVertexArray::setAttribute(U16 Index, KAttributeCountTypes Count,
                                KAttributeTypeTypes Type, bool Normalized,
                                I32 StrideSize, void * Offset){
        static GLenum type[] = {GL_BYTE,
                                GL_UNSIGNED_BYTE,
                                GL_SHORT,
                                GL_UNSIGNED_SHORT,
                                GL_FIXED,
                                GL_FLOAT};

        DGL_CALL(glVertexAttribPointer(Index, Count, type[Type], Normalized, StrideSize, (const GLvoid *)Offset));
    }

    void KVertexArray::enableAttribute(U16 Index){
        DGL_CALL(glEnableVertexAttribArray(Index));
    }

    void KVertexArray::disableAttribute(U16 Index){
        DGL_CALL(glDisableVertexAttribArray(Index));
    }

    void KVertexArray::AttributeDivisor(U16 Index, U32 Divisor){
        DGL_CALL(glVertexAttribDivisor(Index, Divisor));
    }

    KVertexArray::~KVertexArray(){
        DGL_CALL(glDeleteVertexArrays(1, &_karrayId));
        if (_klastArrayId == _karrayId){
            _klastArrayId = 0;
        }
    }

    void KVertexArray::bind() const{
        if(_klastArrayId != _karrayId){
            DGL_CALL(glBindVertexArray(_karrayId));
            _klastArrayId = _karrayId;
        }
    }

    void KVertexArray::unbind(){
        if (_klastArrayId == _karrayId){
            DGL_CALL(glBindVertexArray(0));
            _klastArrayId = 0;
        }
    }

    void KVertexArray::unbindVertexArray(){
        DGL_CALL(glBindVertexArray(0));
        _klastArrayId = 0;
    }

	U64 KVertexArray::getInstanceSize() const{
		return sizeof(KVertexArray);
	}
}

