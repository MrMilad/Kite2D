#include "src/Kite/graphic/glcall.h"

namespace Kite{
namespace Internal{

    bool initeGLEW(){

        static bool isInit = false;
        GLenum err = glewInit();
        if (isInit == false){
            if (err != GLEW_OK){
                KDEBUG_PRINT("glewInit failed.");
                KDEBUG_BREAK;
                return false;
            }
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
            case GL_INVALID_ENUM :{
                KDEBUG_PRINT("GL_INVALID_ENUM\n");
                KDEBUG_BREAK;
                break;
            }

            case GL_INVALID_VALUE :{
                KDEBUG_PRINT("GL_INVALID_VALUE\n");
                KDEBUG_BREAK;
                break;
            }

            case GL_INVALID_OPERATION :{
                KDEBUG_PRINT("GL_INVALID_OPERATION\n");
                KDEBUG_BREAK;
                break;
            }

            case GL_STACK_OVERFLOW :{
                KDEBUG_PRINT("GL_STACK_OVERFLOW\n");
                KDEBUG_BREAK;
                break;
            }

            case GL_STACK_UNDERFLOW :{
                KDEBUG_PRINT("GL_STACK_UNDERFLOW\n");
                KDEBUG_BREAK;
                break;
            }

            case GL_OUT_OF_MEMORY :{
                KDEBUG_PRINT("GL_OUT_OF_MEMORY\n");
                KDEBUG_BREAK;
                break;
            }

            case GL_INVALID_FRAMEBUFFER_OPERATION_EXT :{
                KDEBUG_PRINT("GL_INVALID_FRAMEBUFFER_OPERATION_EXT\n");
                KDEBUG_BREAK;
                break;
            }
            default:
                KDEBUG_PRINT("UNKNOWN_ERROR\n");
                KDEBUG_BREAK;
                break;
            }
            OGLErrCode = glGetError();
        }
        return ret;
    }

    GLBindGuard::GLBindGuard(KGLBindGuardTypes type, GLint objectID):
        _ktypes(type), _kglobject(objectID)
    {}

    GLBindGuard::~GLBindGuard(){
        switch (_ktypes){
        case KBGUARD_TEXTURE:
            DGL_CALL(glBindTexture(GL_TEXTURE_2D, _kglobject));
            break;
        case KBGUARD_BUFFER:
            DGL_CALL(glBindBuffer(GL_ARRAY_BUFFER_ARB, _kglobject));
            break;
        default:
            break;
        }
    }
}
}
