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
#ifndef KSCENEMANAGER_H
#define KSCENEMANAGER_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kscene.h"
#include "Kite/core/kresourcemanager.h"
#include "Kite/meta/kmetadef.h"
#include <unordered_map>
#include <string>
#include <list>
#include "kmeta.khgen.h"
#include "kscenemanager.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KSceneManager {

		KMETA_KSCENEMANAGER_BODY();
	public:
		KSceneManager(KResourceManager &RMan);

		KM_FUN()
		bool loadScene(const std::string &Name);

		KM_FUN()
		void unloadScene(const std::string &Name);

		KM_FUN()
		KScene *getScene(const std::string &Name);

		KM_PRO_GET(KP_NAME = "activeScene", KP_TYPE = KScene, KP_CM = "get active scene")
		inline KScene *getActiveScene() { return _kactive; }

		KM_PRO_SET(KP_NAME = "activeScene")
		inline void setActiveScene(KScene *Scene) { _kactive = Scene; }

	private:
		void initeDefScene();
		KResourceManager *_krman;
		KScene *_kactive;
		KScene _kdef;			// default scene (Kite2D Intro!)
	};
}

#endif // KSCENEMANAGER_H