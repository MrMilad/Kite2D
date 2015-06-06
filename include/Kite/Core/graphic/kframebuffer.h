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

	//! The KFrameBuffer class is a low-level class that encapsulate OpenGL framebuffer object (FBO).
	/*!
		This class provides features such as render to texture and off-screen rendering.
	*/
    class KITE_FUNC_EXPORT KFrameBuffer : KNonCopyable{
	public:

		//! Constructs an framebuffer object (FBO).
        KFrameBuffer();

		//! Destroys the framebuffer object.
        ~KFrameBuffer();

		//! Attach a texture to color buffer. (render to texture)
		/*!
			\param Texture Pointer to a initialized texture.
		*/
        void attachTexture(const KTexture *Texture);

		//! Bind the framebuffer object.
		/*!
			This is similar to the glBind*() function call,
			but avoid calling that gl function several times if our FBO is currently in use.
		*/
        void bind();

		//! Unbind the framebuffer if it is currently bound.
		/*!
			This is similar to the glBind*(0) function call.
			unbind only if our FBO is currently bound.
		*/
        void unbind();

		//! Unbind current frame buffer.
		/*!
			This is similar to the glBind*(0) function call.
		*/
        static void unbindFrameBuffer();

    private:
        U32 _kbufId;			//!< OpenGL FBO ID
        static U32 _klastBufId; //!< Last FBO ID
    };
}

#endif // KFRAMEBUFFER_H
