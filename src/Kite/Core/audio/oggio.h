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
#ifndef OGGIO_H
#define OGGIO_H

#include <cstdio>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include "src/Kite/Core/audio/audioformatio.h"

namespace Kite{
namespace Internal{
    class OGGIO : public AudioFormatIO{
    public:
		OGGIO();
        ~OGGIO();

        /// open file for reading
        bool openFile(const char *FileName);

		/// read the PCM data from input stream
		bool openFile(KInputStream &InputStream);

        /// read and decode data then fill buffer with data
        /// return: indicates actual number of bytes read
        I32 readData(void *Data, I32 Size);

        /// set current read offset of opened file
        bool setReadOffset(I32 Offset);

    private:
		static size_t _read(void *ptr, size_t size, size_t nmemb, void *datasource);
		static int _seek(void *datasource, ogg_int64_t offset, int whence);
		static int _close(void *datasource);
		static long _tell(void *datasource);
        bool _kisOpen;
        OggVorbis_File _koggStream;    // stream handle
        vorbis_info *_koggInfo;      // some formatting data
		ov_callbacks _kcallb;
    };
}
}
#endif // OGGIO_H
