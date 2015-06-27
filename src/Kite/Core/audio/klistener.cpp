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
#include "Kite/core/audio/klistener.h"
#include "src/Kite/core/audio/alcall.h"

namespace Kite{
    void KListener::setMasterGain(F32 Gain){
		Internal::initeAL();
        DAL_CALL(alListenerf(AL_GAIN, Gain * 0.01f));
    }

    F32 KListener::getMasterGain(){
		Internal::initeAL();
        F32 gain = 0.f;
        DAL_CALL(alGetListenerf(AL_GAIN, &gain));

        return gain * 100;
    }

    void KListener::setPosition(const KVector3F32 &Position){
		Internal::initeAL();
        DAL_CALL(alListener3f(AL_POSITION, Position.x, Position.y, Position.z));
    }

    KVector3F32 KListener::getPosition(){
		Internal::initeAL();
        KVector3F32 pos;
        DAL_CALL(alGetListener3f(AL_POSITION, &pos.x, &pos.y, &pos.z));

        return pos;
    }

    void KListener::setOrientation(const KVector3F32 &Orientation){
		Internal::initeAL();
        F32 ori[] = {Orientation.x, Orientation.y, Orientation.z, 0.f, 1.f, 0.f};
        DAL_CALL(alListenerfv(AL_ORIENTATION, ori));
    }

    KVector3F32 KListener::getOrientation(){
		Internal::initeAL();
        F32 ori[6];
        DAL_CALL(alGetListenerfv(AL_ORIENTATION, ori));

        return KVector3F32(ori[0], ori[1], ori[2]);
    }

}
