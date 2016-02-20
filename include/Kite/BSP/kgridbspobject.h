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
#ifndef KGRIDBSPOBJECT_H
#define KGRIDBSPOBJECT_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/math/kmathstructs.h"
#include "Kite/Core/utility/kutilitystructs.h"

namespace Kite {
	class KITE_FUNC_EXPORT KGridBSPObject {
		friend class KGridBSP;
	public:
		KGridBSPObject(I32 ObjectFlag = -1, U32 Layer = 0) :
			_kflag(ObjectFlag),
			_kcID(-1),
			_klayer(Layer)
		{}

		virtual ~KGridBSPObject() {}

		// ID of tile in the map
		inline I32 getCellID() const { return _kcID; }

		inline U32 getCellFlag() const { return _kflag; }

		inline void setLayer(U32 Layer) { _klayer = Layer; }
		inline U32 getLayer() const { return _klayer; }

		// get next object in the tile list
		inline KGridBSPObject *getNextObject() const { return (KGridBSPObject *)_klink.next; }

		virtual const KVector2F32 *getCellPosition() const = 0;

	protected:
		// custom flag
		inline void setCellFlag(U32 Flag) { _kflag = Flag; }

	private:
		U32 _kflag;
		I32 _kcID;
		U32 _klayer;
		Internal::KLinkedPoint _klink;
	};
}


#endif // KGRIDBSPOBJECT_H