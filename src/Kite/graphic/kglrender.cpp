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
#include "Kite/graphic/kglrender.h"
#include "Kite/graphic/glcall.h"

namespace Kite{
    KGLRender::KGLRender(){}

    KGLRender::~KGLRender(){}

    void KGLRender::pushStates(){
        DGL_CALL(glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS));
        DGL_CALL(glPushAttrib(GL_ALL_ATTRIB_BITS));
        DGL_CALL(glMatrixMode(GL_MODELVIEW));
        DGL_CALL(glPushMatrix());
        DGL_CALL(glMatrixMode(GL_PROJECTION));
        DGL_CALL(glPushMatrix());
        DGL_CALL(glMatrixMode(GL_TEXTURE));
        DGL_CALL(glPushMatrix());

        initialize();
    }

    void KGLRender::popStates(){
        DGL_CALL(glPopClientAttrib());
        DGL_CALL(glPopAttrib());
        DGL_CALL(glMatrixMode(GL_PROJECTION));
        DGL_CALL(glPopMatrix());
        DGL_CALL(glMatrixMode(GL_MODELVIEW));
        DGL_CALL(glPopMatrix());
        DGL_CALL(glMatrixMode(GL_TEXTURE));
        DGL_CALL(glPopMatrix());
    }

    bool KGLRender::init(){
        return Internal::initeGLEW();
    }
}

