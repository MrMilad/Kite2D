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
#ifndef KARCHIVE_H
#define KARCHIVE_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/kinputstream.h"
#include "Kite/Core/utility/kutilitystructs.h"
#include <cstring>

namespace Kite{
	namespace Internal{
		class ZipIO;
	}
	class KITE_FUNC_EXPORT KArchive : public KInputStream{
	public:
		KArchive();
		~KArchive();

		bool openArchive(const std::string &ArchiveName);

		bool openArchive(const void *Memory, size_t Size);

		U32 getFilesNumber();

		I32 searchFile(const std::string &FileName);

		bool getFileInformation(U32 FileIndex, KArchiveFileInfo &FileInfo);

		bool openFile(const std::string &FileName);

		U64 read(void *Data, U64 DataSize);

		I32 seek(I64 Offset, I32 Origin);

		I64 tell();

		bool isArchiveOpen() const;

		bool isOpen();

		// returns nonzero if we are at end of file/data
		I32 eof();

		I32 close();

	private:
		Internal::ZipIO *_kzipIO;
	};
}

#endif // KARCHIVE_H