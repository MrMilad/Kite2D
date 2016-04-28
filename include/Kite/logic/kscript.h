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
#ifndef KSCRIPT_H
#define KSCRIPT_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kresource.h"
#include "Kite/meta/kmetadef.h"
#include <string>
#include "kscript.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(RESOURCE)
	class KITE_FUNC_EXPORT KScript : public KResource {
		KMETA_KSCRIPT_BODY();
	public:
		KScript(const std::string &Name);

		bool loadStream(KIStream *Stream, U32 Flag = 0, const std::string &Key = std::string()) override;

		bool saveStream(KOStream *Stream, U32 Flag = 0, const std::string &Key = std::string()) override;

		inline void setCode(const std::string &Code) { _kcode = Code; }

		inline const std::string &getCode() const { return _kcode; }

	private:
		std::string _kcode;
	};
}

#endif // KSCRIPT_H