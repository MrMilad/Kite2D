#ifndef WIN32WINDOW_INL
#define WIN32WINDOW_INL

#include "src/Kite/window/win32/win32window.h"
static inline bool win32window::update(){
    KDEBUG_ASSERT_T(m_lastActiveWin)
    static MSG msg;
    while(true){
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
            //TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }
        break;
    }
    return m_continueLoop;
}

#endif // WIN32WINDOW_INL
