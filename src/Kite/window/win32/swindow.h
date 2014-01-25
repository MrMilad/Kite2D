#ifndef SWINDOW_H
#define SWINDOW_H

#include "Kite/window/win32/window.h"
#include "Kite/window/win32/wglcontext.h"

namespace Kite{
namespace Internal{
    class SWindow{
    public:
        static wglcontext _kcontext; // single context for all windows
        Window _kwindow;
    };
}
}

#endif // SWINDOW_H
