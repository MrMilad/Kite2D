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
#ifndef AUDIOFORMATIO_H
#define AUDIOFORMATIO_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/audio/kaudiostructs.h"
#include "Kite/Core/system/kinputstream.h"

namespace Kite{
namespace Internal{
    class AudioFormatIO{
    public:
        AudioFormatIO():
            _kinfo(0),
            _kreadPos(0)
        {}

        virtual ~AudioFormatIO(){}

		/// read the PCM data from file on disk
        virtual bool openFile(const char *FileName) = 0;

		/// read the PCM data from input stream
		virtual bool openFile(KInputStream &InputStream) = 0;

        /// read and decode data then fill buffer with data
        /// return: indicates actual number of bytes read
        virtual I32 readData(void *Data, I32 Size) = 0;

        /// get information of audio file
        inline KAudioInfo getInfo() const {return _kinfo;}

        /// set current read offset of opened file
        virtual bool setReadOffset(I32 Offset) = 0;

        /// get current read offset of opened file
        inline I32 getReadOffset() const {return _kreadPos;}

    protected:
        KAudioInfo _kinfo;
        I32 _kreadPos;
    };
}
}

#endif // AUDIOFORMATIO_H
