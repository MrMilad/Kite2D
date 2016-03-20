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
#ifndef KINPUTCOM_H
#define KINPUTCOM_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcomponent.h"

namespace Kite {
	class KITE_FUNC_EXPORT KInputCom : public KComponent {
	public:
		KInputCom(const std::string &Name);

		void attached(U32 EntityID) override;

		void deattached(U32 EntityID) override;

		inline void setEnableKeyboard(bool Enable) { _kkeyboard = Enable; }
		inline bool getEnableKeyboard() const { return _kkeyboard; }

		inline void setEnableMouse(bool Enable) { _kmouse = Enable; }
		inline bool getEnableMouse() const { return _kmouse; }

		inline void setEnableJoystick(bool Enable) { _kjoystick = Enable; }
		inline bool getEnableJoystick() const { return _kjoystick; }

	private:
		bool _kkeyboard;
		bool _kmouse;
		bool _kjoystick;
	};
}

#endif // KINPUTCOM_H