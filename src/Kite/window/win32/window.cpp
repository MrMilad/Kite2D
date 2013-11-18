/*    
    Kite2D Game Framework.
    Copyright (C) 2010-2013  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "src/Kite/window/win32/window.h"

namespace Kite{
namespace Internal{

    LRESULT WINAPI Window::_WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
        switch(msg){
            case WM_CLOSE:
                KDEBUG_ASSERT_T(_klastActiveWin);
                if (hWnd != _klastActiveWin->_kwindowHandle)
                    break;
                if (_klastActiveWin->_kcallWinClose != 0)
                    if ((*_klastActiveWin->_kcallWinClose)() == true)
                        return TRUE;
                break;
            case WM_QUIT:
            case WM_DESTROY:
                KDEBUG_ASSERT_T(_klastActiveWin);
                if (hWnd != _klastActiveWin->_kwindowHandle)
                    break;
                _klastActiveWin->_kcontinueLoop = false;
            case WM_ACTIVATE:// focus changed
                KDEBUG_ASSERT_T(_klastActiveWin);
                if (hWnd != _klastActiveWin->_kwindowHandle)
                    break;
                bool active;
                active = (LOWORD(wParam) != WA_INACTIVE) && (HIWORD(wParam) == 0);
                if (_klastActiveWin->_kcallWinFocusChange != 0)
                    (*_klastActiveWin->_kcallWinFocusChange)(active);
                break;
            case WM_SIZING:
            case WM_MOVING:
                KDEBUG_ASSERT_T(_klastActiveWin);
                if (hWnd != _klastActiveWin->_kwindowHandle)
                    break;
                // size changed.
                RECT *rect = (RECT *)lParam;
                if (msg == WM_SIZING){ // Size changed
                    if (_klastActiveWin->_kcallWinSizeChange != 0)
                        (*_klastActiveWin->_kcallWinSizeChange)((U32)((rect->right - rect->left) - _klastActiveWin->_kextraW),
                                                             (U32)((rect->bottom - rect->top) - _klastActiveWin->_kextraH));

                    _klastActiveWin->_kwinState.width = (U32)((rect->right - rect->left) - _klastActiveWin->_kextraW);
                    _klastActiveWin->_kwinState.height = (U32)((rect->bottom - rect->top) - _klastActiveWin->_kextraH);
                }else{ // position changed
                    if (_klastActiveWin->_kcallWinPosChange != 0)
                        (*_klastActiveWin->_kcallWinPosChange)((I32)rect->left, (I32)rect->top);
                    _klastActiveWin->_kwinState.xpos = (I32)rect->left;
                    _klastActiveWin->_kwinState.ypos = (I32)rect->top;
                }
                break;
        }
        return DefWindowProc( hWnd, msg, wParam, lParam );
    }

    // statics
    unsigned int Window::_kclassCounter = 0;
    bool Window::_kcontinueLoop = false;
    Window *Window::_klastActiveWin = 0;

    Window::Window():
        _kwindowHandle(0), // Multiple window creation
        _kinstance(0),
        _kisActiveWin(false),
        _kisOpen(false),
        _kextraW(0), _kextraH(0),
        _kcallWinSizeChange(0),
        _kcallWinPosChange(0),
        _kcallWinFocusChange(0),
        _kcallWinClose(0)
    {}

    Window::~Window(){}

    void Window::open(KWindowState &WindowState){
        if (_kwindowHandle != 0){
            KDEBUG_PRINT("window opened before.")
            return;
        }

        _kwinState = WindowState;
        WNDCLASSEXW wc;
        DWORD dwStyle;
        RECT rcClient, rcWind;
        POINT ptDiff;
        HWND topmost = HWND_NOTOPMOST;
        _kinstance = GetModuleHandleW(NULL);

        // generate ordered class name for multyple class creation
        wchar_t cNumber[4];
        wcscpy(cNumber, _itow(++_kclassCounter, cNumber, 10));
        wchar_t cName[14] = {L"KITE_WINCL"};
        wcscat(cName, cNumber);
        wcscpy(_kclassName, cName);

        // fill the struct with info
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc = (WNDPROC)_WinProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = _kinstance;
        wc.hIcon = NULL;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = cName;
        wc.hIconSm = NULL;

        // set up the window with the class info
        KDEBUG_ASSERT(RegisterClassExW(&wc));

        // set up the screen in windowed mode
        // if user need fullscreen window we will switch it in next step
        if (_kwinState.resizable){
            dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
        }else{
            dwStyle = WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE;
        }

        // checking topmost
        if (_kwinState.topmost){
            topmost = HWND_TOPMOST;
        }

        // create the window
        // before anything we must set active window
        // because we need atleast one active window for handling events
        if (_klastActiveWin == 0){
            _klastActiveWin = this;
            _kisActiveWin = true;
            _kcontinueLoop = true;
        }

        //create window
        _kwindowHandle = CreateWindowExW(
                    0,
                    cName, // window class name
                    _kwinState.title.c_str(), // window title
                    dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, // window styles
                    _kwinState.xpos, _kwinState.ypos, // x and y position
                    _kwinState.width, _kwinState.height,
                    NULL, // parent of window
                    NULL, // menu
                    _kinstance, // instance handle
                    NULL
                );

        // calculate real window size (window client size + window border size)
        GetClientRect(_kwindowHandle, &rcClient);
        GetWindowRect(_kwindowHandle, &rcWind);
        ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
        ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
        _kextraW = ptDiff.x;
        _kextraH = ptDiff.y;

        KDEBUG_ASSERT(SetWindowPos(_kwindowHandle, topmost, rcWind.left, rcWind.top,
                                   _kwinState.width + ptDiff.x,
                                   _kwinState.height + ptDiff.y, SWP_SHOWWINDOW));

        // was there an error creating the window?
        KDEBUG_ASSERT_T(_kwindowHandle);

        KDEBUG_ASSERT(UpdateWindow(_kwindowHandle));

        // Cursor setting
        setShowCursor(_kwinState.showCursor);

        // change dysplay to fullscreen
        if (_kwinState.fullscreen){
            _kwinState.fullscreen = false;
            setFullscreen(true);
        }
        _kisOpen = true;
    }

    void Window::close(){
        // if this window is active window
        if (this->_kwindowHandle)
            DestroyWindow(this->_kwindowHandle);
        if (this->_kinstance)
            UnregisterClassW(_kclassName, _kinstance);
        _kwindowHandle = 0;
        if (this == _klastActiveWin){
            _kcontinueLoop = false;
            _klastActiveWin = 0;
        }
        _kisOpen = false;
        unregisterCallback(KCALL_WIN_ALL);
    }

    bool Window::registerCallback(void *Callback, KWindowCallbackTypes CallbackType){
        KDEBUG_ASSERT_T(Callback)

        switch(CallbackType){
        case KCALL_WIN_SIZECHANGE:
            _kcallWinSizeChange = (KCallWinSizeChange *)Callback;
            break;
        case KCALL_WIN_POSCHANGE:
            _kcallWinPosChange = (KCallWinPositionChange *)Callback;
            break;
        case KCALL_WIN_FOCUSCHANGE:
            _kcallWinFocusChange = (KCallWinFocusChange *)Callback;
            break;
        case KCALL_WIN_CLOSE:
            _kcallWinClose = (KCallWinClose *)Callback;
            break;
        default:
            KDEBUG_PRINT("Message: callback type is not valid.");
            return false;
        }
        return true;
    }

    void Window::unregisterCallback(KWindowCallbackTypes CallbackType){
        switch(CallbackType){
        case KCALL_WIN_SIZECHANGE:
            _kcallWinSizeChange = 0;
            break;
        case KCALL_WIN_POSCHANGE:
            _kcallWinPosChange = 0;
            break;
        case KCALL_WIN_FOCUSCHANGE:
            _kcallWinFocusChange = 0;
            break;
        case KCALL_WIN_CLOSE:
            _kcallWinClose = 0;
            break;
        case KCALL_WIN_ALL:
            _kcallWinSizeChange = 0;
            _kcallWinPosChange = 0;
            _kcallWinFocusChange = 0;
            _kcallWinClose = 0;
            break;
        KDEBUG_TEMP(default:);
            KDEBUG_PRINT("Message: callback type is not valid.");
        }
    }

    void Window::setTitle(const std::wstring &Title){
        KDEBUG_ASSERT_T(_kwindowHandle);
        KDEBUG_ASSERT(SetWindowTextW(_kwindowHandle, Title.c_str()));
        _kwinState.title = Title;
    }

    void Window::setSize(unsigned int Width, unsigned int Height){
        KDEBUG_ASSERT_T(_kwindowHandle);
        KDEBUG_ASSERT(SetWindowPos(_kwindowHandle, HWND_TOP, 0, 0, Width, Height, SWP_NOMOVE));
        _kwinState.width = Width;
        _kwinState.height = Height;
    }

    void Window::setPosition(int XPosition, int YPosition){
        KDEBUG_ASSERT_T(_kwindowHandle);
        KDEBUG_ASSERT(SetWindowPos(_kwindowHandle, HWND_TOP, XPosition, YPosition, 0, 0, SWP_NOSIZE));
        _kwinState.xpos = XPosition;
        _kwinState.ypos = YPosition;
    }

    void Window::setShowCursor(bool Enable){
        KDEBUG_ASSERT_T(_kwindowHandle);

        // avoid multiple call
        static bool check = Enable;

        if (Enable != check){
            check = Enable;
            if (Enable){
                ShowCursor(TRUE); // dont use debug assert. (return value is a counter)
                _kwinState.showCursor = true;
            }else{
                ShowCursor(FALSE); // dont use debug assert. (return value is a counter)
                _kwinState.showCursor = false;
            }
        }
    }

    bool Window::setFullscreen(bool Enable){
        KDEBUG_ASSERT_T(_kwindowHandle);
        if (Enable == _kwinState.fullscreen)
            return true;
        if (Enable){

            // switch to fullscreen mode
            DEVMODEW fSet;
            KDEBUG_ASSERT(EnumDisplaySettingsW(NULL, 0, &fSet));
            fSet.dmPelsWidth        = _kwinState.width;
            fSet.dmPelsHeight       = _kwinState.height;
            fSet.dmBitsPerPel       = _kwinState.colorDepth; // only 32-bit is valid (in windows 8)
            fSet.dmFields           = DM_PELSWIDTH |DM_PELSHEIGHT |DM_BITSPERPEL;

            // test our new resolution first (is valid or not)
            if(ChangeDisplaySettingsW(&fSet, CDS_TEST) != DISP_CHANGE_SUCCESSFUL){
                KDEBUG_PRINT("invalid display state (can't switch to fullscreen state).");
                return false;
            }

            // new resolution is valid. we try to set it.
            //KDEBUG_ASSERT(SetWindowLongPtrA(_kwindowHandle, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST))
            KDEBUG_ASSERT(SetWindowLongPtrW(_kwindowHandle, GWL_STYLE, WS_POPUP | WS_VISIBLE));
            KDEBUG_ASSERT(SetWindowPos(_kwindowHandle, HWND_TOPMOST, 0, 0, _kwinState.width,
                                       _kwinState.height, SWP_SHOWWINDOW));
            if(ChangeDisplaySettingsW(&fSet, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL){
                KDEBUG_PRINT("valid display state but cant set it.");
                KDEBUG_BREAK;
            }
            KDEBUG_ASSERT(UpdateWindow(_kwindowHandle));
            _kwinState.fullscreen = true;
            return true;
        }else{

            DWORD wStyle;
            HWND topmost = HWND_NOTOPMOST;
            RECT rcClient, rcWind;
            POINT ptDiff;

            // switch to window mode
            if (ChangeDisplaySettingsW(0, 0) != DISP_CHANGE_SUCCESSFUL){
                KDEBUG_BREAK;
            }

            // is resizable
            if (_kwinState.resizable){
                wStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
            }else{
                wStyle = WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE;
            }

            // is topmost
            if (_kwinState.topmost){
                topmost = HWND_TOPMOST;
            }

            // set windowed style
            KDEBUG_ASSERT(SetWindowLongPtrW(_kwindowHandle, GWL_STYLE, wStyle));

            // before calculating real window size we have to update it.
            this->update();

            // calculate real window size (window client size + window border size)
            GetClientRect(_kwindowHandle, &rcClient);
            GetWindowRect(_kwindowHandle, &rcWind);
            ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
            ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
            _kextraW = ptDiff.x;
            _kextraH = ptDiff.y;

            // set window size and position
            KDEBUG_ASSERT(SetWindowPos(_kwindowHandle, topmost, rcWind.left, rcWind.top,
                                       _kwinState.width + ptDiff.x,
                                       _kwinState.height + ptDiff.y, SWP_SHOWWINDOW));

            KDEBUG_ASSERT(UpdateWindow(_kwindowHandle));
            _kwinState.fullscreen = false;
            return true;
        }
        return false;
    }

    void Window::setParrentWindow(){
        KDEBUG_ASSERT_T(_kwindowHandle);
        _klastActiveWin->_kisActiveWin = false;
        _klastActiveWin = this;
        _kisActiveWin = true;
        SetFocus(_kwindowHandle);
    }

}
}
