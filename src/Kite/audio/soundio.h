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
#ifndef SOUNDIO_H
#define SOUNDIO_H

#include "Kite/system/ksystemdef.h"
#include "Kite/system/knoncopyable.h"
#include <string>

#ifdef KITE_PLATFORM_WINDOWS
    #include "extlibs/headers/win32/sndfile.h"
#else
    #include "extlibs/headers/osx/sndfile.h"
#endif

namespace Kite{
namespace Internal{
    class SoundIO : public KNonCopyable{
    public:

        SoundIO();
        ~SoundIO();

        // open file for reading
        void openReadFromFile(const std::string& FileName);

        // open file for writing
        void openWriteToFile(const std::string& FileName, U32 ChannelCount, U32 SampleRate);

        // get parameter of opened file
        inline U32 getSampleRate() const {return _ksampleRate;}
        inline std::size_t getSampleCount() const {return _ksampleCount;}
        inline U32 getChannelCount() const {return _kchannelCount;}

        // read from opened file
        std::size_t read(I16* Data, std::size_t SampleCount);

        // write to opened file
        void write(const I16 *Data, std::size_t SampleCount);

        // seek
        void seek(U32 Second);
    private:
        // convert a string to lower case
        std::string toLower(std::string &str);

        // extract file format
        I32 getFileFormat(const std::string& FileName);

        SNDFILE * _kfile;
        std::size_t _ksampleCount;
        U32 _kchannelCount;
        U32 _ksampleRate;
    };
}
}
#endif // SOUNDIO_H
