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
#ifndef KSYSTEM_H
#define KSYSTEM_H

#include "Kite/core/kcoredef.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/ecs/knode.h"
#include "ksystem.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(SYSTEM, ABSTRACT)
	class KITE_FUNC_EXPORT KSystem{
		KMETA_KSYSTEM_BODY();
	public:
		KSystem();

		virtual ~KSystem();

		// delta is based seconds
		virtual bool update(F64 Delta, KNode *Scene) = 0;

		virtual bool inite(KSysInite *IniteData) = 0;

		virtual void destroy() = 0;

		inline bool isInite() const { return _kisinite; }

	protected:
		inline void setInite(bool Inite) { _kisinite = Inite; }

	private:
		bool _kisinite;
	};
}

#endif // KSYSTEM_H