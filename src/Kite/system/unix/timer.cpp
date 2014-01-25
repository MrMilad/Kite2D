#include "Kite/system/unix/ktimerimpl.h"

namespace _KPriv{

    // startCount will be set at this point.
    void KTimerImpl::start(){
        stopped = false; // reset stop flag
        gettimeofday(&startCount, NULL);
    }

    // endCount will be set at this point.
    void KTimerImpl::stop(){
        stopped = true; // set timer stopped flag
        gettimeofday(&endCount, NULL);
    }

    // compute elapsed time in micro-second resolution.
    // other getElapsedTime will call this first, then convert to correspond resolution.
    F64 KTimerImpl::getElapsedTimeInMicroSec(){
        if(!stopped)
            gettimeofday(&endCount, NULL);

        startTimeInMicroSec = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
        endTimeInMicroSec = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;
        return endTimeInMicroSec - startTimeInMicroSec;
    }

//    F64 KTimerImpl::getElapsedTimeInMilliSec(){
//        // divide elapsedTimeInMicroSec by 1000
//        return this->getElapsedTimeInMicroSec() * 0.001;
//    }

//    F64 KTimerImpl::getElapsedTimeInSec(){
//        // divide elapsedTimeInMicroSec by 1000000
//        return this->getElapsedTimeInMicroSec() * 0.000001;
//    }

//    F64 KTimerImpl::getElapsedTime(){
//        return this->getElapsedTimeInSec();
//    }

    U32 KTimerImpl::getCurrentTimeSec(){
        time_t rawtime;
        struct tm * timeinfo;

        time( &rawtime );
        timeinfo = localtime ( &rawtime );
        return timeinfo->tm_sec;
    }

    U32 KTimerImpl::getCurrentTimeMin(){
        time_t rawtime;
        struct tm * timeinfo;

        time( &rawtime );
        timeinfo = localtime ( &rawtime );
        return timeinfo->tm_min;
    }

    U32 KTimerImpl::getCurrentTimeHours(){
        time_t rawtime;
        struct tm * timeinfo;

        time( &rawtime );
        timeinfo = localtime ( &rawtime );
        return timeinfo->tm_hour;
    }
}
