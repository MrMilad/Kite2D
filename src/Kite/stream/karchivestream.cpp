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
#include "Kite/stream/karchivestream.h"
#include "src/Kite/stream/zipio.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include <luaintf\LuaIntf.h>

namespace Kite{
	KArchiveStream::KArchiveStream() :
		_kzipIO(new Internal::ZipIO)
	{}

	KArchiveStream::~KArchiveStream() {
		delete _kzipIO;
	}

	bool KArchiveStream::openRead(const std::string &Address, IOMode Type) {
		// resolve address
		auto found = Address.find(">", 0);
		if (found == std::string::npos) {
			KD_FPRINT("incorrect address: file name not found");
			return false;
		}
		_kinfo.path = Address.substr(0, found);
		_kinfo.name = Address.substr(found);
		_kinfo.fullPath = Address;

		// open archive
		if (!_kzipIO->openArchive(_kinfo.path, OpenMode::READ)) {
			return false;
		}

		// open file inside the archive
		if (!_kzipIO->openFile(_kinfo.name)) {
			return false;
		}

		return true;
	}

	bool KArchiveStream::openRead(const void *Data, size_t Size, const std::string &FileName) {
		// open archive
		if (!_kzipIO->openArchive(Data, Size)) {
			return false;
		}

		// open file inside the archive
		if (!_kzipIO->openFile(FileName)) {
			_kzipIO->closeArchive();
			return false;
		}

		return true;
	}

	bool KArchiveStream::openWrite(const std::string &Address, IOMode IOType, WriteMode WriteType) {
		// resolve address
		auto found = Address.find(">", 0);
		if (found == std::string::npos) {
			KD_FPRINT("incorrect address: file name not found");
			return false;
		}
		_kinfo.path = Address.substr(0, found);
		_kinfo.name = Address.substr(found);
		_kinfo.fullPath = Address;

		// open archive
		OpenMode mode = OpenMode::APPEND;
		if (WriteType == WriteMode::NEW) mode = OpenMode::WRITE;
		if (!_kzipIO->openArchive(_kinfo.path, mode)) {
			return false;
		}

		// open file inside the archive
		if (!_kzipIO->openFile(_kinfo.name)) {
			_kzipIO->closeArchive();
			return false;
		}

		return true;
	}

	const std::string &KArchiveStream::getArchiveName() const {
		return _kzipIO->getArchiveName();
	}

	U32 KArchiveStream::getFilesNumber() {
		return _kzipIO->getFilesNumber();
	}

	I32 KArchiveStream::searchFile(const std::string &FileName) {
		return _kzipIO->searchFile(FileName);
	}

	bool KArchiveStream::getArchiveFileInfo(U32 FileIndex, KArchiveFileInfo &FileInfo) {
		return _kzipIO->getFileInformation(FileIndex, FileInfo);
	}

	const std::string &KArchiveStream::getFileName() const {
		return _kzipIO->getFileName();
	}

	SIZE KArchiveStream::read(void *Data, SIZE DataSize) {
		return _kzipIO->readFile(Data, DataSize);
	}

	SIZE KArchiveStream::write(const void *Data, SIZE DataSize) {
		if (_kzipIO->writeFile(Data, DataSize, _kcomp)) {
			return DataSize;
		}

		KD_PRINT("archive writing error");
		return 0;
	}

	I32 KArchiveStream::seek(U64 Offset, I32 Origin) {
		return _kzipIO->setReadOffset(Offset, Origin);
	}

	I64 KArchiveStream::tell()const {
		return _kzipIO->getReadOffset();
	}

	bool KArchiveStream::isOpen() const {
		return _kzipIO->isFileOpen();
	}

	I32 KArchiveStream::eof() const{
		return _kzipIO->eof();
	}

	U64 KArchiveStream::getSize() const {
		return _kzipIO->getStreamSize();
	}

	I32 KArchiveStream::close() {
		_kzipIO->closeArchive();
		return 0;
	}

	KARCHIVESTREAM_SOURCE();
}