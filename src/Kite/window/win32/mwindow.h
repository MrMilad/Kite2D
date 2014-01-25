#ifndef MWINDOW_H
#define MWINDOW_H

#include "src/Kite/window/win32/window.h"
#include "src/Kite/window/win32/wglcontext.h"

namespace Kite{
namespace Internal{
    class MWindow{
    public:
        Window *_kwindow;
        WGLContext *_kcontext; // one context per window
    };
}
}

#endif // MWINDOW_H
