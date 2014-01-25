#include "Kite/system/ksystemdef.h"
#include "Kite/system/ktimer.h"

#if defined(KITE_PLATFORM_WINDOWS)
    #include "Kite/system/win32/timer.h"
#else
    #include "Kite/system/unix/timer.h"
#endif

namespace Kite{

    KTimer::KTimer():
        _kimpl(new Internal::Timer())
    {}

    KTimer::~KTimer(){
        delete _kimpl;
    }

    void KTimer::start(){
        _kimpl->start();
    }

    void KTimer::stop(){
        _kimpl->stop();
    }

    F64 KTimer::getElapsedTime(){
        return _kimpl->getElapsedTimeInMicroSec() * 0.000001;
    }

    F64 KTimer::getElapsedTimeInSec(){
        // divide elapsedTimeInMicroSec by 1000000
        return _kimpl->getElapsedTimeInMicroSec() * 0.000001;
    }

    F64 KTimer::getElapsedTimeInMilliSec(){
        // divide elapsedTimeInMicroSec by 1000
        return _kimpl->getElapsedTimeInMicroSec() * 0.001;
    }

    F64 KTimer::getElapsedTimeInMicroSec(){
        return _kimpl->getElapsedTimeInMicroSec();
    }
}
