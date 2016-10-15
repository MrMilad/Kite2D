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
#ifndef KVERTEXARRAY_H
#define KVERTEXARRAY_H

/*! \file kvertexarray.h */

#include "Kite/core/kcoredef.h"
#include "Kite/graphic/kgraphictypes.h"

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{

	//! The KVertexArray class encapsulates an OpenGL vertexarray object (VAO) object.
	/*!
		A Vertex Array Object (VAO) is an OpenGL container object that encapsulates the state needed
		to specify per-vertex attribute data to the OpenGL pipeline. To put it another way,
		a VAO remembers the states of buffer objects (see KVertexBuffer and KFrameBuffer) 
		and their associated state (e.g. vertex attribute divisors).
		this allows a very easy and efficient method of switching between OpenGL
		buffer states for rendering different "objects" in a scene.
	*/
    class KITE_FUNC_EXPORT KVertexArray{
    public:

		//! Constructs an vertex array object (VAO).
        KVertexArray();

		//! Destructor
        ~KVertexArray();

		//! Specifies the index of the generic vertex attribute to be modified.
		/*!
			This function is similar to the glVertexAttribPointer().

			\param Index The index of the generic vertex attribute to be modified
			\param Count The number of components per generic vertex attribute
			\param Type The data type of each component in the array
			\param Normalized Whether fixed-point data values should be normalized (true)
					or converted directly as fixed-point values (false) when they are accessed
			\param StrideSize The byte offset between consecutive generic vertex attributes.
					if StrideSize is 0, the generic vertex attributes are understood
					to be tightly packed in the array. The initial value is 0.
			\param Offset Offset of the first component of the first generic
					vertex attribute in the array in the data store of the buffer
					The initial value is 0.
		*/
        void setAttribute(U16 Index, AttributeCount Count,
                          AttributeType Type, bool Normalized,
                          I32 StrideSize, void *Offset);

		//! Enable a generic vertex attribute array
		/*!
			This function is similar to the glEnableVertexAttribArray().

			\param Index The index of the generic vertex attribute to be enabled
		*/
        void enableAttribute(U16 Index);

		//! Disable a generic vertex attribute array
		/*!
			This function is similar to the glDisableVertexAttribArray().

			\param Index The index of the generic vertex attribute to be disabled
		*/
        void disableAttribute(U16 Index);

		//! Modify the rate at which generic vertex attributes advance during instanced rendering
		/*!
			This function is similar to the glVertexAttribDivisor().

			\param Index The index of the generic vertex attribute
			\param Divisor The number of instances that will pass between updates
							of the generic attribute at slot index.
		*/
        void AttributeDivisor(U16 Index, U32 Divisor);

		//! Bind the vertex array object.
		/*!
			This function is similar to the glBind*(),
			but avoid calling that gl function several times if our FBO is currently bound.
			(automatic handle by internal render system)
		*/
        void bind() const;

		//! Unbind the vertex array if it is currently bound.
        void unbind();

		//! Unbind current vertex array.
        static void unbindVertexArray();

		//! Get OpenGL ID of the vertex array object
		/*!
			\return OpenGL ID of the vertex array object
		*/
        inline U32 getGLID() const {return _karrayId;}

    private:
        U32 _karrayId;				//!< OpenGL VAO ID
		static U32 _klastArrayId;	//!< Last VAO ID
    };
}

#endif // KVERTEXARRAY_H
