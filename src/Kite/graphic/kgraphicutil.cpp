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
        DGL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnits));
        return (I32)maxUnits;
    }

    I32 getMaximumVertexAttrib(){
        GLint maxVertAttr;
        DGL_CALL(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertAttr));
        return (I32)maxVertAttr;
    }

    // not work with gl 3.3 context
//    bool isShaderAvailable(){
//        return false;
//        return GLEW_ARB_shading_language_100 &&
//               GLEW_ARB_shader_objects       &&
//               GLEW_ARB_vertex_shader        &&
//               GLEW_ARB_fragment_shader;
//    }

	const std::string &getBuiltinShader(KBuiltinShaderTypes Types) {
		static std::string source;
		source.clear();

		switch (Types) {
		case Kite::KBS_VERT:
			source = "#version 330\n"
				"attribute vec2 in_pos;"
				"attribute vec2 in_uv;"
				"attribute vec4 in_col;"
				"out vec4 ex_col;"
				"out vec2 ex_uv;"
				"void main(void){"
				"gl_Position = vec4(in_pos, 0.0, 1.0);"
				"ex_uv = in_uv;"
				"ex_col = in_col;}";
			return source;

		case Kite::KBS_VERT_POINT:
			source = "#version 330\n"
				"attribute vec2 in_pos;"
				"attribute vec2 in_uv;"
				"attribute vec4 in_col;"
				"attribute vec3 in_psprite;"
				"out vec4 ex_col;"
				"out vec2 ex_uv;"
				"out vec2 ex_tsize;"
				"void main(void) {"
				"gl_Position = vec4(in_pos, 0.0, 1.0);"
				"ex_col = in_col;"
				"ex_uv = in_uv;"
				"gl_PointSize = in_psprite.x;"
				"ex_tsize = vec2(in_psprite.y, in_psprite.z);}";
			return source;

		case Kite::KBS_FRAG_COLOR:
			source = "#version 330\n"
				"in vec4 ex_col;"
				"in vec2 ex_uv;"
				"uniform sampler2D in_texture;"
				"out vec4 out_col;"
				"void main(void) {"
				"out_col = ex_col;}";
			return source;

		case Kite::KBS_FRAG_TEXTURE:
			source = "#version 330\n"
				"in vec4 ex_col;"
				"in vec2 ex_uv;"
				"uniform sampler2D in_texture;"
				"out vec4 out_col;"
				"void main(void) {"
				"out_col = texture2D( in_texture, ex_uv);}";
			return source;

		case Kite::KBS_FRAG_TEXTURE_COLOR:
			source = "#version 330\n"
				"in vec4 ex_col;"
				"in vec2 ex_uv;"
				"uniform sampler2D in_texture;"
				"out vec4 out_col;"
				"void main(void) {"
				"out_col = texture2D( in_texture, ex_uv) * ex_col;}";
			return source;

		case Kite::KBS_FRAG_POINT_COLOR:
			source = "#version 330\n"
				"in vec4 ex_col;"
				"in vec2 ex_uv;"
				"in vec2 ex_tsize;"
				"uniform sampler2D in_texture;"
				"out vec4 out_col;"
				"void main(void) {"
				"vec2 realTexCoord = ex_uv + (gl_PointCoord * ex_tsize);"
				"vec4 fragColor = texture2D(in_texture, realTexCoord);"
				"out_col = ex_col;}";
			return source;

		case Kite::KBS_FRAG_POINT_TEXTURE:
			source = "#version 330\n"
				"in vec4 ex_col;"
				"in vec2 ex_uv;"
				"in vec2 ex_tsize;"
				"uniform sampler2D in_texture;"
				"out vec4 out_col;"
				"void main(void) {"
				"vec2 realTexCoord = ex_uv + (gl_PointCoord * ex_tsize);"
				"vec4 fragColor = texture2D(in_texture, realTexCoord);"
				"out_col = fragColor;}";
			return source;

		case Kite::KBS_FRAG_POINT_TEXTURE_COLOR:
			source = "#version 330\n"
				"in vec4 ex_col;"
				"in vec2 ex_uv;"
				"in vec2 ex_tsize;"
				"uniform sampler2D in_texture;"
				"out vec4 out_col;"
				"void main(void) {"
				"vec2 realTexCoord = ex_uv + (gl_PointCoord * ex_tsize);"
				"vec4 fragColor = texture2D(in_texture, realTexCoord);"
				"out_col = fragColor * ex_col;}";
			return source;

		default:
			KDEBUG_PRINT("invalid shader type");
			break;
		}
		return source;
	}
}
