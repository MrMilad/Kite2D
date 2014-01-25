#include "src/Kite/system/win32/timer.h"

namespace Kite{
namespace Internal{
    Timer::Timer(){
        QueryPerformanceFrequency(&frequency);
        startCount.QuadPart = 0;
        endCount.QuadPart = 0;
        stopped = false;
        startTimeInMicroSec = 0;
        endTimeInMicroSec = 0;
    }

    Timer::~Timer(){}

    // startCount will be set at this point.
    void Timer::start(){
        stopped = false; // reset stop flag
        QueryPerformanceCounter(&startCount);
    }

    // endCount will be set at this point.
    void Timer::stop(){
        stopped = true; // set timer stopped flag
        QueryPerformanceCounter(&endCount);
    }

    // compute elapsed time in micro-second resolution.
    // other getElapsedTime will call this first, then convert to correspond resolution.
    F64 Timer::getElapsedTimeInMicroSec(){
        if(!stopped)
            QueryPerformanceCounter(&endCount);
        startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
        endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);
        return endTimeInMicroSec - startTimeInMicroSec;
    }

//    F64 KTimer::getElapsedTimeInMilliSec(){
//        // divide elapsedTimeInMicroSec by 1000
//        return this->getElapsedTimeInMicroSec() * 0.001;
//    }

//    F64 KTimer::getElapsedTimeInSec(){
//        // divide elapsedTimeInMicroSec by 1000000
//        return this->getElapsedTimeInMicroSec() * 0.000001;
//    }

//    F64 KTimer::getElapsedTime(){
//        return this->getElapsedTimeInSec();
//    }

}
}
