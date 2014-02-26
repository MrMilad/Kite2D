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
#include "Kite/core/audio/kstaticsource.h"
#include "src/Kite/core/audio/alcall.h"

namespace Kite{
    void KStaticSource::play(){
        DAL_CALL(alSourcePlay(_kID));
    }

    void KStaticSource::pause(){
        DAL_CALL(alSourcePause(_kID));
    }

    void KStaticSource::stop(){
        DAL_CALL(alSourceStop(_kID));
    }

    void KStaticSource::setLoop(bool Loop){
        DAL_CALL(alSourcei(_kID, AL_LOOPING, Loop));
    }

    bool KStaticSource::getLoop(){
        ALint loop;
        DAL_CALL(alGetSourcei(_kID, AL_LOOPING, &loop));

        return loop != 0;
    }

    void KStaticSource::attachBuffer(const KSoundBuffer &Buffer){
        DAL_CALL(alSourcei(_kID, AL_BUFFER, Buffer.getID()));
    }

    void KStaticSource::deattachBuffer(){
        stop();
        DAL_CALL(alSourcei(_kID, AL_BUFFER, 0));
    }

    void KStaticSource::setPlayOffset(U32 Second){
        DAL_CALL(alSourcef(_kID, AL_SEC_OFFSET, Second));
    }
}
