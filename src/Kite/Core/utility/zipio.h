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

		inline const std::string &getArchiveName() const { return _kaname; }

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

		inline const std::string &getFileName() const { return _kfname; }

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

		U64 getStreamSize() const;

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
		std::string _kaname;
		std::string _kfname;
	};
}
}

#endif // ZIPIO_H