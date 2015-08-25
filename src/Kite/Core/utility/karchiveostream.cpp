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
#include "Kite/Core/utility/karchiveostream.h"
#include "src/Kite/Core/utility/zipio.h"

namespace Kite {
	KArchiveOStream::KArchiveOStream() :
		_kzipIO(new Internal::ZipIO),
		_kcomp(false)
		{}

	KArchiveOStream::~KArchiveOStream() {
		delete _kzipIO;
	}

	bool KArchiveOStream::openArchive(const std::string &ArchiveName, KIOTypes Mode) {
		if (Mode == KIO_WRITE || Mode == KIO_WRITE_APPEND)
			return _kzipIO->openArchive(ArchiveName, Mode);

		return false;
	}

	const std::string &KArchiveOStream::getArchiveName() const {
		return _kzipIO->getArchiveName();
	}


	bool KArchiveOStream::openFile(const std::string &FileName) {
		return _kzipIO->openFile(FileName);
	}

	bool KArchiveOStream::write(void *Data, U64 DataSize) {
		return _kzipIO->writeFile(Data, DataSize, _kcomp);
	}

	const std::string &KArchiveOStream::getFileName() const {
		return _kzipIO->getFileName();
	}


	bool KArchiveOStream::isArchiveOpen() const {
		return _kzipIO->isArchiveOpen();
	}

	bool KArchiveOStream::isOpen() {
		return _kzipIO->isFileOpen();
	}

	I32 KArchiveOStream::close() {
		_kzipIO->closeArchive();
		return 0;
	}
}