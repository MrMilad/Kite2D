/* 
 Kite2D Game Framework.
 Copyright (C) 2010-2014  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or 
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Kite/core/system/ksystemdef.h"
#include "Kite/core/window/kglmwindow.h"

#if defined (KITE_PLATFORM_WINDOWS)

    #include "src/Kite/core/window/win32/mwindow.h"

#elif defined (KITE_PLATFORM_LINUX)


#elif defined (KITE_PLATFORM_MACOS)


#endif

namespace Kite{
    KGLMWindow::KGLMWindow():
        _kimpl(new Internal::MWindow())
    {
        #if defined (KITE_PLATFORM_WINDOWS)
        _kimpl->_kwindow = new Kite::Internal::Window();
		_kimpl->_kcontext = new Kite::Internal::WGLContext(); 
        #elif defined (KITE_PLATFORM_LINUX)


        #elif defined (KITE_PLATFORM_MACOS)


        #endif
    }

    KGLMWindow::KGLMWindow(KWindowState &WindowState):
        _kimpl(new Internal::MWindow()),
        _kwinstate(WindowState)
    {
        #if defined (KITE_PLATFORM_WINDOWS)
        _kimpl->_kwindow = new Kite::Internal::Window();
        _kimpl->_kcontext = new Kite::Internal::WGLContext();
        #elif defined (KITE_PLATFORM_LINUX)


        #elif defined (KITE_PLATFORM_MACOS)


        #endif
    }

    KGLMWindow::~KGLMWindow(){
    #if defined (KITE_PLATFORM_WINDOWS)
        //m_impl->_kcontext->unbind();
        delete _kimpl->_kcontext;
        _kimpl->_kcontext = 0;
        //m_impl->_kwindow->close();
        delete _kimpl->_kwindow;
        _kimpl->_kwindow = 0;
        delete _kimpl;
        _kimpl = 0;
    #elif defined (KITE_PLATFORM_LINUX)


    #elif defined (KITE_PLATFORM_MACOS)


    #endif
    }

    void KGLMWindow::setTitle(const std::wstring &Title){
        _kimpl->_kwindow->setTitle(Title);
    }

    void KGLMWindow::setSize(U32 Width, U32 Height){
        _kimpl->_kwindow->setSize(Width, Height);
    }

    void KGLMWindow::setPosition(U32 XPosition, U32 YPosition){
        _kimpl->_kwindow->setPosition(XPosition, YPosition);
    }

    void KGLMWindow::setShowCursor(bool Show){
        _kimpl->_kwindow->setShowCursor(Show);
    }

    bool KGLMWindow::setFullscreen(bool Fullscreen){
        return _kimpl->_kwindow->setFullscreen(Fullscreen);
    }

    void KGLMWindow::setParrentWindow(){
        _kimpl->_kwindow->setParrentWindow();
    }

    void KGLMWindow::setActiveRender(){
        _kimpl->_kcontext->setActiveRender();
    }

    void KGLMWindow::setVSync(bool Enable){
        _kimpl->_kcontext->setVSync(Enable);
    }

    KWindowHandle KGLMWindow::getWindowHandle() const{
        return (KWindowHandle) _kimpl->_kwindow->getWindowHandle();
    }

    const KWindowState &KGLMWindow::getWindowState() const{
        return _kimpl->_kwindow->getWindowState();
    }

    bool KGLMWindow::isParrentWindow() const{
        return _kimpl->_kwindow->isParrentWindow();
    }

    bool KGLMWindow::isOpen() const{
        return _kimpl->_kwindow->isOpen();
    }

    bool KGLMWindow::isActiveRender() const{
        return _kimpl->_kcontext->isActiveRender();
    }

    bool KGLMWindow::isVsyncEnable() const{
        return _kimpl->_kcontext->isVsyncEnable();
    }

    bool KGLMWindow::registerCallback(void *Callback, KWindowCallbackTypes CallbackType){
        return _kimpl->_kwindow->registerCallback(Callback, CallbackType);
    }

    void KGLMWindow::unregisterCallback(KWindowCallbackTypes CallbackType){
        _kimpl->_kwindow->unregisterCallback(CallbackType);
    }

    bool KGLMWindow::isAnyCreatedContext(){
        #if defined (KITE_PLATFORM_WINDOWS)
        return Internal::WGLContext::isAnyContext();
        #elif defined (KITE_PLATFORM_LINUX)


        #elif defined (KITE_PLATFORM_MACOS)


        #endif

    }

    /* update window events */
    bool KGLMWindow::update(){
        #if defined (KITE_PLATFORM_WINDOWS)
        return Internal::Window::update();
        #elif defined (KITE_PLATFORM_LINUX)


        #elif defined (KITE_PLATFORM_MACOS)


        #endif
    }

    void KGLMWindow::display(){
        _kimpl->_kcontext->display();
    }

    void KGLMWindow::open(){
        // open window then bind gl context
        _kimpl->_kwindow->open(_kwinstate);
        _kimpl->_kcontext->bind(_kimpl->_kwindow->getWindowHandle(),
                                _kimpl->_kwindow->getWindowState(),
                                0); // no need to share contexts usually
    }

    void KGLMWindow::open(KWindowState &WindowState){
        _kwinstate = WindowState;
        // open window then bind gl context
        _kimpl->_kwindow->open(WindowState);
        _kimpl->_kcontext->bind(_kimpl->_kwindow->getWindowHandle(),
                                _kimpl->_kwindow->getWindowState(),
                                0); // no need to share contexts usually
    }

    void KGLMWindow::close(){
        // close window then unbind gl context
        _kimpl->_kwindow->close();
        _kimpl->_kcontext->unbind();
    }
}
