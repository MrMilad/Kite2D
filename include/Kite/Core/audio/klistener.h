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
#ifndef KLISTENER_H
#define KLISTENER_H

#include "Kite/system/ksystemdef.h"
#include "Kite/system/kvector3.h"

namespace Kite{
    class KITE_FUNC_EXPORT KListener{
    public:

        /// master global gain
        /// default 1
        static void setMasterGain(F32 Gain);
        static F32 getMasterGain();

        /// listener position
        /// pass (-1, 0, 0) or (1, 0, 0) for 2D panning
        /// default (0, 0, 0)
        static void setPosition(const KVector3F32 &Position);
        static KVector3F32 getPosition();

        /// listener orientation
        /// default (0, 0, 0)
        static void setOrientation(const KVector3F32 &Orientation);
        static KVector3F32 getOrientation();

    };
}

#endif // KLISTENER_H
