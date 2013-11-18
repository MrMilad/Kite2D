/*    
    Kite2D Game Framework.
    Copyright (C) 2010-2013  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
