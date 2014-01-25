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

