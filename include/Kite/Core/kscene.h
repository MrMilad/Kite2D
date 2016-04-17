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
#ifndef KSCENE_H
#define KSCENE_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kentitymanager.h"
#include "Kite/core/kresourcemanager.h"
#include "Kite/core/kresource.h"
#include "Kite/core/kostream.h"
#include "Kite/meta/kmetadef.h"
#include <string>
#include <unordered_map>
#include "kscene.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(RESOURCE)
	class KITE_FUNC_EXPORT KScene : public KResource {
		friend class KSceneManager;

		// KM_INFO("CatchStream", "false"); // false by default
		KM_INFO("RType", "Scene");
		KMETA_KSCENE_BODY();
	public:
		KScene(const std::string &Name);
		~KScene();

		/// load from an open stream
		bool loadStream(KIStream &Stream, U32 Flag = 0) override;

		/// save to an open stream
		bool saveStream(KOStream &Stream);

		inline bool isLoaded() const { return _kloaded; }
		inline bool isModified() { if (isLoaded()) return _keman.isModified(); return false; }
		inline auto &getEManager() { return _keman; }
		inline const auto &getName() { return _kname; }

		bool addResource(const std::string &RName, const std::string &RType, U32 Flag = 0);

		void removeResource(const std::string &RName);

		inline auto beginResource() { return _kres.begin(); }
		inline auto endResource() { return _kres.end(); }

	private:
		inline void setName(const std::string &Name) { _kname = Name; }

		std::string _kname;
		std::unordered_map<std::string, std::pair<std::string, U32>> _kres; // <name, <type, flag>>
		KEntityManager _keman;
		bool _kloaded;
	};
}


#endif // KSCENE_H