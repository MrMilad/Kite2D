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
#include "src/Kite/core/graphic/glcall.h"
#include "Kite/core/graphic/kgraphicutil.h"

namespace Kite{
    I32 getMaximumTextureSize(){
        GLint size;
        DGL_CALL(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size));
        return (I32)size;
    }


    I32 getMaximumTextureUnit(){
        GLint maxUnits;
        DGL_CALL(glGetIntegerv(GL_MAX_TEXTURE_COORDS_ARB, &maxUnits));
        return (I32)maxUnits;
    }

    I32 getMaximumVertexAttrib(){
        GLint maxVertAttr;
        DGL_CALL(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertAttr));
        return (I32)maxVertAttr;
    }

    bool isShaderAvailable(){
        return GLEW_ARB_shading_language_100 &&
               GLEW_ARB_shader_objects       &&
               GLEW_ARB_vertex_shader        &&
               GLEW_ARB_fragment_shader;
    }
}
