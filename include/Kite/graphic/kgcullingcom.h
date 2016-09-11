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
#ifndef KGCULLINGCOM_H
#define KGCULLINGCOM_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcomponent.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/math/kmathstructs.h"
#include <kgcullingcom.khgen.h>

KMETA
namespace Kite {
	KM_CLASS(COMPONENT)
	class KITE_FUNC_EXPORT KGCullingCom : public KComponent {
		friend class KGCullingSys;
		KM_INFO(KI_SHOW = false);
		KM_INFO(KI_NAME = "GCulling");
		KMETA_KGCULLINGCOM_BODY();
	public:
		KGCullingCom(const std::string &Name = "");

		void attached(KEntity *Owner) override;

		void deattached(KEntity *Owner) override;

		RecieveTypes onMessage(KMessage *Message, MessageScope Scope) override;

	private:
		// runtime variables 
		void(*_kcallb)(KGCullingCom *); // cleanup callback
		void *_ksysptr; // opaque pointer
		void *_kobjptr;
	};
}

#endif // KGCULLINGCOM_H
