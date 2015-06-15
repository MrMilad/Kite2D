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
#include "Kite/Core/utility/karchive.h"
#include "src/Kite/Core/utility/zipio.h"

namespace Kite{
	KArchive::KArchive():
		_kzipIO(new Internal::ZipIO)
	{}

	KArchive::~KArchive(){
		delete _kzipIO;
	}

	bool KArchive::openArchive(const std::string &ArchiveName){
		return _kzipIO->openArchive(ArchiveName);
	}

	bool KArchive::openArchive(const void *Memory, size_t Size){
		return _kzipIO->openArchive(Memory, Size);
	}

	U32 KArchive::getFilesNumber(){
		return _kzipIO->getFilesNumber();
	}

	I32 KArchive::searchFile(const std::string &FileName){
		return _kzipIO->searchFile(FileName);
	}

	bool KArchive::getFileInformation(U32 FileIndex, KArchiveFileInfo &FileInfo){
		return _kzipIO->getFileInformation(FileIndex, FileInfo);
	}

	bool KArchive::openFile(const std::string &FileName){
		return _kzipIO->openFile(FileName);
	}

	U64 KArchive::read(void *Data, U64 DataSize){
		return _kzipIO->readFile(Data, DataSize);
	}

	I32 KArchive::seek(I64 Offset, I32 Origin){
		return _kzipIO->setReadOffset(Offset, Origin);
	}

	I64 KArchive::tell(){
		return _kzipIO->getReadOffset();
	}

	bool KArchive::isArchiveOpen() const{
		return _kzipIO->isArchiveOpen();
	}

	bool KArchive::isOpen(){
		return _kzipIO->isFileOpen();
	}

	I32 KArchive::eof(){
		return _kzipIO->eof();
	}

	I32 KArchive::close(){
		_kzipIO->closeArchive();
		return 0;
	}
}