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
#include "Kite/Core/utility/karchiveistream.h"
#include "src/Kite/Core/utility/zipio.h"

namespace Kite{
	KArchiveIStream::KArchiveIStream() :
		_kzipIO(new Internal::ZipIO)
	{}

	KArchiveIStream::~KArchiveIStream() {
		delete _kzipIO;
	}

	bool KArchiveIStream::openArchive(const std::string &ArchiveName) {
		return _kzipIO->openArchive(ArchiveName, KIO_READ);
	}

	bool KArchiveIStream::openArchive(const void *Data, size_t Size) {
		return _kzipIO->openArchive(Data, Size);
	}

	const std::string &KArchiveIStream::getArchiveName() const {
		return _kzipIO->getArchiveName();
	}

	U32 KArchiveIStream::getFilesNumber() {
		return _kzipIO->getFilesNumber();
	}

	I32 KArchiveIStream::searchFile(const std::string &FileName) {
		return _kzipIO->searchFile(FileName);
	}

	bool KArchiveIStream::getFileInformation(U32 FileIndex, KArchiveFileInfo &FileInfo) {
		return _kzipIO->getFileInformation(FileIndex, FileInfo);
	}

	bool KArchiveIStream::openFile(const std::string &FileName) {
		return _kzipIO->openFile(FileName);
	}

	const std::string &KArchiveIStream::getFileName() const {
		return _kzipIO->getFileName();
	}

	U64 KArchiveIStream::read(void *Data, U64 DataSize) {
		return _kzipIO->readFile(Data, DataSize);
	}

	I32 KArchiveIStream::seek(I64 Offset, I32 Origin) {
		return _kzipIO->setReadOffset(Offset, Origin);
	}

	I64 KArchiveIStream::tell() {
		return _kzipIO->getReadOffset();
	}

	bool KArchiveIStream::isArchiveOpen() const {
		return _kzipIO->isArchiveOpen();
	}

	bool KArchiveIStream::isOpen() {
		return _kzipIO->isFileOpen();
	}

	I32 KArchiveIStream::eof() {
		return _kzipIO->eof();
	}

	U64 KArchiveIStream::getSize() {
		return _kzipIO->getStreamSize();
	}

	I32 KArchiveIStream::close() {
		_kzipIO->closeArchive();
		return 0;
	}
}