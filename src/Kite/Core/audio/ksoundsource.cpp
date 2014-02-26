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
#include "Kite/core/audio/ksoundsource.h"
#include "src/Kite/core/audio/alcall.h"

namespace Kite{
    KSoundSource::KSoundSource(){
        DAL_CALL(alGenSources(1, &_kID));
    }

    KSoundSource::~KSoundSource(){
        DAL_CALL(alDeleteSources(1, &_kID));
    }

    void KSoundSource::setReleative(bool Releative){
        DAL_CALL(alSourcei(_kID, AL_SOURCE_RELATIVE, Releative));
    }

    bool KSoundSource::getReleative(){
        ALint relative;
        DAL_CALL(alGetSourcei(_kID, AL_SOURCE_RELATIVE, &relative));

        return relative != 0;
    }

    void KSoundSource::setPitch(F32 Pitch){
        DAL_CALL(alSourcef(_kID, AL_PITCH, Pitch));
    }

    F32 KSoundSource::getPitch(){
        ALfloat pitch;
        DAL_CALL(alGetSourcef(_kID, AL_PITCH, &pitch));

        return pitch;
    }

    void KSoundSource::setVolume(F32 Volume){
        DAL_CALL(alSourcef(_kID, AL_GAIN, Volume * 0.01f));
    }

    F32 KSoundSource::getVolume(){
        ALfloat gain;
        DAL_CALL(alGetSourcef(_kID, AL_GAIN, &gain));

        return gain * 100.f;
    }

    void KSoundSource::setRolloffFactor(F32 Factor){
        DAL_CALL(alSourcef(_kID, AL_ROLLOFF_FACTOR, Factor));
    }

    F32 KSoundSource::getRolloffFactor(){
        ALfloat factor;
        DAL_CALL(alGetSourcef(_kID, AL_ROLLOFF_FACTOR, &factor));

        return factor;
    }

    void KSoundSource::setMinDistance(F32 Distance){
        DAL_CALL(alSourcef(_kID, AL_REFERENCE_DISTANCE, Distance));
    }

    F32 KSoundSource::getMinDistance(){
        ALfloat distance;
        DAL_CALL(alGetSourcef(_kID, AL_REFERENCE_DISTANCE, &distance));

        return distance;
    }

    void KSoundSource::setPosition(KVector3F32 Position){
        DAL_CALL(alSource3f(_kID, AL_POSITION, Position.x, Position.y, Position.z));
    }

    KVector3F32 KSoundSource::getPosition(){
        KVector3F32 position;
        DAL_CALL(alGetSource3f(_kID, AL_POSITION, &position.x, &position.y, &position.z));

        return position;
    }

    KSourceStateTypes KSoundSource::getState(){
        ALint status;
        DAL_CALL(alGetSourcei(_kID, AL_SOURCE_STATE, &status));

        switch (status){
            case AL_INITIAL: return KSS_INTIAL;
            case AL_STOPPED: return KSS_STOPPED;
            case AL_PAUSED:  return KSS_PAUSED;
            case AL_PLAYING: return KSS_PLAYING;
        }
        return KSS_STOPPED;
    }
}
