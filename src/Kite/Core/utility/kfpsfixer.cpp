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
#include "Kite/utility/kfpsfixer.h"
#include "Kite/system/ksystemutil.h"
namespace Kite{

    KFPSFixer::KFPSFixer(F64 fTimeInMillsec, bool useSleep):
        fps(fTimeInMillsec), useSleep(useSleep),
        m_startTime(0.0), m_firstTime(true)
    {}

    bool KFPSFixer::start(){
        if (m_firstTime){
            m_timer.start();
            m_startTime = m_timer.getElapsedTimeInMilliSec();
            m_firstTime = false;
            return true;
        }
        F64 mDelta =  m_timer.getElapsedTimeInMilliSec() - m_startTime;
        if (mDelta < fps){
            if (useSleep && (fps - mDelta) >= 2)
                Kite::sleep(100);
            return false;
        }
        m_timer.start();
        m_startTime = m_timer.getElapsedTimeInMilliSec();
        return true;
    }
}
