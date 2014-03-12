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
#include "Kite/core/system/ksystemdef.h"
#include "Kite/core/system/ktimer.h"

#if defined(KITE_PLATFORM_WINDOWS)
    #include "src/Kite/core/system/win32/timer.h"
#else
    #include "Kite/core/system/unix/timer.h"
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
