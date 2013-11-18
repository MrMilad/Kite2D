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
