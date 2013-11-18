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
#ifndef KFPSFIXER_H
#define KFPSFIXER_H

#include "Kite/system/ksystemdef.h"
#include "Kite/system/ktimer.h"

namespace Kite{
    class KFPSFixer{
    public:
        KFPSFixer(F64 fTimeInMillsec = 0.0f, bool useSleep = false);

        bool start();

        F64 fps;
        bool useSleep;
    private:
        KTimer m_timer;
        F64 m_startTime;
        bool m_firstTime;
    };
}

#endif // KFPSFIXER_H
