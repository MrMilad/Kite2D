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
#include "Kite/Core/utility/karchivestream.h"

namespace Kite{
	KArchiveStream::KArchiveStream(const std::string &ArchiveName):
		_karchive(ArchiveName),
		_kloadmemory(false)
	{}

	KArchiveStream::KArchiveStream(const void *Data, size_t Size):
		_kdata(Data),
		_ksize(Size),
		_kloadmemory(true)
	{}
	
	KInputStream *KArchiveStream::openRead(const std::string &Name){
		if (!_karchive.empty()){
			KArchive *archive = new KArchive;
			bool ret = false;

			if (_kloadmemory){
				ret = archive->openArchive(_kdata, _ksize);
			}else{
				ret = archive->openArchive(_karchive);
			}

			if (ret){
				if (archive->openFile(Name)){
					return archive;
				}
			}
			delete archive;
		}

		KDEBUG_PRINT("error in archive stream");
		return 0;
	}

	const std::string &KArchiveStream::getArchiveName() const{
		return _karchive;
	}
}