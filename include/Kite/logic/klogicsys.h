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
#ifndef KLOGICSYS_H
#define KLOGICSYS_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/ksystem.h"
#include "Kite/meta/kmetadef.h"
#include "klogicsys.khgen.h"

KMETA
namespace Kite {
	class KLogicCom;
	KM_CLASS(SYSTEM)
	class KITE_FUNC_EXPORT KLogicSys : public KSystem {
		KMETA_KLOGICSYS_BODY();
	public:
		bool update(F32 Delta, KEntityManager *EManager, KResourceManager *RManager);

		bool inite(void *Data);

	private:
		bool catchAndRegist(KLogicCom *Component, KResourceManager *RManager);
		bool initeComp(KEntity *Self, KLogicCom *Comp);
		bool startComp(KEntity *Self, KLogicCom *Comp);
		bool updateComp(F32 Delta, KEntity *Self, KLogicCom *Comp);
		lua_State *_klvm;
	};
}
#endif // KLOGICSYS_H