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
#ifndef KRENDER_H
#define KRENDER_H

/*! \file krender.h */

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include <vector>

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{
	//! The KRender class is a low-level class that encapsulate all supported render functions.
	/*!
		This class is a OpenGL wrapper that provide managed and optimized functions for rendering vertex buffers.
	*/
    class KITE_FUNC_EXPORT KRender{
    public:
		//! Initialize OpenGL for 2D rendering
        static bool inite();

		//! Clear the scene
        static void clear();

		//! Draw a range of elements in the VBO
		/*!
			This function is similar to the glDrawArrays().

			\param FirstIndex Specifies the starting index in the enabled VBO
			\param Size Specifies the number of indices to be rendered
			\param Primitive Specifies what kind of primitives to render
		*/
        static void draw(U32 FirstIndex, U32 Size, KGeoPrimitiveTypes Primitive);

		//! Draw multiple instances of a range of elements in the VBO
		/*!
			This function is similar to the glDrawArraysInstanced().

			\param FirstIndex Specifies the starting index in the enabled VBO
			\param Size Specifies the number of indices to be rendered
			\param Primitive Specifies what kind of primitives to render
			\param InstanceCount Specifies the number of instances of the specified range of indices to be rendered
		*/
        static void draw(U32 FirstIndex, U32 Size, KGeoPrimitiveTypes Primitive, U32 InstanceCount);

		//! Draw indexed range of elements in the VBO
		/*!
			This function is similar to the glDrawElements().

			\param Count Specifies the number of elements to be rendered. 
			\param Indices Specifies a pointer to the location where the indices are stored
			\param Primitive Specifies what kind of primitives to render
		*/
        static void draw(U32 Count, const U16 *Indices, KGeoPrimitiveTypes Primitive);

		//! Draw multiple instances of indexed range of elements in the VBO
		/*!
			This function is similar to the glDrawElementsInstanced().

			\param Count Specifies the number of elements to be rendered.
			\param Indices Specifies a pointer to the location where the indices are stored
			\param Primitive Specifies what kind of primitives to render
			\param InstanceCount Specifies the number of instances of the specified range of indices to be rendered
		*/
        static void draw(U32 Count, const U16 *Indices, KGeoPrimitiveTypes Primitive, U32 InstanceCount);

		//! Set the clear color
		/*!
			\param Color Specify clear values for the color buffers
		*/
        static void setClearColor(const KColor &Color);

		//! Set the size of points
		/*!
			\param Size Size of points
		*/
        static void setPointSize(F32 Size);

		//! Enable or disable wireframe mode
		/*!
			\param Wireframe If set true, wireframe mode is enabled
		*/
		static void setWireframe(bool Wireframe);

		//! Set the viewport
		/*!
			The left and bottom components in KRectU32 specify the lower left corner of the viewport rectangle.
            the initial value is (0,0). (in pixels)
			the right and top components in KRectU32 specify the width and height of the viewport.
            when a GL context is first attached to a window,
            width and height are set to the dimensions of that window.

			\param Viewport Specify the viewport
		*/
        static void setViewport(const KRectI32 &Viewport);

		//! Get the viewport
		/*!
			\return Read-only pointer to viewport
		*/
        static const KRectI32 *getViewport();

//        void setBlendMode(KBlendMode BlendMode);
    private:
		//! Default constructors
        KRender();

		//! Destructurs
        ~KRender();

        static const U32 geoTypes[11];	//!< Static array of GL geometric types
        static KRectI32 _kviewport;		//!< Static viewport
    };
}

#endif // KRENDER_H
