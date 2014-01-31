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
