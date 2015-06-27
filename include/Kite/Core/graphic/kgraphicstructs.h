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
#ifndef KGRAPHICSTRUCTS_H
#define KGRAPHICSTRUCTS_H

/*! \file kgraphicstructs.h
	\brief All core graphic structs.
*/

#include "Kite/Core/system/ksystemtypes.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Core/math/kmath.h"
#include "Kite/Core/system/ksystemdef.h"
#include <vector>

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{
	//! Utility struct for manipulating RGBAs color.
	/*! 
		KColor is a simple color struct composed of 4 components: r, g, b, a (opacity)
		each component is an float in the range [0, 255]
	*/
    struct KColor{
		F32 r;	//!< Red component
		F32 g;	//!< Green component
		F32 b;	//!< Blue component
		F32 a;	//!< Alpha component (opacity)

		//! Construct the color from its 4 RGBA components.
		/*!
			Default value is (255, 255, 255, 255)
		*/
		KColor(F32 R = 255, F32 G = 255, F32 B = 255, F32 A = 255) :
            r(R/255.0f), g(G/255.0f), b(B/255.0f), a(A/255.0f)
        {}

		//! Construct the color from a hexadecimal color code.
		/*!
			A large number of color codes defined in KColorTypes 
			However, the hexadecimal code can be passed directly
		*/
		KColor(KColorTypes HexCode){
			r = ((U8)((HexCode >> 16) & 0xFF)) / 255.0f;
			g = ((U8)((HexCode >> 8) & 0xFF)) / 255.0f;
			b = ((U8)((HexCode)& 0xFF)) / 255.0f;
		}
	};

	//! Utility struct for manipulating vertex attributes.
	/*!
		KVertex is a struct to store the vertex attributes in RAM
		in order to prepare for graphics memory
	*/
    struct KVertex{
		KVector2F32 pos;	//!< Position
		KVector2F32 uv;		//!< Texture UV
		KColor color;		//!< Color


		//! Default constructors
        KVertex():
            pos(), uv(), color()
        {}

		//! Construct the vertex from its 3 attributes.
        KVertex(KVector2F32 Pos, KVector2F32 UV, KColor Color):
            pos(Pos), uv(UV), color(Color)
        {}
    };

	//! Utility struct for representing a atlas object.
	/*!
		KAtlas is a simple and compact struct for managing objects in atlas textures.
	*/
	struct KAtlas{
		U32 id;		//!< Unique ID
		F32 blu;	//!< Bottom-left U texture position with range [0, 1]
		F32 blv;	//!< Bottom-left V texture position with range [0, 1]
		F32 tru;	//!< Top-right U texture position with range [0, 1]
		F32 trv;	//!< Top-right V texture position with range [0, 1]
		F32 w;		//!< Width in pixel
		F32 h;		//!< Height in pixel

		//! Construct the atlas object from its attributes.
		KAtlas(U32 ID = 0, F32 BLU = 0, F32 BLV = 0,
			F32 TRU = 0, F32 TRV = 0, F32 W = 0, F32 H = 0) :
			id(ID),
			blu(BLU), blv(BLV),
			tru(TRU), trv(TRV),
			w(W), h(H)
		{}
	};

	//! State of a key in key-based animations for user classes.
	/*!
		Each channel disable/enable related value.
		each state-change mode, determine how to change values. (add or discard)
	*/
	struct KAnimeValue{
		KVector2F32 translate;	//!< Translate state
		KVector2F32 scale;		//!< Scale state
		F32 rotate;				//!< Rotate state
		KVector2F32 center;		//!< Origin state
		KColor color;			//!< Color state
		KRectF32 uv;			//!< UV state
		bool trChannel;			//!< Translate channel state
		bool scaleChannel;		//!< Scale channel state
		bool rotateChannel;		//!< Rotate channel state
		bool uvChannel;			//!< UV channel state
		bool colorChannel;		//!< Color channel state
		KAnimeValueChangeTypes tchange;	//!< Translate state-change mode
		KAnimeValueChangeTypes schange;	//!< Scale state-change mode
		KAnimeValueChangeTypes rchange;	//!< Rotate state-change mode

		//! Default constructors
		KAnimeValue() :
			translate(), scale(1.0f, 1.0f), rotate(0), center(),
			color(), uv(),
			trChannel(false), scaleChannel(false), rotateChannel(false),
			uvChannel(false), colorChannel(false),
			tchange(KAV_SET), schange(KAV_SET), rchange(KAV_SET)
		{}
	};

	//! State of a key in key-based animations with extra options for animation controller classes.
	/*!
		Each channel disable/enable related value.
		each state-change mode, determine how to change values. (add or discard)
	*/
	struct KAnimeKey{
		F32 time;				//!< Time in milliseconds
		KVector2F32 translate;	//!< Translate state
		KVector2F32 scale;		//!< Scale state
		F32 rotate;				//!< Rotate state
		KVector2F32 center;		//!< Origin state
		U32 uv;					//!< UV state
		KColor color;			//!< Color state
		bool trChannel;			//!< Translate channel state
		bool scaleChannel;		//!< Scale channel state
		bool rotateChannel;		//!< Rotate channel state
		bool uvChannel;			//!< UV channel state
		bool colorChannel;		//!< Color channel state
		KAnimeValueChangeTypes tchange;	//!< Translate state-change mode
		KAnimeValueChangeTypes schange;	//!< Scale state-change mode
		KAnimeValueChangeTypes rchange;	//!< Rotate state-change mode
		KInterpolationTypes tinterp;	//!< Translate interpolation type
		KInterpolationTypes sinterp;	//!< Scale interpolation type
		KInterpolationTypes rinterp;	//!< Rotate interpolation type
		KInterpolationTypes cinterp;	//!< Origin interpolation type

		//! Default constructors
		KAnimeKey() :
			time(0), translate(), scale(1.0f, 1.0f),
			rotate(0.0f), center(), uv(0), color(),
			tinterp(KIN_LINEAR), sinterp(KIN_LINEAR), rinterp(KIN_LINEAR), cinterp(KIN_LINEAR),
			tchange(KAV_SET), schange(KAV_SET), rchange(KAV_SET),
			trChannel(false), scaleChannel(false), rotateChannel(false),
			uvChannel(false), colorChannel(false)
		{}
	};

	//! OpenGL vecrsion holder
    struct KOGLVersion{
        U8 major;	//!< OpenGL major version
        U8 minor;	//!< OpenGL minor version
    };

	//! Particle attribute
    struct KParticle{
		KVector2F32 pos;	//!< Position
		KColor color;		//!< Color
        I32 angle;			//!< Angle
        I32 speed;			//!< Speed
        U32 life;			//!< Life
        U32 size;			//!< Size
    };

	//! Configuration of Batch objects.
	/*!
		Specifies how to configure the vertex buffer objects (VBO).
	*/
    struct KBatchConfig{
		KVertexBufferTypes position;	//!< Position buffer config
		KVertexBufferTypes uv;			//!< UV buffer config
		KVertexBufferTypes color;		//!< Color buffer config

		//! Default constructors
        KBatchConfig(KVertexBufferTypes Position = KVB_STATIC, KVertexBufferTypes UV = KVB_STATIC,
                     KVertexBufferTypes Color = KVB_STATIC):
            position(Position), uv(UV), color(Color)
        {}
    };

	struct KTileMapInfo{
		KTileMapTypes mapType;
		KVector2F32 tileSize;	//!< x = width, y = height
		KVector2U32 mapSize;	//!< x = number of tiles in row, y = number of tiles in column
	};

	//! isometric tile structure for drawing purpose
	struct KIsometricTile{
		U32 textureId;		//!< Texture id (atlas id)
		U32 baseSpace;		//!< Space between texture and ground(y = 0)
		U32 height;			//!< Height of tile
		U16 leftExtraUnit;	//!< Extera width from left (merged tile)
		U16 rightExtraUnit; //!< Extera width from right (merged tile)
		bool stacked;		//!< Is stacked (stakced tile will draw on previous tile)
	};

	/*! \namespace Kite::Internal
		\brief Private namespace.
	*/
    namespace Internal{
		//! Animation time trigger. (internally use)
		struct KAnimeTimeTrigger{
			F32 start;				//!< Start time
			F32 end;				//!< End time
			KCallAnimeTrigger func; //!< Callback
			bool called;			//!< Called once in duration
			void *sender;			//!< Opaque pointer

			//! Default constructors
			KAnimeTimeTrigger(F32 Start = 0, F32 End = 0,
				KCallAnimeTrigger Functor = 0, bool Called = false, void *Sender = 0) :
				start(Start), end(End), func(Functor), called(Called), sender(Sender)
			{}
		};

		//! Catch last OpenGL state. (internally use)
		struct KCatchDraw{
			U32 objIndex;	//!< Index of current object
			U32 lastTexId;	//!< Last used texture id
			U32 lastShdId;	//!< Last used shader id
			KGeoPrimitiveTypes lastGeo;	//!< Last used geometric type

			//! Default constructors
			KCatchDraw(U32 ObjectIndex = 0, U32 LastTextureID = 0, U32 LastSeaderID = 0,
				KGeoPrimitiveTypes GeoType = KGP_TRIANGLES) :
				objIndex(ObjectIndex),
				lastTexId(LastTextureID),
				lastShdId(LastSeaderID),
				lastGeo(GeoType)
			{}
			
			//! Equal operator
			inline bool operator==(const KCatchDraw& rhs){
				if (rhs.lastTexId == lastTexId &&
					rhs.lastShdId == lastShdId &&
					rhs.lastGeo == lastGeo)
					return true;

				return false;
			}

			//! Not equal operator
			inline bool operator!=(const KCatchDraw& rhs){ return !(*this == rhs); }
		};
    }
}

#endif // KGRAPHICSTRUCTS_H
