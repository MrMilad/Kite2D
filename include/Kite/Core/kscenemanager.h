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
#include "kscenemanager.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KSceneManager {

		KMETA_KSCENEMANAGER_BODY();
	public:
		KSceneManager(KResourceManager &RMan);

		/// S: stream type
		template<typename S>
		bool loadScene(const std::string &Name) {
			KScene *scene = _krman->load<KScene, S>(Name, false);
			if (scene == nullptr) {
				KD_FPRINT("can't load scene. sname: %s", Name.c_str());
				return false;
			}

			//scene->_
		}

		void unloadScene(const std::string &Name);

		KScene *getScene(const std::string &Name);

		inline KScene *getActiveScene() { return _kactive; }

		bool setActiveScene(const std::string &Name);

	private:
		KResourceManager *_krman;
		KScene *_kactive;
	};
}

#endif // KSCENEMANAGER_H