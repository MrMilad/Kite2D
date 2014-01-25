/*  High Resolution Timer Class
    Timer class provides timing and stopwatch features to the engine */

#ifndef KTIMER_H
#define KTIMER_H

#include "Kite/system/ksystemdef.h"

namespace Kite {
namespace Internal{
    class Timer;
}
    class KITE_FUNC_EXPORT KTimer{
    public:
        KTimer();                                // default constructor
        ~KTimer();                               // default destructor

        void start();                            // start timer
        void stop();                             // stop the timer
        F64 getElapsedTime();                    // get elapsed time in second
        F64 getElapsedTimeInSec();               // get elapsed time in second (same as getElapsedTime)
        F64 getElapsedTimeInMilliSec();          // get elapsed time in milli-second
        F64 getElapsedTimeInMicroSec();          // get elapsed time in micro-second

    private:

        Internal::Timer *_kimpl; // platform-specific object
    };
}
#endif // KTIMER_H
