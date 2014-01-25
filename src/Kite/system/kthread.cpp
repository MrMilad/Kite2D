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
