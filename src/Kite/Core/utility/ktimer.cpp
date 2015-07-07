/*
    Kite2D Game Framework.
    Copyright (C) 2010-2015  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
    USA
*/
#include "Kite/core/system/ksystemdef.h"
#include "Kite/core/utility/ktimer.h"
#include "src/Kite/Core/window/fwcall.h"

namespace Kite{
	KTimer::KTimer():
		_kscale(1.0f),
		_kstart(0.0)
	{
		// initialize glfw
		Internal::initeGLFW();
	}
    void KTimer::start(){
        // get current time
		_kstart = glfwGetTime();
    }

    void KTimer::stop(){
        F64 end = glfwGetTime();
		_kstart = end - _kstart;
    }

    F64 KTimer::getElapsedSec(){
		return (glfwGetTime() - _kstart);
    }

	F64 KTimer::getScaledElapsedSec(){
		return ((glfwGetTime() - _kstart) * _kscale);
	}

    void KTimer::resetAllTimers(){
        glfwSetTime(0.0);
    }
}
