/* 
 Kite2D Game Framework.
 Copyright (C) 2010-2014  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or 
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
