#ifndef GLCALL_H
#define GLCALL_H

//#define GLEW_STATIC
#include "GL/glew.h"
#include "Kite/system/ksystemdef.h"
#include "Kite/graphic/kgraphictypes.h"

namespace Kite{
namespace Internal{
    extern bool initeGLEW();
    extern bool checkLastGLErr();

    class GLBindGuard{
    public:
        GLBindGuard(GLBindGuardTypes type = BGUARD_NONE, GLint objectID = 0);
        ~GLBindGuard();
    private:
        GLBindGuardTypes _ktypes; // type of object
        GLuint _kglobject; // current object binding to restore
    };
}
}

// OpenGL debug macro
#if defined(KITE_DEV_DEBUG)
    #define DGL_CALL(call) call; KDEBUG_ASSERT_T(Kite::Internal::checkLastGLErr())
#else
    #define DGL_CALL(call) call;
#endif

#endif // GLCALL_H
