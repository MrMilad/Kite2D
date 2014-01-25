#ifndef KGLMWINDOW_H
#define KGLMWINDOW_H

#include <string>
#include "Kite/system/ksystemdef.h"
#include "Kite/window/kwindowtypes.h"
#include "Kite/window/kwindowstructs.h"
#include "Kite/system/knoncopyable.h"

namespace Kite{
namespace Internal{
    class MWindow;
}
    class KITE_FUNC_EXPORT KGLMWindow : KNonCopyable{
    public:
        KGLMWindow(); // (window will created after open() call)
        KGLMWindow(KWindowState &WindowState); // (window will created after open() call)
        ~KGLMWindow();

        static bool update();
        void display(); // swap buffer

        void open();
        void open(KWindowState &WindowState);
        void close();

        /* manipulate window settings */
        void setTitle(const std::wstring &Title);
        void setSize(U32 Width, U32 Height);
        void setPosition(U32 XPosition, U32 YPosition);
        void setShowCursor(bool Enable);
        bool setFullscreen(bool Enable);
        void setParrentWindow(); // multyple window
        void setActiveRender(); // multyple window
        void setVSync(bool Enable);

        KWindowHandle getWindowHandle() const;
        const KWindowState &getWindowState() const;
        bool isOpen() const;
        bool isActiveRender() const;
        bool isVsyncEnable() const;
        bool isParrentWindow() const;
        static bool isAnyCreatedContext();

        /* window callbacks */
        bool registerCallback(void *Callback, KWindowCallbackTypes CallbackType);
        void unregisterCallback(KWindowCallbackTypes CallbackType);

    private:
        Internal::MWindow *_kimpl; // platform-specific implementation
        KWindowState _kwinstate;
    };
}

#endif // KGLMWINDOW_H
