#ifndef WGLCONTEXT_H
#define WGLCONTEXT_H

#include "windows.h"
#include "WinGDI.h"
#include "Kite/system/knoncopyable.h"
#include "Kite/window/kwindowstructs.h"

namespace Kite{
namespace Internal{
    class WGLContext : KNonCopyable{
    public:
        WGLContext();
        virtual ~WGLContext();

        void bind(KWindowHandle WindowHandle,
                  const KWindowState &WindowState,
                  const WGLContext *ShareContext);
        void unbind();

        inline void display(){
            KDEBUG_ASSERT_T(_kcontext);
            SwapBuffers(_khdc);
        }

        void setActiveRender();
        void setVSync(bool Enable);

        inline const WGLContext *getSharedContext() const {return _ksharedContext;}
        inline const KContextHandle getContext() const {return (KContextHandle)_kcontext;}
        inline bool isVsyncEnable() const {return _kisVSync;}
        inline bool isActiveRender() const {return _kisActiveCon;}
        static inline bool isAnyContext() {if (_kinsCounter > 0) return true; return false;}

    private:
        static void _inite();
        void _create();
        const WGLContext *_ksharedContext;
        KWindowState _kwinState;
        HGLRC _kcontext;
        HWND _khwnd;
        HDC _khdc;
        bool _kisActiveCon;
        bool _kisVSync;
        static bool _kisInited;
        static unsigned int _kinsCounter;
    };
}
}

#endif // KWGLCONTEXT_H
