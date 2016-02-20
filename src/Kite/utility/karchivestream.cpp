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
	void KArchiveStream::openArchive(const std::string &ArchiveName){
		_karchive = ArchiveName;
		_kloadmemory = false;
	}

	void KArchiveStream::openArchive(const void *Data, size_t Size){
		_kdata = Data;
		_ksize = Size;
		_kloadmemory = true;
	}
	
	KIStream *KArchiveStream::openRead(const std::string &Name){
		if (!_karchive.empty()){
			KArchiveIStream *archive = new KArchiveIStream;
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

	KOStream *KArchiveStream::openWrite(const std::string &Name, KIOTypes Mode) {
		if (_kloadmemory) {
			KDEBUG_PRINT("memory archive for write operation not supported");
			return 0;
		}

		if (Mode != KIO_WRITE && Mode != KIO_WRITE_APPEND) {
			KDEBUG_PRINT("invalid write mode");
			return 0;
		}

		if (!_karchive.empty()) {
			KDEBUG_PRINT("empty archive");
			return 0;
		}

		KArchiveOStream *archive = new KArchiveOStream;
		bool ret = false;

		ret = archive->openArchive(_karchive, Mode);

		if (ret) {
			if (archive->openFile(Name)) {
				return archive;
			}
		}

		delete archive;
		return 0;
	}
}