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
#ifndef ZIPIO_H
#define ZIPIO_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/utility/kutilitystructs.h"
#include "extlibs/headers/zip/miniz.h"
#include <cstring>

namespace Kite{
namespace Internal{
	// stream operations (read, seek) only allowed for uncompressed files in the archive.
	class ZipIO{
	public:
		ZipIO();
		~ZipIO();

		// open from file
		bool openArchive(const std::string &ArchiveName);

		// open from memory
		bool openArchive(const void *Memory, size_t Size);

		// get total number of the files in the archive
		U32 getFilesNumber();

		// return -1 if file not found
		// else return the index of the file
		I32 searchFile(const std::string &FileName);

		// get information of the file in the archive
		bool getFileInformation(U32 FileIndex, KArchiveFileInfo &FileInfo);

		// open archived file by name
		bool openFile(const std::string &FileName);

		// open archived file by index
		bool openFile(U32 FileIndex);

		// read a portion of the file (stream - allowed only for uncompressed files in the archive)
		// read entire file with a single call (allowed for both compressed/uncompressed files in the archive)
		// note: to read a compressed file, the DataSize must be equal to or greater than the uncompressed size
		U64 readFile(void *Data, U64 DataSize);

		// set read offset (stream - allowed only for uncompressed files in the archive)
		I32 setReadOffset(I64 Offset, I32 Origin);

		// get read offset (stream - allowed only for uncompressed files in the archive)
		I64 getReadOffset();

		bool isArchiveOpen() const;

		bool isFileOpen() const;

		I32 eof() const;

		void closeArchive();

	private:
		// stream not allowed
		U64 _readCompressed(void *Data, U64 DataSize);
		// stream allowed
		U64 _readUncompressed(void *Data, U64 DataSize);
		mz_zip_archive _kzarchive;
		mz_zip_archive_file_stat _kfstat;
		U64 _kcurOfst;
		U64 _kfileCurOfst;
		U32 _kfindex;
		bool _kisopen;
		bool _kready;
		bool _kisCmprsd;
	};
}
}

#endif // ZIPIO_H