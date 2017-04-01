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
#ifndef KATLASTEXTURE_H
#define KATLASTEXTURE_H

#include "Kite/core/kcoredef.h"
#include "Kite/ecs/kresource.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/graphic/ktexture.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "katlastexture.khgen.h"
#include <vector>

/*! \namespace Kite
\brief Public namespace.
*/
KMETA
namespace Kite {

	KM_CLASS(RESOURCE)
	class KITE_FUNC_EXPORT KAtlasTexture : public KResource {
		KM_INFO(KI_NAME = "AtlasTexture");
		KATLASTEXTURE_BODY();
	public:
		KAtlasTexture(const std::string &Name, const std::string &Address);

		KM_FUN()
		bool saveStream(KOStream &Stream, const std::string &Address) override;

		KM_FUN()
		void addItem(const KAtlasItem &Item);

		KM_FUN()
		inline const KAtlasItem *getItem(U32 ID) const { return &_kitems.at(ID); }

		KM_FUN()
		inline void clearItems() { _kitems.clear(); }

		KM_PRO_GET(KP_NAME = "size", KP_TYPE = U32, KP_CM = "number of items")
		inline U32 getItemsSize() const { return _kitems.size(); }

		inline void setTexture(KTexture *Texture) { _ktexture = Texture; }

		KM_PRO_GET(KP_NAME = "texture", KP_CM = "texture", KP_RES = Resource::TEXTURE)
		inline KSharedResource getTexture() const { return _ktexture; }

#if defined(KITE_EDITOR)
		inline auto getContiner() { return &_kitems; }
#endif

	private:
		bool _loadStream(std::unique_ptr<KIStream> Stream, KResourceManager *RManager) override;

		std::vector<KAtlasItem> _kitems;
		KSharedResource _ktexture;
	};
}

#endif // KATLASTEXTURE_H
