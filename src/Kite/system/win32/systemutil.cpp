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
#include "Kite/system/ksystemutil.h"
#include <cstdio>
#include <windows.h>

namespace Kite{
    void sleep(U64 MicroSec){
        static bool m_isPeriodSet = false;
        if (!m_isPeriodSet){
            timeBeginPeriod(1);
            m_isPeriodSet = true;
        }
        Sleep(MicroSec);
    }

    KPowerState getPowerState(){
        SYSTEM_POWER_STATUS m_pState;
        KPowerState m_ret;
        if (GetSystemPowerStatus(&m_pState) == 0){
            KDEBUG_PRINT("API Fail: GetSystemPowerStatus")
            return m_ret;
        }
        switch(m_pState.ACLineStatus){
        case 0: // AC Offline
            m_ret.powerType = KPOWER_ACOFF;
            if (m_pState.BatteryFlag < 8){
                m_ret.batteryTypes = KBATTERY_USING;
                m_ret.batteryLife = m_pState.BatteryLifePercent;
            }else if (m_pState.BatteryFlag == 8){
                m_ret.batteryTypes = KBATTERY_CHARGING;
                m_ret.batteryLife = 100;
            }else if (m_pState.BatteryFlag == 128){
                m_ret.batteryTypes = KBATTERY_NOBATTERY;
                m_ret.batteryLife = 0;
            }else{
                m_ret.batteryTypes = KBATTERY_UNKNOWN;
                m_ret.batteryLife = 0;
            }
            break;
        case 1: // AC Online
            m_ret.powerType = KPOWER_ACON;
            m_ret.batteryTypes = KBATTERY_NOBATTERY;
            m_ret.batteryLife = 0;
            break;
        default: // Unknown
            m_ret.powerType = KPOWER_UNKNOWN;
            m_ret.batteryTypes = KBATTERY_UNKNOWN;
            m_ret.batteryLife = 0;
            break;
        }
        return m_ret;
    }

}
