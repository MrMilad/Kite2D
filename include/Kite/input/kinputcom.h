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
#include "Kite/meta/kmetadef.h"
#include "kinputcom.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(COMPONENT)
	class KITE_FUNC_EXPORT KInputCom : public KComponent {
		KM_INFO(KI_CTYPE = "Input");
		KMETA_KINPUTCOM_BODY();
	public:
		KInputCom(const std::string &Name = "");

		void attached() override;

		void deattached() override;

		RecieveTypes onMessage(KMessage *Message, MessageScope Scope) override ;

		KM_PRO_SET(KP_NAME = "Keyboard")
		inline void setEnableKeyboard(bool Enable) { _kkeyboard = Enable; }

		KM_PRO_GET(KP_NAME = "Keyboard", KP_TYPE = bool, KP_CM = "enable keyboard events")
		inline bool getEnableKeyboard() const { return _kkeyboard; }

		KM_PRO_SET(KP_NAME = "Mouse")
		inline void setEnableMouse(bool Enable) { _kmouse = Enable; }

		KM_PRO_GET(KP_NAME = "Mouse", KP_TYPE = bool, KP_CM = "enable mouse events")
		inline bool getEnableMouse() const { return _kmouse; }

		KM_PRO_SET(KP_NAME = "Joystick")
		inline void setEnableJoystick(bool Enable) { _kjoystick = Enable; }

		KM_PRO_GET(KP_NAME = "Joystick", KP_TYPE = bool, KP_CM = "enable joystick events")
		inline bool getEnableJoystick() const { return _kjoystick; }

	private:
		KM_VAR() bool _kkeyboard;
		KM_VAR() bool _kmouse;
		KM_VAR() bool _kjoystick;
	};
}

#endif // KINPUTCOM_H