#include "include/Kite/system/kmutex.h"

#if defined(KITE_PLATFORM_WINDOWS)

    #include "src/Kite/system/win32/kmuteximpl.h"

#elif defined(KITE_PLATFORM_LINUX)


#elif defined(KITE_PLATFORM_MACOS)


#endif

namespace Kite{

    KMutex::KMutex(){
        m_impl = new Internal::KMutexImpl();
    }

    KMutex::~KMutex(){
        delete m_impl;
    }

    void KMutex::lock(){
        m_impl->lock();
    }

    void KMutex::unlock(){
        m_impl->unlock();
    }

}
