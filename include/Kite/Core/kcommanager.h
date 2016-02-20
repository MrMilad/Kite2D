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
#ifndef KCOMMANAGER_H
#define KCOMMANAGER_H

#include "Kite/core/kcoredef.h"
#include "kite/memory/kbasestorage.h"
#include "Kite/core/kcomponent.h"
#include "Kite/core/kgameplaytypes.h"
#include <vector>

KMETA
namespace Kite {
	KMETA_CLASS()
	class KITE_FUNC_EXPORT KComManager {
	public:
		KComManager(KBaseStorage &Storage);

		KComponent *createComponent();

	private:
		std::vector<KComponent> _kcomponents[(U8)KComponentTypes::KCT_MAX_COMP_SIZE];
		KBaseStorage *_kbaseAlloc;
	};
}

#endif // KCOMMANAGER_H
