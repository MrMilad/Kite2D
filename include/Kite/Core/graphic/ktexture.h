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
#ifndef KTEXTURE_H
#define KTEXTURE_H

/*! \file ktexture.h */

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/math/kmathstructs.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/ktextureresource.h"
#include "Kite/Core/graphic/kimage.h"

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{

	//! The KTexture class encapsulates an OpenGL texture object.
	/*!
		KTexture makes it easy to work with OpenGL textures.
	*/
    class KITE_FUNC_EXPORT KTexture : public KTextureResource{
    public:

		//! Constructs an blank texture object.
        KTexture();

		//! Destructor
        ~KTexture();

		//! Create a blank texture with the specific size, filter and wrap
		/*!
			\param Size of texture (in pixels)
			\param Filter of texture
			\param Wrap Wrap of texture
		*/
        void create(const KVector2U32 &Size ,KTextureFilterTypes Filter, KTextureWrapTypes Wrap);

		//! Create the texture from an image
		/*!
			The entire image is used to create the texture.

			\param Image Image to load into the texture
			\param Filter of texture
			\param Wrap Wrap of texture
		*/
        void create(const KImage &Image, KTextureFilterTypes Filter, KTextureWrapTypes Wrap);

		//! Update whole or piece of texture with an image
		/*!
			Size of the image + position must equal or smaller than texture size,
			If not, this is an undefined behaviour.
			always "update" an existing texture is faster than "re-create" that.
			this function does nothing if the texture was not previously created.

			\param Image Image to copy into the texture
			\param Position in the texture where to copy the source pixels
		*/
		void update(const KImage &Image, const KVector2U32 &Position);

		//! Get filter of the texture
		/*!
			\return Filter of the texture
		*/
        inline KTextureFilterTypes getFilter() const {return _kfilter;}

		//! Get wrap of the texture
		/*!
			\return Filter of the texture
		*/
        inline KTextureWrapTypes getWrap() const {return _kwrap;}

		//! Get OpenGL ID of the texture
		/*!
			\return OpenGL ID of the texture
		*/
        inline U32 getGLID() const {return _ktexId;}

		//! Set texture filtering
		/*!
			\param Filter Filter of the texture
		*/
        void setFilter(KTextureFilterTypes Filter);

		//! Set texture wraping
		/*!
			\param Wrap Wrap of the texture
		*/
        void setWrap(KTextureWrapTypes Wrap);

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

		//! Get size of resource in memory
		/*!
			\return Size of resource in bytes
		*/
		U64 resGetSize() const;

    private:
		//! Create the texture
        static void _create(const U8 *Data, const KVector2U32 &Size,
                            KTextureFilterTypes Filter, KTextureWrapTypes Wrap, KTexture &Instance);
        U32 _ktexId;	//!< ogl texture name
        KTextureFilterTypes _kfilter;	//!< Texture interpolation
        KTextureWrapTypes _kwrap;		//!< Texture wrapping
        KVector2U32 _ksize;				//!< Size of texture
        static U32 _klastTexId;			//!< Static last texture ID
    };
}

#endif // KTEXTURE_H
