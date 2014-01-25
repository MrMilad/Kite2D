#ifndef WINDOW_H
#define WINDOW_H

#include "Windows.h"
#include "Kite/system/knoncopyable.h"
#include "Kite/window/kwindowstructs.h"
#include "Kite/window/kwindowtypes.h"

namespace Kite{
namespace Internal{
    class Window : KNonCopyable{
    public:
        Window();
        virtual ~Window();

        void open(KWindowState &WindowState);
        void close();
        static inline bool update(){
            KDEBUG_ASSERT_T(_klastActiveWin);
            static MSG msg;
            while(true){
                if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
                    //TranslateMessage(&msg);
                    DispatchMessage(&msg);
                    continue;
                }
                break;
            }
            return _kcontinueLoop;
        }

        /* window callbacks */
        bool registerCallback(void *Callback, KWindowCallbackTypes CallbackType);
        void unregisterCallback(KWindowCallbackTypes CallbackType);

        /* manipulate window settings */
        void setTitle(const std::wstring &Title);
        void setSize(unsigned int Width, unsigned int Height);
        void setPosition(int XPosition, int YPosition);
        void setShowCursor(bool Enable);
        bool setFullscreen(bool Enable);
        void setParrentWindow(); // used in multyple window creation

        inline const KWindowHandle getWindowHandle() const {return (KWindowHandle) _kwindowHandle;}
        inline const KWindowState &getWindowState() const {return _kwinState;}
        inline const bool isOpen() const {return _kisOpen;}
        inline const bool isParrentWindow() const {return _kisActiveWin;} // used in multyple window creation

    private:
        static LRESULT WINAPI _WinProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
        HWND _kwindowHandle;
        HINSTANCE _kinstance;
        KWindowState _kwinState;
        bool _kisActiveWin;
        static bool _kcontinueLoop;
        bool _kisOpen;
        wchar_t _kclassName[14];
        unsigned int  _kextraW, _kextraH;

        // callbacks
        KCallWinSizeChange *_kcallWinSizeChange;
        KCallWinPositionChange *_kcallWinPosChange;
        KCallWinFocusChange *_kcallWinFocusChange;
        KCallWinClose *_kcallWinClose;

        static unsigned int  _kclassCounter;
        static Window *_klastActiveWin;
    };
}
}

#endif // WINDOW_H
