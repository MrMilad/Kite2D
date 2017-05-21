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
#ifndef KATLASIMAGE_H
#define KATLASIMAGE_H

#include "Kite/core/kcoredef.h"
#include "Kite/ecs/kresource.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/graphic/kimage.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "katlasimage.khgen.h"
#include <vector>

/*! \namespace Kite
\brief Public namespace.
*/
KMETA
namespace Kite {

	KM_CLASS(RESOURCE)
	class KITE_FUNC_EXPORT KAtlasImage : public KResource {
		KM_INFO(KI_NAME = "AtlasImage");
		KATLASIMAGE_BODY();
	public:
		/// create atals from an image.
		/// this constructure will add an default atlas item fully matched with image size
		KAtlasImage(const KSharedResource &Image);

		KAtlasImage() = delete;
		KAtlasImage(const KAtlasImage &Copy) = delete;
		KAtlasImage &operator=(const KAtlasImage &Right) = delete;

		bool saveStream(KIOStream &Stream, const std::string &Address) override;

		/// return id of the added items
		KM_FUN()
		U32 addItem(const KAtlasItem &Item);

		KM_FUN()
		KAtlasItem getItem(U32 ID) const;

		KM_FUN()
		void clearItems();

		KM_PRO_GET(KP_NAME = "size", KP_TYPE = U32, KP_CM = "number of items")
		inline U32 getItemsSize() const { return _kitems.size(); }

		KM_PRO_GET(KP_NAME = "image", KP_RES = Resource::IMAGE)
		inline KSharedResource getImage() const { return _kimage; }

#if defined(KITE_EDITOR)
		inline auto getContiner() { return &_kitems; }
#endif

	private:
		KAtlasImage(const std::string &Name, const std::string &Address);

		KM_FUN(KP_NAME = "__call")
		static KSharedResource luaConstruct(const KSharedResource &Image);

		bool _loadStream(std::unique_ptr<KIOStream> Stream, KResourceManager *RManager) override;

		std::vector<KAtlasItem> _kitems;
		KSharedResource _kimage;
	};
}

#endif // KATLASIMAGE_H
