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
#ifndef KSYSTEMTYPES_H
#define KSYSTEMTYPES_H

namespace Kite{

/* Power Sourece State */
enum KPowerStateTypes{
    KPOWER_ACON,
    KPOWER_ACOFF,
    KPOWER_UNKNOWN
};

/* Battery State */
enum KBatteryStateTypes{
    KBATTERY_USING,
    KBATTERY_CHARGING,
    KBATTERY_NOBATTERY,
    KBATTERY_UNKNOWN
};

enum KSeqIOAccTypes{
    KSEQIO_ACC_READ,
    KSEQIO_ACC_WRITE,
    KSEQIO_ACC_APPEND,
    KSEQIO_ACC_RW,
    KSEQIO_ACC_RWEX,
    KSEQIO_ACC_RWTOEND
};

enum KVectorComponent{
    KVC_X = 0,
    KVC_Y = 1,
    KVC_Z = 2,
    KVC_W = 3
};

}
#endif // KSYSTEMTYPES_H
