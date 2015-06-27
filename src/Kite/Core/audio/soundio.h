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
#ifndef SOUNDIO_H
#define SOUNDIO_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/audio/kaudiotypes.h"
#include "Kite/Core/audio/kaudiostructs.h"
#include "src/Kite/Core/audio/audioformatio.h"

namespace Kite{
namespace Internal{
    class SoundIO{
    public:
        SoundIO();
        ~SoundIO();

        /// open file for reading
        bool openFile(const char *FileName, KAudioFileTypes FileTypes);

		/// read the PCM data from input stream
		bool openFile(KInputStream &InputStream, KAudioFileTypes FileTypes);

        /// read and decode data then fill buffer with data
        /// return: indicates actual number of bytes read
        I32 readData(void *Data, I32 Size);

        /// get information of audio file
        KAudioInfo getInfo() const;

        /// set current read offset of opened file
        bool setReadOffset(I32 Offset);

        /// get current read position of opened file
        I32 getReadOffset() const;

    private:
        AudioFormatIO *_kformat;
    };
}
}

#endif // SOUNDIO_H
