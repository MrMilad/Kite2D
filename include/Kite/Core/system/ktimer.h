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
#ifndef KTIMER_H
#define KTIMER_H

#include "Kite/core/system/ksystemdef.h"

namespace Kite {
    class KITE_FUNC_EXPORT KTimer{
    public:
		KTimer();

        /// start timer
        void start();

        /// stop the timer
        void stop();

		/// timer scale (default = 1)
		inline void setScale(F32 Scale) { _kscale = Scale; }
		inline F32 getScale() const { return _kscale; }

        /// get elapsed time in second
        F64 getElapsedSec();
		F64 getScaledElapsedSec();

        /// set all timers to 0
        static void resetAllTimers();

    private:
        F64 _kdelta;
		F32 _kscale;
    };
}
#endif // KTIMER_H
