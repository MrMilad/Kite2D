#ifndef KMUTEX_H
#define KMUTEX_H

#include "include/Kite/system/ksystemdef.h"
#include "include/Kite/system/knoncopyable.h"

namespace Kite{
namespace Internal{
    class Mutex;
}
    class KITE_FUNC_EXPORT KMutex : KNonCopyable{
    public:
        KMutex();
        ~KMutex();

        void lock();
        void unlock();

    private:
        Internal::Mutex *_kimpl;
    };
}

#endif // KMUTEX_H
