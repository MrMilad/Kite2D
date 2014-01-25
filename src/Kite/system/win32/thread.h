#ifndef THREAD_H
#define THREAD_H

#include "windows.h"
#include "include/Kite/system/ksystemdef.h"
#include "include/Kite/system/knoncopyable.h"
#include "include/Kite/system/kthread.h"

namespace Kite{
namespace Internal{
    class Thread : KNonCopyable{
    public:
        Thread(KThread *ThreadOwner);
        ~Thread();

        void wait();
        void terminate();
        bool isRunning();

    private:
        static unsigned int __stdcall threadEntryPoint(void* anyData);
        HWND _kthreadHandle;
        U32 _kthreadID;
    };
}
}

#endif // KTHREADIMPL_H
