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
#ifndef KFOSTREAM_H
#define KFOSTREAM_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kostream.h"
#include "Kite/meta/kmetadef.h"
#include "kfostream.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(OSTREAM, SCRIPTABLE)
	class KITE_FUNC_EXPORT KFOStream : public KOStream {
		KMETA_KFOSTREAM_BODY();
	public:
		KFOStream();
		~KFOStream();

		KM_FUN()
		bool open(const std::string &Address, IOMode Type) override;

		SIZE write(void *Data, SIZE DataSize) override;

		KM_FUN()
		bool isOpen() override;

		KM_FUN()
		I32 close() override;

	private:
		FILE *_kfile;
	};
}

#endif // KFOSTREAM_H