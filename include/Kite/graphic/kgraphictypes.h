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
#ifndef KGRAPHICTYPES_H
#define KGRAPHICTYPES_H

#include "Kite/core/kcoredef.h"
#include "Kite/meta/kmetadef.h"
#include "kgraphictypes.khgen.h"

/*! \file kgraphictypes.h
	\brief All core graphic types.
*/

/*! \namespace Kite
	\brief Public namespace.
*/
KMETA
namespace Kite{
	/*! \enum Pixel blend
	\brief Blend types
	*/
	KM_ENUM()
    enum class TextureEnv{
        ADD = 0,
        MODULATE,
        DECAL,
        BLEND,
        REPLACE,
        COMBINE
    };
	KMETA_TEXTUREENV_BODY();

	/*! \enum KVertexBufferTypes
		\brief Performance hint for VBO to provide how the buffer object is going to be used.
	*/
	KM_ENUM()
    enum class VBufferType{
        STATIC = 0,	//!< The data in VBO will not be changed (specified once and used many times)
        DYNAMIC,	//!< The data in VBO will be changed frequently (specified and used repeatedly)
        STREAM		//!< The data in VBO will be changed every frame (specified once and used once)
    };
	KMETA_VBUFFERTYPE_BODY();

	/*! \enum KBufferTargetTypes
		\brief Buffer types.
	*/
	KM_ENUM()
	enum class BufferTarget{
		INDEX = 0,	//!< Index buffer
		VERTEX		//!< Vertex buffer
	};
	KMETA_BUFFERTARGET_BODY();

	/*! \enum KMapAccessTypes */
	KM_ENUM()
    enum class MapAccess{
        READ = 0,	//!< Reading access
        WRITE,		//!< Writing access
        RW			//!< Reading/writing access
    };
	KMETA_MAPACCESS_BODY();

	/*! \enum KGeoPrimitiveTypes
		\brief Primitive types
	*/
	KM_ENUM()
    enum class GLPrimitive{
        POINTS = 0,
        LINES,
        LINE_STRIP,
        LINE_LOOP,
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,
        LINE_STRIP_ADJACENCY,
        LINES_ADJACENCY,
        TRIANGLE_STRIP_ADJACENCY,
        TRIANGLES_ADJACENCY
    };
	KMETA_GLPRIMITIVE_BODY();

//    enum KFilteringDrawTypes{
//        KFD_ALIASED,
//        KFD_SMOOTH
//    };

	/*! \enum KColorTypes
		\brief Hex colors code
	*/
	KM_ENUM()
    enum class Colors{
        BLACK = 0x000000,           WHITE = 0xFFFFFF,
        RED = 0xff0000,             YELLOW = 0xffff00,
        BLUE = 0x0000ff,            GREEN = 0x00ff00,
        CRIMSON = 0xDC143C,         PINK = 0xFFC0CB,
        PALEVIOLETRED = 0xDB7093,   LIMEGREEN = 0x32CD32,
        LAVENDERBLUSH = 0xFFF0F5,   HOTPINK = 0xFF69B4,
        RASPBERRY = 0x872657,       COBALTGREEN = 0x3D9140,
        DEEPPINK = 0xFF1493,        MAROON = 0xFF34B3,
        VIOLET = 0xEE82EE,          MEDIUMSEAGREEN = 0x3CB371,
        DARKVIOLET = 0x9400D3,      ORCHID = 0xDA70D6,
        THISTLE = 0xD8BFD8,         CYAN = 0x00FFFF,
        PLUM = 0xDDA0DD,            MAGENTA  = 0xFF00FF,
        BLUEVIOLET = 0x8A2BE2,      POWDERBLUE = 0xB0E0E6,
        PURPLE = 0x800080,          GHOSTWHITE = 0xF8F8FF,
        INDIGO = 0x4B0082,          DODGERBLUE = 0x1E90FF,
        LAVENDER = 0xE6E6FA,        NAVY = 0x000080,
        MIDNIGHTBLUE = 0x191970,    CORNFLOWERBLUE = 0x6495ED,
        COBALT = 0x3D59AB,          ROYALBLUE = 0x4169E1,
        LIGHTSTEELBLUE = 0xB0C4DE,  SLATEGRAY = 0x708090,
        SKYBLUE = 0x87CEEB,         PEACOCK = 0x33A1C9,
        CADETBLUE = 0x5F9EA0,       AZURE = 0xF0FFFF,
        COLDGREY = 0x808A87,        SPRINGGREEN = 0x00FF7F,
        EMERALDGREEN = 0x00C957,    MINT = 0xBDFCC9,
        DARKSEAGREEN = 0x8FBC8F,    PALEGREEN = 0x98FB98,
        FORESTGREEN = 0x228B22,     LAWNGREEN = 0x7CFC00,
        GREENYELLOW = 0xADFF2F,     MELON = 0xE3A869,
        OLIVEDRAB = 0x6B8E23,       IVORY = 0xFFFFF0,
        BEIGE = 0xF5F5DC,           ORANGE = 0xFF8000,
        OLIVE = 0x808000,           DARKKHAKI = 0xBDB76B,
        KHAKI = 0xF0E68C,           SIENNA = 0xA0522D,
        PALEGOLDENROD = 0xEEE8AA,   BANANA = 0xE3CF57,
        GOLD = 0xFFD700,            SNOW = 0xFFFAFA,
        CADMIUMYELLOW = 0xFF9912,   BRICK = 0x9C661F,
        ARKORANGE = 0xFF8C00,       CARROT = 0xED9121,
        FLESH = 0xFF7D40,           CADMIUMORANGE = 0xFF6103,
        CORAL = 0x8B5742,           SALMON = 0xFA8072,
        BROWN = 0xA52A2A,           FIREBRICK = 0xB22222,
        GRAY = 0x808080
    };
	KMETA_COLORS_BODY();

