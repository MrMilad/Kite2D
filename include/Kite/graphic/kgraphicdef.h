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

	/// color blending macros
	#define BLEND_Normal(A,B)     ((unsigned char)(A))
	#define BLEND_Lighten(A,B)    ((unsigned char)((B > A) ? B:A))
	#define BLEND_Darken(A,B)     ((unsigned char)((B > A) ? A:B))
	#define BLEND_Multiply(A,B)   ((unsigned char)((A * B) / 255))
	#define BLEND_Average(A,B)    ((unsigned char)((A + B) / 2))
	#define BLEND_Add(A,B)        ((unsigned char)(min(255, (A + B))))
	#define BLEND_Subtract(A,B)   ((unsigned char)((A + B < 255) ? 0:(A + B - 255)))
	#define BLEND_Difference(A,B) ((unsigned char)(abs(A - B)))
	#define BLEND_Negation(A,B)   ((unsigned char)(255 - abs(255 - A - B)))
	#define BLEND_Screen(A,B)     ((unsigned char)(255 - (((255 - A) * (255 - B)) >> 8)))
	#define BLEND_Exclusion(A,B)  ((unsigned char)(A + B - 2 * A * B / 255))
	#define BLEND_Overlay(A,B)    ((unsigned char)((B < 128) ? (2 * A * B / 255):(255 - 2 * (255 - A) * (255 - B) / 255)))
	#define BLEND_SoftLight(A,B)  ((unsigned char)((B < 128)?(2*((A>>1)+64))*((float)B/255):(255-(2*(255-((A>>1)+64))*(float)(255-B)/255))))
	#define BLEND_HardLight(A,B)  (BLEND_Overlay(B,A))
	#define BLEND_ColorDodge(A,B) ((unsigned char)((B == 255) ? B:min(255, ((A << 8 ) / (255 - B)))))
	#define BLEND_ColorBurn(A,B)  ((unsigned char)((B == 0) ? B:max(0, (255 - ((255 - A) << 8 ) / B))))
	#define BLEND_LinearDodge(A,B)(BLEND_Add(A,B))
	#define BLEND_LinearBurn(A,B) (BLEND_Subtract(A,B))
	#define BLEND_LinearLight(A,B)((unsigned char)(B < 128)?BLEND_LinearBurn(A,(2 * B)):BLEND_LinearDodge(A,(2 * (B - 128))))
	#define BLEND_VividLight(A,B) ((unsigned char)(B < 128)?BLEND_ColorBurn(A,(2 * B)):BLEND_ColorDodge(A,(2 * (B - 128))))
	#define BLEND_PinLight(A,B)   ((unsigned char)(B < 128)?BLEND_Darken(A,(2 * B)):BLEND_Lighten(A,(2 * (B - 128))))
	#define BLEND_HardMix(A,B)    ((unsigned char)((BLEND_VividLight(A,B) < 128) ? 0:255))
	#define BLEND_Reflect(A,B)    ((unsigned char)((B == 255) ? B:min(255, (A * A / (255 - B)))))
	#define BLEND_Glow(A,B)       (BLEND_Reflect(B,A))
	#define BLEND_Phoenix(A,B)    ((unsigned char)(min(A,B) - max(A,B) + 255))
	#define BLEND_Alpha(A,B,O)    ((unsigned char)(O * A + (1 - O) * B))
	#define BLEND_AlphaF(A,B,F,O) (BLEND_Alpha(F(A,B),A,O))

	/*! \brief Buffer offest*/
	#define KBUFFER_OFFSET(i) ((void*)(i))

	/*! \brief Vertex attribute index*/
	#define KVATTRIB_XY 0
	/*! \brief Vertex attribute index*/
	#define KVATTRIB_UV 1
	/*! \brief Vertex attribute index*/
	#define KVATTRIB_RGBA 2
	/*! \brief Texture array index attribute index*/
	#define KVATTRIB_ARRAYINDEX 3

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
