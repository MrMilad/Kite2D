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
#ifndef KTEXTURE_H
#define KTEXTURE_H

/*! \file ktexture.h */

#include "Kite/core/kcoredef.h"
#include "Kite/ecs/kresource.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/graphic/kgraphictypes.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/graphic/kimage.h"
#include "ktexture.khgen.h"

/*! \namespace Kite
	\brief Public namespace.
*/
KMETA
namespace Kite{

	//! The KTexture class encapsulates an OpenGL texture object.
	/*!
		KTexture makes it easy to work with OpenGL textures.
		Supported formats: PNG, BMP, TGA.
		note: only use KFOStream for saving textures.
	*/
	KM_CLASS(RESOURCE)
    class KITE_FUNC_EXPORT KTexture : public KResource{
		KM_INFO(KI_NAME = "Texture");
		KTEXTURE_BODY();
    public:

		//! Constructs an 1x1 texture object.
        KTexture(const std::string &Name, const std::string &Address);

		//! Destructor
        ~KTexture();

		KM_FUN()
		bool saveStream(KOStream &Stream, const std::string &Address) override;

		//! Create a blank texture with the specific size, filter and wrap
		/*!
			\param Size of texture (in pixels)
			\param Filter of texture
			\param Wrap Wrap of texture
		*/
		KM_FUN()
        void createFromSize(const KVector2U32 &Size ,TextureFilter Filter, TextureWrap Wrap);

		//! Create the texture from an image
		/*!
			The entire image is used to create the texture.
			Image object can be deleted after callig this function.

			\param Image Image to load into the texture
			\param Filter of texture
			\param Wrap Wrap of texture
			\param ClearImage Clear image data after texture initialization
		*/
		KM_FUN()
        void createFromImage(KImage *Image, TextureFilter Filter, TextureWrap Wrap, bool ClearImage);

		KM_FUN()
		void getImage(KImage &ImageOutput) const;

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
		void update(const KImage &Image, const KVector2U32 &Position);

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
		KM_PRO_GET(KP_NAME = "glid", KP_TYPE = U32, KP_CM = "opengl id of the texture")
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

		//! Bind the texture
		/*!
			This function is similar to the glBindTexture(),
			but avoid calling that gl function several times if our FBO is currently bound.
			(automatic handle by internal render system)
		*/
		void bind() const;

		//! Unbind the texture if it is currently bound.
		void unbind();

		//! Unbind current texture
		static void unbindTexture();

    private:
		bool _loadStream(std::unique_ptr<KIStream> Stream, KResourceManager *RManager) override;

		//! create the texture (opengl)
        static void _create(const U8 *Data, const KVector2U32 &Size,
                            TextureFilter Filter, TextureWrap Wrap, KTexture &Instance);

		bool _kclearImage;		//!< clear image after texture initializaton
        U32 _ktexId;			//!< ogl texture name
        TextureFilter _kfilter;	//!< Texture interpolation
        TextureWrap _kwrap;		//!< Texture wrapping
        KVector2U32 _ksize;		//!< Size of texture

		KImage *_kimage;		//!< Offline pixel data
		static U32 _klastTexId;	//!< Static last texture ID
    };
}

#endif // KTEXTURE_H
