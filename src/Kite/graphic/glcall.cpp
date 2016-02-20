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

namespace Kite{
namespace Internal{

    bool initeGLEW(){
        static bool isInit = false;
        if (isInit == false){
            glewExperimental = GL_TRUE;
            GLenum err = glewInit();
            if (err != GLEW_OK){
                KDEBUG_PRINT("glewInit failed.");
                KDEBUG_BREAK;
                return false;
            }
        }

        // free error queue just in case
        GLenum OGLErrCode = glGetError();
        while (OGLErrCode != GL_NO_ERROR){
            OGLErrCode = glGetError();
        }

        isInit = true;
        return true;
    }

    bool checkLastGLErr(){

        // get the last OpenGL error
        GLenum OGLErrCode = glGetError();
        bool ret = true;

        while (OGLErrCode != GL_NO_ERROR){
            ret = false;
            // decode the error code
            switch (OGLErrCode){
            case GL_INVALID_ENUM:
                KDEBUG_PRINT("GL_INVALID_ENUM");
                break;

            case GL_INVALID_VALUE:
                KDEBUG_PRINT("GL_INVALID_VALUE");
                break;

            case GL_INVALID_OPERATION:
                KDEBUG_PRINT("GL_INVALID_OPERATION");
                break;

            case GL_STACK_OVERFLOW:
                KDEBUG_PRINT("GL_STACK_OVERFLOW");
                break;

            case GL_STACK_UNDERFLOW:
                KDEBUG_PRINT("GL_STACK_UNDERFLOW");
                break;

            case GL_OUT_OF_MEMORY:
                KDEBUG_PRINT("GL_OUT_OF_MEMORY");
                break;

            case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
                KDEBUG_PRINT("GL_INVALID_FRAMEBUFFER_OPERATION_EXT");
                break;

            default:
                KDEBUG_PRINT("GL_UNKNOWN_ERROR");
                break;
            }
            OGLErrCode = glGetError();
        }
        return ret;
    }

	GLBindGuard::GLBindGuard(KGLBindGuardTypes Type, GLint LastObjectID, GLint CurObjectID) :
		_ktypes(Type), _kglLastObj(LastObjectID), _kglCurObj(CurObjectID)
    {}

	GLBindGuard::~GLBindGuard(){
		if (_kglLastObj != _kglCurObj){
			switch (_ktypes){
			case KBG_TEXTURE:
				DGL_CALL(glBindTexture(GL_TEXTURE_2D, _kglLastObj));
				break;
			case KBG_VBUFFER:
				DGL_CALL(glBindBuffer(GL_ARRAY_BUFFER_ARB, _kglLastObj));
				break;
			case KBG_FBUFFER:
				DGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, _kglLastObj));
				break;
			default:
				KDEBUG_PRINT("Invalid types.");
				break;
			}
		}
    }
}
}
