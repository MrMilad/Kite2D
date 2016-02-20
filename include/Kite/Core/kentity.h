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
#ifndef KENTITY_H
#define KENTITY_H

#include "Kite/core/kcoredef.h"

#include "Kite/core/kcoretypes.h"
#include "Kite/core/kcomponent.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/serialization/kbaseserial.h"
#include <vector>
#include <kentity.khgen.h>

KMETA
namespace Kite {
	class KEntityManager;
	KMETA_CLASS(SCRIPTABLE, SERIALIZABLE)
	class KITE_FUNC_EXPORT KEntity : public KObject{
		friend KEntityManager;
	public:
		KMETA_CONSTRUCTURE()
		KEntity(U32 ID = 0);

		/// only non-script components
		KMETA_FUNCTION()
		void addComponentByType(KComponentTypes Type);

		/// all types of components
		KMETA_FUNCTION()
		void addComponent(KComponent *Component);

		/// only non-script components
		KMETA_FUNCTION()
		void removeComponentByType(KComponentTypes Type);

		/// only script component
		KMETA_FUNCTION()
		void removeComponentByName(const char *Name);

		/// all types of components
		KMETA_FUNCTION()
		void removeComponent(KComponent *Component);

		/// only non-script components
		KMETA_FUNCTION()
		KComponent *getComponentByType(KComponentTypes type);

		/// only script components
		KMETA_FUNCTION()
		KComponent *getComponentByName(const std::string &Name);

		/// only non-script components
		KMETA_FUNCTION()
		bool hasComponentByType(KComponentTypes type);

		/// only script components
		KMETA_FUNCTION()
		bool hasComponentByName(const std::string &Name);

		KMETA_PROPERTY("ID", "entity unique ID")
		inline U32 getID() const { return _kid; }

		KMETA_KENTITY_BODY();
	private:
		KMETA_VARIABLE(SERIALIZABLE) U32 _kid;															// entity unique id
		KMETA_VARIABLE(SERIALIZABLE) KComponent *_kfixedComp[(U8)KComponentTypes::KCT_MAX_COMP_SIZE]; 	// fixed components slots (built-in components)
		KMETA_VARIABLE(SERIALIZABLE) std::vector<KComponent *> _klogicComp;								// dynamic components slots (logic components)
	};
}

#endif // KENTITY_H