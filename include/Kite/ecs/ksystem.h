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
#include "Kite/core/kcorestructs.h"
#include "Kite/ecs/kecsstructs.h"
#include "Kite/meta/kmetadef.h"
#include "ksystem.khgen.h"
#include "ktypes.khgen.h"
#include <initializer_list>
#include <vector>
#include <string>

KMETA
namespace Kite {
	class KNode;
	class KComponent;
	struct KSysInite;
	KM_CLASS(SYSTEM, ABSTRACT)
	class KITE_FUNC_EXPORT KSystem{
		KM_INFO(KI_NAME = "SystemBase");
		KSYSTEM_BODY();
	public:
		/// interest components and interfaces
		KSystem();
		KSystem(const KSystem &Copy) = delete;
		KSystem &operator=(const KSystem &Right) = delete;

		virtual ~KSystem();

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "drivedType", KP_TYPE = stypes, KP_CM = "type of the drived system")
		virtual inline System getDrivedType() const = 0;

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "typeName", KP_TYPE = std::string, KP_CM = "name of the system's type")
		virtual inline const std::string &getTypeName() const = 0;

		virtual void reset(KNode *Hierarchy, KSysInite *Data) = 0;

		/// delta is based seconds
		virtual bool update(F64 Delta) = 0;

		virtual void nodeAdded(KNode *Node) = 0;

		virtual void nodeRemoved(KNode *Node) = 0;

		virtual void componentAdded(KComponent *Component) = 0;

		virtual void componentRemoved(KComponent * Component) = 0;
	};
}

#endif // KSYSTEM_H