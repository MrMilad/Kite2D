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
#include "src/Kite/core/system/win32/thread.h"
#include "process.h"

namespace Kite{
namespace Internal{
    Thread::Thread(KThread *ThreadOwner):
        _kthreadHandle(0),
        _kthreadID(0)
    {
        _kthreadHandle = (HWND)_beginthreadex(NULL, 0, &threadEntryPoint, ThreadOwner, 0, &_kthreadID);
        KDEBUG_ASSERT_T(_kthreadHandle);
    }

    Thread::~Thread(){
        CloseHandle(_kthreadHandle);
    }

    void Thread::wait(){
        KDEBUG_ASSERT_T(_kthreadHandle)

        if (_kthreadID == GetCurrentThreadId()){
            KDEBUG_PRINT("A thread cannot wait for itself!")
            KDEBUG_BREAK
            return;
        }
        WaitForSingleObject(_kthreadHandle, INFINITE);
    }

    void Thread::terminate(){
        TerminateThread((HANDLE)_kthreadHandle, 0);
    }

    bool Thread::isRunning(){
        KDEBUG_ASSERT_T(_kthreadHandle)
        if (_kthreadID == GetCurrentThreadId()){
            KDEBUG_PRINT("A thread cannot wait for itself!")
            return true;
        }
        if (WaitForSingleObject(_kthreadHandle, 0) == WAIT_OBJECT_0)
            return false;
        return true;
    }

    unsigned int __stdcall Thread::threadEntryPoint(void* anyData){
        KThread *tOwner = (KThread *)anyData;
        KDEBUG_ASSERT_T(tOwner);

        tOwner->runFunc();

        return 0;
    }
}
}
