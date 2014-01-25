#ifndef KTIMERIMPLUNIX_H
#define KTIMERIMPLUNIX_H

#include <sys/time.h>
#include <time.h>
#include "Kite/system/ksystemdef.h"
using namespace Kite;

namespace _KPriv {
    class KTimerImpl{
    public:
        KTimerImpl();                                   // default constructor
        ~KTimerImpl();                                  // default destructor

        void start();                               // start timer
        void stop();                                // stop the timer
/*        F64 getElapsedTime();                    // get elapsed time in second
        F64 getElapsedTimeInSec();               // get elapsed time in second (same as getElapsedTime)
        F64 getElapsedTimeInMilliSec();   */       // get elapsed time in milli-second
        F64 getElapsedTimeInMicroSec();          // get elapsed time in micro-second

        static U32 getCurrentTimeSec();
        static U32 getCurrentTimeMin();
        static U32 getCurrentTimeHours();

    private:
        F64 startTimeInMicroSec;                 // starting time in micro-second
        F64 endTimeInMicroSec;                   // ending time in micro-second
        bool stopped;                                // stop flag

        timeval startCount;                         //
        timeval endCount;                           //
    };
}

#endif // KTIMERIMPLUNIX_H
