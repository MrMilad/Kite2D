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
#include "src/Kite/Core/audio/soundio.h"
#include "src/Kite/Core/audio/oggio.h"

namespace Kite{
namespace Internal{

    SoundIO::SoundIO():
        _kformat(0)
    {}

    SoundIO::~SoundIO(){
        delete _kformat;
    }

    bool SoundIO::openFile(const char *FileName, KAudioFileTypes FileTypes){
        delete _kformat;
        switch (FileTypes){
        case KAF_OGG:
            _kformat = new OGGIO();
            return _kformat->openFile(FileName);
            break;
        default:
            KDEBUG_PRINT("invalid or not supportd file type");
            break;
        }
		return false;
    }

	bool SoundIO::openFile(KInputStream &InputStream, KAudioFileTypes FileTypes){
		delete _kformat;
		switch (FileTypes){
		case KAF_OGG:
			_kformat = new OGGIO();
			return _kformat->openFile(InputStream);
			break;
		default:
			KDEBUG_PRINT("invalid or not supportd file type");
			break;
		}
		return false;
	}

    I32 SoundIO::readData(void *Data, I32 Size){
        if (!_kformat){
            KDEBUG_PRINT("open file first");
            return 0;
        }
        return _kformat->readData(Data, Size);
    }

    KAudioInfo SoundIO::getInfo() const{
        if (!_kformat){
            KDEBUG_PRINT("open file first");
            return 0;
        }
        return _kformat->getInfo();
    }

    bool SoundIO::setReadOffset(I32 Offset){
        if (!_kformat){
            KDEBUG_PRINT("open file first");
            return false;
        }
        return _kformat->setReadOffset(Offset);
    }

    I32 SoundIO::getReadOffset() const{
        if (!_kformat){
            KDEBUG_PRINT("open file first");
            return 0;
        }
        return _kformat->getReadOffset();
    }
}
}