	/*! \enum KTextureFilterTypes
		\brief Magnification texture filtering options
	*/
	KM_ENUM()
    enum class TextureFilter{
		NEAREST = 0,	//!< Select the texel nearest the texture coordinate
        LINEAR		//!< Perform a weighted linear blend between the nearest adjacent samples
    };
	KMETA_TEXTUREFILTER_BODY();

	/*! \enum KTextureWrapTypes
		\brief The possible heuristic when a texture coordinate is not within the [0, 1] range
	*/
	KM_ENUM()
    enum class TextureWrap{
		REPEAT = 0,			//!< The texture coordinate wraps around the texture
        MIRRORED_REPEAT,	//!< The texture coordinate wraps around like a mirror
        CLAMP_TO_EDGE		//!< The texture coordinate is clamped to the [0, 1] range
     // KTW_CLAMP_TO_BORDER		// border not supported (see KTexture::create())
    };
	KMETA_TEXTUREWRAP_BODY();

	/*! \enum KShaderTypes
		\brief OpenGL shader types
	*/
	KM_ENUM()
    enum class ShaderType{
        VERTEX = 0,	//!< Vertex shader
        FRAGMENT,	//!< Fragment shader
		GEOMETRY,	//!< Geometry shader
		UNKNOWN
    };
	KMETA_SHADERTYPE_BODY();

//    enum KShaderTextureTypes{
//        KST_CURRENT = 0
//    };

	/*! \enum KAttributeCountTypes
		\brief Specifies the number of components per generic vertex attribute. (Internally use)
	*/
	KM_ENUM()
    enum class AttributeCount{
        COMPONENT_1 = 1,
        COMPONENT_2 = 2,
        COMPONENT_3 = 3,
        COMPONENT_4 = 4
    };
	KMETA_ATTRIBUTECOUNT_BODY();

	/*! \enum KAttributeTypeTypes
		\brief Specifies the data type of each component in the array. (Internally use)
	*/
	KM_ENUM()
    enum class AttributeType{
        BYTE = 0,
        UNSIGNED_BYTE,
        SHORT,
        UNSIGNED_SHORT,
        FIXED,
        FLOAT
    };
	KMETA_ATTRIBUTETYPE_BODY();

	KM_ENUM()
	enum class BuiltinShaderType {
		VERT = 0,
		FRAG_COLOR,
		FRAG_TEXTURE,
		FRAG_TEXTURE_COLOR,
		VERT_POINT,
		FRAG_POINT_COLOR,
		FRAG_POINT_TEXTURE,
		FRAG_POINT_TEXTURE_COLOR,
	};
	KMETA_BUILTINSHADERTYPE_BODY();

	/*! \typedef KAtlasFileTypes
	\brief Types of the atlas files
	*/
	KM_ENUM()
	enum class AtlasFile{
		ATLAS,	//!< Default KAtlas format (compact format, better speed and size)
		JSON	//!< Adobe Flash created JSON 
	};
	KMETA_ATLASFILE_BODY();

	KM_ENUM()
	enum class GCullingObjectsFilter {
		STATIC = 0,
		DYNAMIC,
		TILE,
		ALL
	};
	KMETA_GCULLINGOBJECTSFILTER_BODY();

	/*! \typedef KCallVBUpdate
		\brief Function handle for mapping and updating VBO buffers
	*/
	typedef void (KCallVBUpdate)(void *Data, U32 Offset, U32 DataSize, void *Sender);

	/*! \namespace Kite::Internal
		\brief Private namespace.
	*/
    namespace Internal{

		/*! \enum KGLBindGuardTypes
			\brief (Internal use)
		*/
		enum KGLBindGuardTypes{
			KBG_NONE = 0,	//!< None
			KBG_TEXTURE,	//!< Texture
			KBG_VBUFFER,	//!< Vertex buffer
			KBG_FBUFFER		//!< Frame buffer
		};
    }

}
#endif // KGRAPHICTYPES_H
