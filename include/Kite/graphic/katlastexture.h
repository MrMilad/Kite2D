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

/*! \file ktexture.h */

#include "Kite/core/kcoredef.h"
#include "Kite/ecs/kresource.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/graphic/kgraphictypes.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "katlastexture.khgen.h"

/*! \namespace Kite
	\brief Public namespace.
*/
KMETA
namespace Kite{

	//! The KAtlasTexture class encapsulates an OpenGL texture object with atlas-textures features.
	/*!
		KAtlasTexture makes it easy to work with OpenGL textures.
	*/
	KM_CLASS(RESOURCE)
    class KITE_FUNC_EXPORT KAtlasTexture : public KResource{
		KM_INFO(KI_NAME = "AtlasTexture");
		KATLASTEXTURE_BODY();
    public:
		KAtlasTexture(const KSharedResource &AtlasImage);

		//! Destructor
		~KAtlasTexture();

		KAtlasTexture() = delete;
		KAtlasTexture(const KAtlasTexture &Copy) = delete;
		KAtlasTexture &operator=(const KAtlasTexture &Right) = delete;

		bool saveStream(KIOStream &Stream, const std::string &Address) override;

		KM_PRO_GET(KP_NAME = "atlasImage", KP_RES = Resource::ATLASIMAGE)
		inline KSharedResource getAtlasImage() const { return _kimage; }

		KM_PRO_GET(KP_NAME = "width", KP_TYPE = U32, KP_CM = "width of the texture")
		inline U32 getWidth() const { return _ksize.x; }

		KM_PRO_GET(KP_NAME = "height", KP_TYPE = U32, KP_CM = "height of the texture")
		inline U32 getHeight() const { return _ksize.y; }

		KM_PRO_GET(KP_NAME = "size", KP_TYPE = KVector2U32, KP_CM = "size of the texture")
		inline const KVector2U32 &getSize() const { return _ksize; }

		//! Get filter of the texture
		/*!
			\return Filter of the texture
		*/
		KM_PRO_GET(KP_NAME = "filter", KP_TYPE = TextureFilter, KP_CM = "filter of the texture")
        inline TextureFilter getFilter() const {return _kfilter;}

		//! Get wrap of the texture
		/*!
			\return Filter of the texture
		*/
		KM_PRO_GET(KP_NAME = "wrap", KP_TYPE = TextureWrap, KP_CM = "wrap of the texture")
        inline TextureWrap getWrap() const {return _kwrap;}

		//! Get OpenGL ID of the texture
		/*!
			note: call inite before calling this function.

			\return OpenGL ID of the texture
		*/
		KM_PRO_GET(KP_NAME = "glID", KP_TYPE = U32, KP_CM = "opengl id of the texture")
        inline U32 getGLID() const {return _ktexId;}

		//! Set texture filtering
		/*!
			note: call inite before calling this function.

			\param Filter Filter of the texture
		*/
		KM_PRO_SET(KP_NAME = "filter")
        void setFilter(TextureFilter Filter);

		//! Set texture wraping
		/*!
			note: call inite before calling this function.

			\param Wrap Wrap of the texture
		*/
		KM_PRO_SET(KP_NAME = "wrap")
        void setWrap(TextureWrap Wrap);

		KM_PRO_SET(KP_NAME = "cleanUpImage")
		void setCleanupImage(bool CleanUp);

		KM_PRO_GET(KP_NAME = "cleanUpImage", KP_TYPE = bool, KP_CM = "clean up atlas image after creating ogl texture")
		inline bool getCleanUpImage() const { return _kclearImage; }

		/// get online GPU-side pixeldata
		KM_PRO_GET(KP_NAME = "textureImage", KP_CM = "GPU-side image", KP_RES = Resource::IMAGE)
		KSharedResource getTextureImage() const;

		//! Update whole or piece of texture with an image
		/*!
		Size of the image + position must equal or smaller than texture size,
		If not, this is an undefined behaviour.
		always "update" an existing texture is faster than "re-create" that.
		this function does nothing if the texture was not previously created.
		note: call inite before calling this function.

		\param Image Image to copy into the texture
		\param Position in the texture where to copy the source pixels
		*/
		KM_FUN()
		void update(const KSharedResource &Image, const KVector2U32 &Position);

		//! Bind the texture
		/*!
			This function is similar to the glBindTexture(),
			but avoid calling that gl function several times if our FBO is currently bound.
			(automatic handle by internal render system)
		*/
		bool bind();

		//! Unbind the texture if it is currently bound.
		void unbind();

		//! Unbind current texture
		static void unbindTexture();

    private:
		KAtlasTexture(const std::string &Name, const std::string &Address);

		KM_FUN(KP_NAME = "__call")
		static KSharedResource luaConstruct(const KSharedResource &AtlasImage);

		bool _loadStream(std::unique_ptr<KIOStream> Stream, KResourceManager *RManager) override;

		//! create the texture (opengl)
        bool _create();

		bool _kclearImage;			//!< clear image after texture creation
		bool _kisCreated;
        U32 _ktexId;				//!< ogl texture name
        TextureFilter _kfilter;		//!< Texture interpolation
        TextureWrap _kwrap;			//!< Texture wrapping
        KVector2U32 _ksize;			//!< Size of texture
		KSharedResource _kimage;	//!< Offline pixel data (KImage)
		static U32 _klastTexId;		//!< Static last texture ID
    };
}

#endif // KATLASTEXTURE_H
