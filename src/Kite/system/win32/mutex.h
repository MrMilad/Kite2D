#ifndef KMUTEXIMPL_H
#define KMUTEXIMPL_H

#include "windows.h"

namespace Kite{
namespace Internal{
    class Mutex{
    public:
        Mutex();
        ~Mutex();

        void lock();
        void unlock();

    private:
        CRITICAL_SECTION _ksysmutex;
    };
}
}

#endif // KMUTEXIMPL_H
