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
                printf("GL_INVALID_ENUM\n");
                break;
            }

            case GL_INVALID_VALUE :{
                printf("GL_INVALID_VALUE\n");
                break;
            }

            case GL_INVALID_OPERATION :{
                printf("GL_INVALID_OPERATION\n");
                break;
            }

            case GL_STACK_OVERFLOW :{
                printf("GL_STACK_OVERFLOW\n");
                break;
            }

            case GL_STACK_UNDERFLOW :{
                printf("GL_STACK_UNDERFLOW\n");
                break;
            }

            case GL_OUT_OF_MEMORY :{
                printf("GL_OUT_OF_MEMORY\n");
                break;
            }

            case GL_INVALID_FRAMEBUFFER_OPERATION_EXT :{
                printf("GL_INVALID_FRAMEBUFFER_OPERATION_EXT\n");
                break;
            }
            default:
                printf("UNKNOWN_ERROR\n");
                break;
            }
            OGLErrCode = glGetError();
        }
        return ret;
    }

    GLBindGuard::GLBindGuard(GLBindGuardTypes type, GLint objectID):
        _ktypes(type), _kglobject(objectID)
    {}

    GLBindGuard::~GLBindGuard(){
        switch (_ktypes){
        case BGUARD_TEXTURE:
            DGL_CALL(glBindTexture(GL_TEXTURE_2D, _kglobject));
            break;
        case BGUARD_BUFFER:
            DGL_CALL(glBindBuffer(GL_ARRAY_BUFFER_ARB, _kglobject));
            break;
        default:
            break;
        }
    }
}
}
