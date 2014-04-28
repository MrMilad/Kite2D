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
#include "Kite/core/graphic/krender.h"
#include "src/Kite/core/graphic/glcall.h"

namespace Kite{

    // gl geometric types
    const U32 KRender::geoTypes[] = {GL_POINTS, GL_LINES,
                                    GL_LINE_STRIP, GL_LINE_LOOP,
                                    GL_TRIANGLES, GL_TRIANGLE_STRIP,
                                    GL_TRIANGLE_FAN, GL_LINE_STRIP_ADJACENCY,
                                    GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP_ADJACENCY,
                                    GL_TRIANGLES_ADJACENCY};

    KRender::KRender(const Kite::KRectI32 &Viewport):
        _kviewport(Viewport),
        _kgeoType(KGP_TRIANGLES)
    {
        Internal::initeGLEW();

        DGL_CALL(glDisable(GL_DEPTH_TEST));

        // enable blend
        DGL_CALL(glEnable(GL_BLEND));
        DGL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // initialize viewport
        setViewport(_kviewport);
    }

    KRender::~KRender(){}

    void KRender::clear(){
        DGL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    }


    void KRender::draw(U32 FirstIndex, U32 Size, KGeoPrimitiveTypes Primitive){
        // draw buffer
        DGL_CALL(glDrawArrays(geoTypes[Primitive], FirstIndex, Size));
    }

    void KRender::draw(U32 FirstIndex, U32 Size, KGeoPrimitiveTypes Primitive, U32 InstanceCount){
        // draw buffer
        DGL_CALL(glDrawArraysInstanced(geoTypes[Primitive], FirstIndex, Size, InstanceCount));
    }

    void KRender::draw(U32 Count, const U16 *Indices, KGeoPrimitiveTypes Primitive){
        // draw buffer
		DGL_CALL(glDrawElements(geoTypes[Primitive], Count, GL_UNSIGNED_SHORT, Indices));
    }

    void KRender::draw(U32 Count, const U16 *Indices, KGeoPrimitiveTypes Primitive, U32 InstanceCount){
        // draw buffer
        DGL_CALL(glDrawElementsInstanced(geoTypes[Primitive], Count, GL_UNSIGNED_SHORT, Indices, InstanceCount));
    }
    void KRender::setClearColor(const KColor &Color){
        DGL_CALL(glClearColor((GLclampf)((F32)Color.r / 255.0f),
                              (GLclampf)((F32)Color.g / 255.0f),
                              (GLclampf)((F32)Color.b / 255.0f),
                              (GLclampf)((F32)Color.a / 255.0f)));
    }

    void KRender::setLPSize(KGeoPrimitiveTypes Type, F32 Size){
        switch (Type){
        // line
        case KGP_LINES:
            DGL_CALL(glLineWidth(Size));
            break;

        // point
        case KGP_POINTS:
            DGL_CALL(glPointSize(Size));
            break;

        // invalid ...
        default:
            KDEBUG_PRINT("invalid type");
            break;
        }
    }

    void KRender::setViewport(const KRectI32 &Viewport){
        _kviewport = Viewport;
        DGL_CALL(glViewport(_kviewport.left,
                            _kviewport.bottom,
                            _kviewport.right,
                            _kviewport.top));
    }
}
