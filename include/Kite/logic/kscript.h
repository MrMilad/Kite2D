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
#include "Kite/ecs/kresourcemanager.h"
#include "Kite/meta/kmetadef.h"
#include <string>
#include "kscript.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(RESOURCE)
	class KITE_FUNC_EXPORT KScript : public KResource {
		KM_INFO(KI_NAME = "Script");
		KSCRIPT_BODY();
	public:

		KM_FUN()
		bool saveStream(KIOStream &Stream, const std::string &Address) override;

		KM_PRO_SET(KP_NAME = "code")
		inline void setCode(const std::string &Code) { _kcode = Code;}

		KM_PRO_GET(KP_NAME = "code", KP_TYPE = std::string, KP_CM = "script code")
		inline const std::string &getCode() const { return _kcode; }

	private:
		KScript(const std::string &Name, const std::string &Address);
		bool _loadStream(std::unique_ptr<KIOStream> Stream, KResourceManager *RManager) override;
		std::string _kcode;
	};
}

#endif // KSCRIPT_H