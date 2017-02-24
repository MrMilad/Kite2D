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
#include "Kite/ecs/kcomponent.h"
#include "Kite/graphic/kcullable.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/math/kmathstructs.h"
#include <kgcullingcom.khgen.h>

KMETA
namespace Kite {
	KM_CLASS(COMPONENT)
	class KITE_FUNC_EXPORT KGCullingCom : public KComponent {
		friend class KGCullingSys;
		KM_INFO(KI_SHOW = false);
		KM_INFO(KI_NAME = "GCullingInstance");
		KMETA_KGCULLINGCOM_BODY();
	public:
		KGCullingCom(const std::string &Name = "");

		void attached(KNode *Owner) override;

		void deattached(KNode *Owner) override;

		RecieveTypes onMessage(KMessage *Message, MessageScope Scope) override;

		/// register cullable interface
		template<typename T>
		bool registerInterface(T *Component) {
			static_assert(std::is_base_of<KComponent, T>::value, "T must be a drived class from KComponent");
			static_assert(std::is_base_of<KCullable, T>::value, "T must be a drived class from KRenderable");

			if (_kihandle.getCType() == Component->getType()) return true;
			if (_kihandle.getCType() != CTypes::maxSize) {
				KD_PRINT("override interface detected");
				return false;
			}
			if (Component->getOwnerNode() != getOwnerNode()) {
				KD_PRINT("owner missmatch");
				return false;
			}
			_kihandle = Component->getHandle();
			return true;
		}

		template<typename T>
		bool unregisterInterface(T *Component) {
			static_assert(std::is_base_of<KComponent, T>::value, "T must be a drived class from KComponent");
			static_assert(std::is_base_of<KCullable, T>::value, "T must be a drived class from KCullable");

			if (_kihandle.getCType() == CTypes::maxSize) return true;
			if (Component->getOwnerNode() != getOwnerNode()) {
				KD_PRINT("owner missmatch");
				return false;
			}
			if (_kihandle.getCType() != Component->getType()) {
				KD_PRINT("component type missmatch");
				return false;
			}

			_kihandle = KHandle();
			return true;
		}

		KM_PRO_GET(KP_NAME = "interfaceHandle", KP_TYPE = KHandle)
			inline const KHandle &getInterfaceHandle() const { return _kihandle; }

	private:
		KM_VAR() KHandle _kihandle; 
		// runtime variables
		static void(*_kswitchCallb)(KNode *, KGCullingCom *); // static/dynamic switch callback
		static void *_ksysptr; // opaque pointer
		void(*_kcleanCallb)(KGCullingCom *); // cleanup callback
		void *_kobjptr;
		U32 _kobjIndex;
	};
}

#endif // KGCULLINGCOM_H
