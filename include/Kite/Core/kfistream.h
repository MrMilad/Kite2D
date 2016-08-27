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
#ifndef KFISTREAM_H
#define KFISTREAM_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kistream.h"
#include "Kite/meta/kmetadef.h"
#include <cstring>
#include "kfistream.khgen.h"

KMETA
namespace Kite{
	KM_CLASS(ISTREAM, SCRIPTABLE)
	class KITE_FUNC_EXPORT KFIStream : public KIStream{
		KM_INFO(KI_NAME = "FIStream");
		KMETA_KFISTREAM_BODY();
	public:
		KM_CON()
		KFIStream();

		~KFIStream();

		KM_FUN()
		bool open(const std::string &Address, IOMode Type) override;

		SIZE read(void *Data, SIZE DataSize) override;

		I32 seek(U64 Offset, I32 Origin) override;

		I64 tell() override;

		I32 eof() override;

		// return binary size (if file opened  with BIN mode)
		KM_FUN()
		U64 getSize() override;

		KM_FUN()
		bool isOpen() override;

		KM_FUN()
		I32 close() override;

		KM_FUN()
		inline const KFileInfo *getFileInfo() override { return &_kfinfo; }

		KM_FUN()
		void getFileInfoStr(const std::string &Address, KFileInfo &FileInfo) override;

		KM_FUN()
		inline IOMode getIOMode() override { return _kio; }

	private:
		KFileInfo _kfinfo;
		IOMode _kio;
		FILE *_kfile;
	};
}

#endif // KFISTREAM_H