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
#ifndef KARCHIVESTREAM_H
#define KARCHIVESTREAM_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/utility/kstream.h"
#include "Kite/Core/utility/karchive.h"

namespace Kite{
	class KITE_FUNC_EXPORT KArchiveStream : public KStream{
	public:
		// open from file
		KArchiveStream(const std::string &ArchiveName);

		// open from memory
		KArchiveStream(const void *Data, size_t Size);

		KInputStream *openRead(const std::string &Name);

		const std::string &getArchiveName() const;

	private:
		std::string _karchive;
		const void *_kdata;
		rsize_t _ksize;
		bool _kloadmemory;
	};
}

#endif // KARCHIVESTREAM_H