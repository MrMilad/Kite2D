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
#include "Kite/core/audio/kstaticsource.h"
#include "src/Kite/core/audio/alcall.h"

namespace Kite{
    void KStaticSource::play(){
		if (getState() != KSS_PLAYING)
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
        DAL_CALL(alSourcei(_kID, AL_BUFFER, Buffer.getALID()));
    }

    void KStaticSource::deattachBuffer(){
        stop();
        DAL_CALL(alSourcei(_kID, AL_BUFFER, 0));
    }

    void KStaticSource::setPlayOffset(F32 Second){
        DAL_CALL(alSourcef(_kID, AL_SEC_OFFSET, Second));
    }
}
