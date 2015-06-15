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
#ifndef KFRAMEBUFFER_H
#define KFRAMEBUFFER_H

/*! \file kframebuffer.h */

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/ktexture.h"

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{

	//! The KFrameBuffer class encapsulate OpenGL framebuffer object (FBO).
	/*!
		This class provides features such as render to texture and off-screen rendering.
	*/
    class KITE_FUNC_EXPORT KFrameBuffer : KNonCopyable{
	public:

		//! Constructs an frame buffer object (FBO).
        KFrameBuffer();

		//! Destructor
        ~KFrameBuffer();

		//! Attach a texture to color buffer. (render to texture)
		/*!
			This function is similar to the glFramebufferTexture2D().

			\param Texture Pointer to a initialized texture.
		*/
        void attachTexture(const KTexture *Texture);

		//! Bind the framebuffer object.
		/*!
			This function is similar to the glBind*(),
			but avoid calling that gl function several times if our FBO is currently bound.
			(automatic handle by internal render system)
		*/
        void bind();

		//! Unbind the framebuffer if it is currently bound.
		/*!
			This function is similar to the glBind*(0).
			unbind only if our FBO is currently bound.
		*/
        void unbind();

		//! Unbind current frame buffer.
		/*!
			This function is similar to the glBind*(0).
		*/
        static void unbindFrameBuffer();

		//! Get OpenGL ID of the frame buffer object
		/*!
			\return OpenGL ID of frame buffer object
		*/
		inline U32 getGLID() const { return _kbufId; }

    private:
        U32 _kbufId;			//!< OpenGL FBO ID
        static U32 _klastBufId; //!< Last FBO ID
    };
}

#endif // KFRAMEBUFFER_H
