#include "src/Kite/system/win32/thread.h"
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
