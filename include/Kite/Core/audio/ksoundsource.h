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
#ifndef KSOUNDSOURCE_H
#define KSOUNDSOURCE_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/kvector3.h"
#include "Kite/Core/audio/kaudiotypes.h"

namespace Kite{
    class KITE_FUNC_EXPORT KSoundSource{
    public:
        KSoundSource();
        virtual ~KSoundSource();
        /// basic audio operations
        virtual void play() = 0;
        virtual void pause() = 0;
        virtual void stop() = 0;

        /// loop
        virtual void setLoop(bool Loop) = 0;
        virtual bool getLoop() = 0;

        /// make the sound's position relative to the listener or absolute
        /// default false
        void setReleative(bool Releative);
        bool getReleative();

        /// pitch
        /// default 1
        void setPitch(F32 Pitch);
        F32 getPitch();

        /// volume
        /// range 0 to 100
        /// default 100
        void setVolume(F32 Volume);
        F32 getVolume();

        /// rollof factor
        /// pass 0 for simulate 2D panning
        /// default 1
        void setRolloffFactor(F32 Factor);
        F32 getRolloffFactor();

        /// minimum distance of sound
        /// always positive
        /// default 1
        void setMinDistance(F32 Distance);
        F32 getMinDistance();

        /// position
        /// default (0, 0, 0)
        void setPosition(KVector3F32 Position);
        KVector3F32 getPosition();

        /// current source state
        KSourceStateTypes getState();

        /// get ID
        inline U32 getID() const {return _kID;}

    protected:
        U32 _kID;
    };
}

#endif // KSOUNDSOURCE_H
