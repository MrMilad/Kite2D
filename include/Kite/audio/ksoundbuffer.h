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
#ifndef KSOUNDBUFFER_H
#define KSOUNDBUFFER_H

#include "Kite/system/ksystemdef.h"
#include <string>

namespace Kite{
    class KITE_FUNC_EXPORT KSoundBuffer{
    public:
        KSoundBuffer();
        ~KSoundBuffer();

        // load sound file
        void load(const std::string &FileName);

        inline UL32 getSampleRate() const {return _ksampleRate;}
        inline U16 getChannelCount() const {return _kchannelCount;}
        inline U16 getBitsPerSample() const {return _kbitsPerSample;}
        inline UL32 getSize() const {return _ksize;}
        inline U32 getID() const {return _kID;}

    private:
        UL32 _ksampleRate;
        U16 _kchannelCount;
        U16 _kbitsPerSample;
        UL32 _ksize;
        U32 _kID;
    };
}

#endif // KSOUNDBUFFER_H
