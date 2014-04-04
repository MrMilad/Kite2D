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
#include "src/Kite/Core/window/fwcall.h"

namespace Kite{
    void KTimer::start(){
        // initialize glfw
        Internal::initeGLFW();

        // get current time
        _kdelta = glfwGetTime();
    }

    void KTimer::stop(){
        F64 end = glfwGetTime();
        _kdelta = end - _kdelta;
    }

    F64 KTimer::getElapsedTimeInSec(){
        return (glfwGetTime() - _kdelta);
    }

    void KTimer::resetAllTimers(){
        glfwSetTime(0.0);
    }
}
