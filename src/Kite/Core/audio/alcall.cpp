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
#include "src/Kite/core/audio/alcall.h"

namespace Kite{
namespace Internal{

    ALDevice _kaldevice(NULL);

    bool checkLastALErr(){
        // get the last OpenAL error
        ALenum OALErrCode = alGetError();
        bool ret = true;

        if (OALErrCode != AL_NO_ERROR){
            ret = false;

            // oal error code
            switch (OALErrCode){
                case AL_INVALID_NAME:
                    KDEBUG_PRINT("AL_INVALID_NAME");
                    break;

                case AL_INVALID_ENUM:
                    KDEBUG_PRINT("AL_INVALID_ENUM");
                    break;

                case AL_INVALID_VALUE:
                    KDEBUG_PRINT("AL_INVALID_VALUE");
                    break;

                case AL_INVALID_OPERATION:
                    KDEBUG_PRINT("AL_INVALID_OPERATION");
                    break;

                case AL_OUT_OF_MEMORY:
                    KDEBUG_PRINT("AL_OUT_OF_MEMORY");
                    break;

                default:
                    KDEBUG_PRINT("AL_UNKNOWN_ERROR");
                    break;
            }
        }
        return ret;
    }

    ALenum getFormat(U16 ChannelCount, U16 BitsPerSample){

        // find the good format according to the number of channels
        // MONO
        if (ChannelCount == 1){
            switch(BitsPerSample){
            case 4: return alGetEnumValue("AL_FORMAT_MONO_IMA4");
            case 8: return alGetEnumValue("AL_FORMAT_MONO8");
            case 16: return alGetEnumValue("AL_FORMAT_MONO16");
            }
        // STEREO
        }else if (ChannelCount == 2){
            switch (BitsPerSample){
            case 4: return alGetEnumValue("AL_FORMAT_STEREO_IMA4");
            case 8: return alGetEnumValue("AL_FORMAT_STEREO8");
            case 16: return alGetEnumValue("AL_FORMAT_STEREO16");
            }
        }else{
            switch (ChannelCount){
            case 4: return alGetEnumValue("AL_FORMAT_QUAD16");
            case 6: return alGetEnumValue("AL_FORMAT_51CHN16");
            case 7: return alGetEnumValue("AL_FORMAT_61CHN16");
            case 8: return alGetEnumValue("AL_FORMAT_71CHN16");
            default : return 0;
            }
        }
        return 0;
    }


    ALDevice::ALDevice(const char *DeviceName){
        // open\create device (if DeviceName = NULL then oal select the preferred (default) device)
        DAL_CALL(_kdevice = alcOpenDevice(DeviceName));

        // create context
        DAL_CALL(_kcontext = alcCreateContext(_kdevice, NULL));

        // set context as current
        DAL_CALL(alcMakeContextCurrent(_kcontext));
    }

    ALDevice::~ALDevice(){
        // destroy the context
        alcMakeContextCurrent(NULL);
        if (_kcontext)
            alcDestroyContext(_kcontext);

        // destroy the device
        if (_kdevice)
            alcCloseDevice(_kdevice);
    }
}
}
