#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include "windows.h"
#include "Kite/system/ksystemdef.h"

namespace Kite{
namespace Internal{
    class KITE_FUNC_EXPORT Timer{
    public:
        Timer();
        ~Timer();

        void start();
        void stop();
        F64 getElapsedTimeInMicroSec();

    private:
        F64 startTimeInMicroSec;                 // starting time in micro-second
        F64 endTimeInMicroSec;                   // ending time in micro-second
        bool stopped;                            // stop flag
        LARGE_INTEGER frequency;                 // ticks per second
        LARGE_INTEGER startCount;                //
        LARGE_INTEGER endCount;                  //
    };
}
}

#endif // TIMER_H
