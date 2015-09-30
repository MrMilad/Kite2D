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

    KRectI32 KRender::_kviewport;

    KRender::KRender(){}

    KRender::~KRender(){}

    bool KRender::inite(){
        bool ret = Internal::initeGLEW();
        if (ret){
            DGL_CALL(glDisable(GL_DEPTH_TEST));

            // enable blend
            DGL_CALL(glEnable(GL_BLEND));
            DGL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        }
        return ret;
    }

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
        DGL_CALL(glClearColor((GLclampf)(Color.getGLR()),
			(GLclampf)(Color.getGLG()),
			(GLclampf)(Color.getGLB()),
			(GLclampf)(Color.getGLA())));
    }

	void KRender::setPointSize(F32 Size){
        DGL_CALL(glPointSize(Size));
    }

	void KRender::setWireframe(bool Wireframe){
		if (Wireframe){
			DGL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		}
		else{
			DGL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		}
	}

    void KRender::setViewport(const KRectI32 &Viewport){
        _kviewport = Viewport;
        DGL_CALL(glViewport(_kviewport.left,
                            _kviewport.bottom,
                            _kviewport.right,
                            _kviewport.top));
    }

    const KRectI32 *KRender::getViewport(){
        return &_kviewport;
    }
}
