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
#ifndef KATLASTEXTUREARRAY_H
#define KATLASTEXTUREARRAY_H

#include "Kite/core/kcoredef.h"
#include "Kite/ecs/kresource.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/graphic/kgraphictypes.h"
#include "Kite/graphic/katlasimage.h"
#include "katlastexturearray.khgen.h"
#include <map>

/*! \namespace Kite
\brief Public namespace.
*/
KMETA
namespace Kite {

	KM_CLASS(RESOURCE)
	class KITE_FUNC_EXPORT KAtlasTextureArray : public KResource {
		KM_INFO(KI_NAME = "AtlasTextureArray");
		KATLASTEXTUREARRAY_BODY();
	public:
		KAtlasTextureArray(const KSharedResource &AtlasImage);
		~KAtlasTextureArray();

		KAtlasTextureArray() = delete;
		KAtlasTextureArray(const KAtlasTextureArray &Copy) = delete;
		KAtlasTextureArray &operator=(const KAtlasTextureArray &Right) = delete;

		bool saveStream(KIOStream &Stream, const std::string &Address) override;

		KM_PRO_GET(KP_NAME = "width", KP_TYPE = U32)
		inline U32 getWidth() const { return _ksize.x; }

		KM_PRO_GET(KP_NAME = "height", KP_TYPE = U32)
		inline U32 getHeight() const { return _ksize.y; }

		KM_PRO_GET(KP_NAME = "size", KP_TYPE = KVector2U32)
		inline const KVector2U32 &getSize() const { return _ksize; }

		/// return item id
		/// return 0 if there is no image attached to atlas
		/// return 0 if atlas image size is not correct size
		KM_FUN()
		U32 addItem(const KSharedResource &AtlasImage);

		/// 0 reserved for default item
		KM_FUN()
		KSharedResource getItem(U32 ID) const;

		/// clear all items but default
		KM_FUN()
		void clearItems();

		KM_PRO_GET(KP_NAME = "itemSize", KP_TYPE = U32)
		inline U32 getItemSize() const { return _kimageArray.size(); }

		KM_PRO_GET(KP_NAME = "textureFilter", KP_TYPE = TextureFilter)
		inline TextureFilter getFilter() const { return _kfilter; }

		KM_PRO_GET(KP_NAME = "textureWrap", KP_TYPE = TextureWrap)
		inline TextureWrap getWrap() const { return _kwrap; }

		KM_PRO_GET(KP_NAME = "glID", KP_TYPE = U32)
		inline U32 getGLID() const { return _ktexId; }

		KM_PRO_SET(KP_NAME = "textureFilter")
		void setFilter(TextureFilter Filter);

		KM_PRO_SET(KP_NAME = "textureWrap")
		void setWrap(TextureWrap Wrap);

		KM_PRO_SET(KP_NAME = "cleanUpImage")
		void setCleanupImage(bool CleanUp);

		KM_PRO_GET(KP_NAME = "cleanUpImage", KP_TYPE = bool, KP_CM = "clean up atlas image after creating ogl texture")
		inline bool getCleanUpImage() const { return _kclearImage; }

		/// opengl texture will created in the first time bind
		bool bind();

		void unbind();

		//! Unbind current texture
		static void unbindTextureArray();

#if defined(KITE_EDITOR)
		inline auto getContiner() { return &_kimageArray; }
#endif

	private:
		KAtlasTextureArray(const std::string &Name, const std::string &Address);

		KM_FUN(KP_NAME = "__call")
		static KSharedResource luaConstruct(const KSharedResource &AtlasImage);

		bool _loadStream(std::unique_ptr<KIOStream> Stream, KResourceManager *RManager) override;

		//! create the texture (opengl)
		bool _create();

		bool _kclearImage;
		bool _kisCreated;
		U32 _ktexId;			//!< ogl array texture name
		KVector2U32 _ksize;
		TextureFilter _kfilter;	//!< Texture interpolation
		TextureWrap _kwrap;		//!< Texture wrapping
		std::vector<KSharedResource> _kimageArray;
		static U32 _klastTexId;	//!< Static last texture ID
	};
}

#endif // KATLASTEXTURE_H
