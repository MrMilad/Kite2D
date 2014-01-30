#include "include/Kite/system/kmutex.h"

#if defined(KITE_PLATFORM_WINDOWS)

    #include "src/Kite/system/win32/mutex.h"

#elif defined(KITE_PLATFORM_LINUX)


#elif defined(KITE_PLATFORM_MACOS)


#endif

namespace Kite{

    KMutex::KMutex(){
        _kimpl = new Internal::Mutex();
    }

    KMutex::~KMutex(){
        delete _kimpl;
    }

    void KMutex::lock(){
        _kimpl->lock();
    }

    void KMutex::unlock(){
        _kimpl->unlock();
    }

}
