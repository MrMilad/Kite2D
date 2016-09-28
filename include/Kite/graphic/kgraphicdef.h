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
#ifndef KGRAPHICDEF_H
#define KGRAPHICDEF_H

/*! \file kgraphicdef.h
	\brief All core graphic macros.
*/

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{

	#define KCOLOR_TO_GL(col)		(col / 255.0f)
	#define KCOLOR_TO_RGB(col)		(col * 255.0f)

	/*! \brief Hardware color macros*/
	#define ARGB(a,r,g,b)	((DWORD(a)<<24) + (DWORD(r)<<16) + (DWORD(g)<<8) + DWORD(b))
	/*! \brief Hardware color macros*/
	#define GETA(col)		((col)>>24)
	/*! \brief Hardware color macros*/
	#define GETR(col)		(((col)>>16) & 0xFF)
	/*! \brief Hardware color macros*/
	#define GETG(col)		(((col)>>8) & 0xFF)
	/*! \brief Hardware color macros*/
	#define GETB(col)		((col) & 0xFF)
	/*! \brief Hardware color macros*/
	#define SETA(col,a)		(((col) & 0x00FFFFFF) + (DWORD(a)<<24))
	/*! \brief Hardware color macros*/
	#define SETR(col,r)		(((col) & 0xFF00FFFF) + (DWORD(r)<<16))
	/*! \brief Hardware color macros*/
	#define SETG(col,g)		(((col) & 0xFFFF00FF) + (DWORD(g)<<8))
	/*! \brief Hardware color macros*/
	#define SETB(col,b)		(((col) & 0xFFFFFF00) + DWORD(b))

	/*! \brief Buffer offest*/
	#define KBUFFER_OFFSET(i) ((void*)(i))

	/*! \brief Vertex attribute index*/
	#define KVATTRIB_XY 0
	/*! \brief Vertex attribute index*/
	#define KVATTRIB_UV 1
	/*! \brief Texture array index attribute index*/
	#define KVATTRIB_ARRAYINDEX 2
	/*! \brief Vertex attribute index*/
	#define KVATTRIB_RGBA 3

	/*!
		\brief Convert opengl (buttom-left) float coordinate to window (top-left) integer coordinate.
				default center = top-right
	*/
	#define KCGL_TO_WIN_X(x, w)         (((float)x + 1) * (w / 2))

	/*!
		\brief Convert window (top-left) integer coordinate to opengl (buttom-left) float coordinate.
				default center = top-right
	*/
	#define KCGL_TO_WIN_Y(y, h)         (h - (((float)y + 1) * (h / 2)))

	/*!
		\brief Convert window (top-left) integer coordinate to opengl (buttom-left) float coordinate.
				default center = top-right
	*/
	#define KCWIN_TO_GL_X(x, w)         ((x - ((float)w / 2)) * (1 / ((float)w / 2)))

	/*!
		\brief Convert window (top-left) integer coordinate to opengl (buttom-left) float coordinate.
				default center = top-right
	*/
	#define KCWIN_TO_GL_Y(y, h)         ((y - ((float)h / 2)) * (1 / ((float)h / 2)) * -1)

	/*!
		\brief Convert (buttom-left) integer coordinate to (buttom-left) opengl float coordinate.
				default center = top-right
	*/
	#define KCINT_TO_F_POS(xy, wh)      ((xy - ((float)wh / 2)) * (1 / ((float)wh / 2)))

	/*!
		\brief Convert integer base size to float (opengl -1 to 0) base size.
				e.g s = 10, b = 40 , result = 0.5
	*/
	#define KCINT_TO_F_SIZE(s, b)       ((2 / (float)b) * s)


	/*!
	\brief Convert integer base size to float (opengl 0 to 1) texture uv.
		e.g uv = 10, wh = 100 , result = 0.1
	*/
	#define KCINT_TO_F_TEXTURE(uv, wh)       ((1 / (float)wh) * uv)

	/*! \brief Convert mouse position (y) from microsoft windows coordinate to opengl coordinate */
	#define KCYMOUSE(y, h)			(h - y)

}
#endif // KGRAPHICDEF_H
