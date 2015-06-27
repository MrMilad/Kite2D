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
#include "src/Kite/Core/audio/oggio.h"

namespace Kite{
namespace Internal{
    OGGIO::OGGIO():
        _kisOpen(false),
        _koggInfo(0)
    {}

    OGGIO::~OGGIO(){
        if (_kisOpen)
            ov_clear(&_koggStream);
    }

    bool OGGIO::openFile(const char *FileName){
        if (_kisOpen){
            ov_clear(&_koggStream);
            _kinfo = KAudioInfo();
            _kreadPos = 0;
            _kisOpen = false;
        }

        if (!FileName){
            KDEBUG_PRINT("invalid file name");
            return false;
        }

        // open stream    
        if(ov_fopen(FileName, &_koggStream) < 0){
            KDEBUG_PRINT("could not open ogg stream.");
            return false;
        }

        // get vorbis info
        if(!(_koggInfo = ov_info(&_koggStream, -1))){
            KDEBUG_PRINT("could not retrieve vorbis info.");
            ov_clear(&_koggStream);
            return false;
        }

        _kinfo.channel = _koggInfo->channels;
        _kinfo.sampleRate = _koggInfo->rate;
        _kinfo.bitsPerSample = 16;
        _kinfo.bitRate = ov_bitrate(&_koggStream, -1);
		_kinfo.size = (I32)(ov_pcm_total(&_koggStream, -1) * sizeof(I16)* _kinfo.channel);
        _kinfo.totalTime = ov_time_total(&_koggStream, -1);

        if (ov_seekable(&_koggStream) == 0){
            _kinfo.isSeekable = false;
        }else{
            _kinfo.isSeekable = true;
        }
		
        _kisOpen = true;
        return true;
    }

	
	bool OGGIO::openFile(KInputStream &InputStream){
		if (_kisOpen){
			ov_clear(&_koggStream);
			_kinfo = KAudioInfo();
			_kreadPos = 0;
			_kisOpen = false;
		}

		_kcallb.read_func = _read;
		_kcallb.seek_func = _seek;
		_kcallb.tell_func = _tell;
		_kcallb.close_func = _close;

		// open stream    
		if (ov_open_callbacks((void *) &InputStream, &_koggStream, NULL, 0, _kcallb) < 0){
			KDEBUG_PRINT("could not open ogg stream.");
			return false;
		}

		// get vorbis info
		if (!(_koggInfo = ov_info(&_koggStream, -1))){
			KDEBUG_PRINT("could not retrieve vorbis info.");
			ov_clear(&_koggStream);
			return false;
		}

		_kinfo.channel = _koggInfo->channels;
		_kinfo.sampleRate = _koggInfo->rate;
		_kinfo.bitsPerSample = 16;
		_kinfo.bitRate = ov_bitrate(&_koggStream, -1);
		_kinfo.size = (I32)(ov_pcm_total(&_koggStream, -1) * sizeof(I16)* _kinfo.channel);
		_kinfo.totalTime = ov_time_total(&_koggStream, -1);

		if (ov_seekable(&_koggStream) == 0){
			_kinfo.isSeekable = false;
		}
		else{
			_kinfo.isSeekable = true;
		}

		_kisOpen = true;
		return true;
	}

    I32 OGGIO::readData(void *Data, I32 Size){
        // check requested size
        if (Size == 0 || !_kisOpen){
            return 0;
        }

        // default set little endian
        int endian = 0;
#ifdef KITE_ENDIAN_BIG
        endian = 1;
#endif
        I32 pos = 0;
		I32 read = 0;

		while (pos < Size){
			read = ov_read(&_koggStream, (char *)Data + pos, (Size - pos), endian, 2, 1, &_kreadPos);
			pos += read;
		}
		return pos;
    }

    bool OGGIO::setReadOffset(I32 Offset){
        if (!_kisOpen){
            return false;
        }

        if (!_kinfo.isSeekable){
            return false;
        }

        // calculate offset in PCM
        _kreadPos = (Offset / sizeof(I32));
        ov_pcm_seek(&_koggStream, (Offset / sizeof(I32)));
        return true;
    }

	size_t OGGIO::_read(void *ptr, size_t size, size_t nmemb, void *datasource){
		KInputStream *stream = (KInputStream *)datasource;
		return stream->read(ptr, size * nmemb) / size;
	}

	int OGGIO::_seek(void *datasource, ogg_int64_t offset, int whence){
		KInputStream *stream = (KInputStream *)datasource;
		return stream->seek(offset, whence);
	}

	int OGGIO::_close(void *datasource){
		KInputStream *stream = (KInputStream *)datasource;
		return stream->close();
	}

	long OGGIO::_tell(void *datasource){
		KInputStream *stream = (KInputStream *)datasource;
		return stream->tell();
	}
}
}
