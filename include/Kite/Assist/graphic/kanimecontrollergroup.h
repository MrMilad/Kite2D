/*
Kite2D Game Framework.
Copyright (C) 2010-2014  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef KANIMECONTROLLERGROUP_H
#define KANIMECONTROLLERGROUP_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Assist/graphic/kanimecontroller.h"
#include <vector>

namespace Kite{
	class KITE_FUNC_EXPORT KAnimeControllerGroup{
	public:
		void addObject(KAnimeController *Controller);

		void removeAllObjects();

		// set group loop
		// loop apply at any direction (foreward and backwrad)
		void setLoop(bool Loop);

		// set group state (play, pause, stop)
		void setState(KAnimeStateTypes State);

		// update animation state
		void update(F32 Delta);

	private:
		std::vector<KAnimeController *> _klist;
	};
}

#endif // KANIMECONTROLLERGROUP_H