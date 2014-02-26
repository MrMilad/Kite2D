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
#include "include/Kite/system/kthread.h"

#if defined(KITE_PLATFORM_WINDOWS)

    #include "src/Kite/system/win32/thread.h"

#elif defined(KITE_PLATFORM_LINUX)


#elif defined(KITE_PLATFORM_MACOS)


#endif

namespace Kite{

    KThread::~KThread(){
        wait();
        delete _kthreadFunc;
    }

    void KThread::lunch(){
        wait();
        _kimpl = new Internal::Thread(this);
    }

    void KThread::wait(){
        if (_kimpl){
            _kimpl->wait();
            delete _kimpl;
            _kimpl = 0;
        }
    }

    void KThread::terminate(){
        if (_kimpl){
            _kimpl->terminate();
            delete _kimpl;
            _kimpl = 0;
        }
    }

    bool KThread::isRunning(){
        if (_kimpl){
            return _kimpl->isRunning();
        }
        return false;
    }

    void KThread::runFunc(){
        _kthreadFunc->start();
    }
}
