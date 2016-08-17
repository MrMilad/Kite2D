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
#ifndef KPREFAB_H
#define KPREFAB_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kresource.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/serialization/kbinaryserial.h"
#include <string>
#include "kprefab.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(RESOURCE, SCRIPTABLE)
	class KITE_FUNC_EXPORT KPrefab : public KResource {
		friend class KEntityManager;
		KMETA_KPREFAB_BODY();
	public:
		KM_CON(std::string)
		KPrefab(const std::string &Name);
		~KPrefab();

		bool inite() override;

		KM_FUN()
		void clear();

		KM_FUN()
		inline const std::string &getCode() const { return _kcode; }

		inline bool isEmpty() const { return _kisempty; }
	private:
		bool _saveStream(KOStream &Stream, const std::string &Address) override;

		bool _loadStream(KIStream &Stream, const std::string &Address) override;

		void initeLoad();
		bool _kisempty;
		std::string _kcode;
		KBinarySerial _kdata;
	};
}

#endif // KPREFAB_H