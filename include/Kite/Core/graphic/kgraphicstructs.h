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

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Core/math/kmath.h"
#include "Kite/Core/utility/kmetadef.h"
#include "Kite/Core/utility/kutilitytypes.h"
#include "Kite/Core/utility/kproperty.h"
#include "Kite/Core/utility/kbytesarray.h"


/*! \namespace Kite
	\brief Public namespace.
*/
KMETA
namespace Kite{
	//! Utility struct for manipulating RGBAs color.
	/*! 
		KColor is a simple color struct composed of 4 components: r, g, b, a (opacity)
		each component is an byte in the range [0, 255] (set*()/get*()), also in OpenGL Range [0, 1] (setGL*()/getGL*())
	*/
	struct KColor : public KProperty<KColor> {
		//! Construct the color from its 4 RGBA components.
		/*!
	Range [0 to 255]
	default value is (255, 255, 255, 255)
	*/
		KColor(U8 R = 255, U8 G = 255, U8 B = 255, U8 A = 255) :
		r(R / 255.0f), g(G / 255.0f), b(B / 255.0f), a(A / 255.0f) {}

		//! Construct the color from a hexadecimal color code.
		/*!
	A large number of color codes defined in KColorTypes
	However, the hexadecimal code can be passed directly
	*/
		KColor(KColorTypes HexCode) {
			r = ((U8)((HexCode >> 16) & 0xFF)) / 255.0f;
			g = ((U8)((HexCode >> 8) & 0xFF)) / 255.0f;
			b = ((U8)((HexCode)& 0xFF)) / 255.0f;
			a = 1.0;
		}

		inline F32 getGLR() const { return r; }
		//! Set R component. range [0 to 255]
		inline U8 getR() const { return (U8)(r * 255.0f); }

		//! Set R component. range [0 to 255]
		inline void setR(U8 R) { r = (R / 255.0f); }

		inline F32 getGLG() const { return g; }
		//! Set G component. range [0 to 255]
		inline void setG(U8 G) { g = (G / 255.0f); }

		inline F32 getGLB() const { return b; }
		//! Set B component. range [0 to 255]
		inline void setB(U8 B) { b = (B / 255.0f); }

		inline F32 getGLA() const { return a; }
		//! Set A component. range [0 to 255]
		inline void setA(U8 A) { a = (A / 255.0f); }

		//! Set G component. range [0 to 255]
		inline U8 getG() const { return (U8)(g * 255.0f); }

		//! Set B component. range [0 to 255]
		inline U8 getB() const { return (U8)(b * 255.0f); }

		//! Set A component. range [0 to 255]
		inline U8 getA() const { return (U8)(a * 255.0f); }

		KMETACLASS_PROPERTY(U8, r, getR, setR)
		KMETACLASS_PROPERTY(U8, g, getG, setG)
		KMETACLASS_PROPERTY(U8, b, getB, setB)
		KMETACLASS_PROPERTY(U8, a, getA, setA)
		KMETACLASS(KColor)

	private:
		F32 r;	//!< Red component
		F32 g;	//!< Green component
		F32 b;	//!< Blue component
		F32 a;	//!< Alpha component (opacity)
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
		KVertex(const KVector2F32 &Pos, const KVector2F32 &UV, const KColor &Color) :
            pos(Pos), uv(UV), color(Color)
        {}

		//KMETA_DATA(KVertex);
    };

	struct KPointSprite {
		F32 pointSize;
		KVector2F32 textureSize;

		KPointSprite() :
			pointSize(), textureSize() 
		{}

		KPointSprite(F32 PointSize, const KVector2F32 &TextureSize) :
			pointSize(PointSize), textureSize(TextureSize) 
		{}

		//KMETA_DATA(KPointSprite);
	};

	//! Utility struct for representing a atlas object.
	/*!
		KAtlas is a simple and compact struct for managing objects in atlas textures.
	*/
	struct KAtlasItem{
		U32 id;		//!< Unique ID
		F32 blu;	//!< Bottom-left U texture position with range [0, 1]
		F32 blv;	//!< Bottom-left V texture position with range [0, 1]
		F32 tru;	//!< Top-right U texture position with range [0, 1]
		F32 trv;	//!< Top-right V texture position with range [0, 1]
		F32 w;		//!< Width in pixel
		F32 h;		//!< Height in pixel

		//! Construct the atlas object from its attributes.
		KAtlasItem(U32 ID = 0, F32 BLU = 0, F32 BLV = 0,
			F32 TRU = 0, F32 TRV = 0, F32 W = 0, F32 H = 0) :
			id(ID),
			blu(BLU), blv(BLV),
			tru(TRU), trv(TRV),
			w(W), h(H)
		{}

		/*KMETA_DEF(KAtlasItem, KMFLAG_CLASS)
			KMETA_ADD_MEMBER(KAtlasItem, id, U32)
			KMETA_ADD_MEMBER(KAtlasItem, blu, F32)
			KMETA_ADD_MEMBER(KAtlasItem, blv, F32)
			KMETA_ADD_MEMBER(KAtlasItem, tru, F32)
			KMETA_ADD_MEMBER(KAtlasItem, trv, F32)
			KMETA_ADD_MEMBER(KAtlasItem, w, F32)
			KMETA_ADD_MEMBER(KAtlasItem, h, F32)
		KMETA_DEF_END*/
	};

	/*struct KAnimeKey {
		F32 time;
		std::string name;
		KVariant value;
		U8 interp;
		U8 change;

		KAnimeKey() :
			time(0), interp(0), change(0) 
		{}

		friend KBytesArray &operator>>(KBytesArray &In, KAnimeKey &Value) {
			In >> Value.time >> Value.name >> Value.value >> Value.position >>

			return In;
		}
	};*/

	//! State of a key in key-based animations for user classes.
	/*!
		Each channel disable/enable related value.
		each state-change mode, determine how to change values. (add or discard)
	*/
	struct KAnimeValue{
		KVector2F32 translate;	//!< Translate state
		KVector2F32 scale;		//!< Scale state
		KVector2F32 skew;		//!< Skew state
		KVector2F32 position;	//!< Position state
		F32 rotate;				//!< Rotate state
		KVector2F32 center;		//!< Origin state
		KColor color;			//!< Color state
		KRectF32 uv;			//!< UV state
		bool trChannel;			//!< Translate channel state
		bool scaleChannel;		//!< Scale channel state
		bool skewChannel;		//!< Skew channel state
		bool rotateChannel;		//!< Rotate channel state
		bool uvChannel;			//!< UV channel state
		bool colorChannel;		//!< Color channel state
		U8 tchange;		//!< Translate state-change mode (KAnimeValueChangeTypes)
		U8 schange;		//!< Scale state-change mode (KAnimeValueChangeTypes)
		U8 skchange;	//!< Skew state-change mode (KAnimeValueChangeTypes)
		U8 rchange;		//!< Rotate state-change mode (KAnimeValueChangeTypes)

		//! Default constructors
		KAnimeValue() :
			translate(), scale(1.0f, 1.0f), skew(0.0f, 0.0f), position(0.0f, 0.0f),
			rotate(0), center(), color(), uv(),
			trChannel(false), scaleChannel(false), skewChannel(false),
			rotateChannel(false), uvChannel(false), colorChannel(false),
			tchange(KAV_SET), schange(KAV_SET), skchange(KAV_SET), rchange(KAV_SET)
		{}

		friend KBytesArray &operator>>(KBytesArray &In, KAnimeValue &Value) {
			In >> Value.translate >> Value.scale >> Value.skew >> Value.position >>
				Value.rotate >> Value.center >> Value.uv >> Value.trChannel >>
				Value.scaleChannel >> Value.skewChannel >> Value.rotateChannel >>
				Value.uvChannel >> Value.colorChannel >> Value.tchange >>
				Value.schange >> Value.skchange >> Value.rchange;
			return In;
		}

		friend KBytesArray &operator<<(KBytesArray &Out, const KAnimeValue &Value) {
			Out << Value.translate << Value.scale << Value.skew << Value.position <<
				Value.rotate << Value.center << Value.uv << Value.trChannel <<
				Value.scaleChannel << Value.skewChannel << Value.rotateChannel <<
				Value.uvChannel << Value.colorChannel << (U32)Value.tchange <<
				(U32)Value.schange << (U32)Value.skchange << (U32)Value.rchange;
			return Out;
		}
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
		KVector2F32 skew;		//!< Skew state
		KVector2F32 position;	//!< Position state (use it just when you need to specific object position)
		F32 rotate;				//!< Rotate state
		KVector2F32 center;		//!< Origin state
		U32 uv;					//!< UV state
		KColor color;			//!< Color state
		bool trChannel;			//!< Translate channel state
		bool scaleChannel;		//!< Scale channel state
		bool skewChannel;		//!< Skew channel state
		bool rotateChannel;		//!< Rotate channel state
		bool uvChannel;			//!< UV channel state
		bool colorChannel;		//!< Color channel state
		U8 tchange;		//!< Translate state-change mode (KAnimeValueChangeTypes)
		U8 schange;		//!< Scale state-change mode (KAnimeValueChangeTypes)
		U8 skchange;	//!< Skew state-change mode (KAnimeValueChangeTypes)
		U8 rchange;		//!< Rotate state-change mode (KAnimeValueChangeTypes)
		U8 tinterp;		//!< Translate interpolation type (KAnimeValueChangeTypes)
		U8 sinterp;		//!< Scale interpolation type (KInterpolationTypes)
		U8 skinterp;	//!< Skew interpolation type
		U8 rinterp;		//!< Rotate interpolation type
		U8 cinterp;		//!< Origin interpolation type

		//! Default constructors
		KAnimeKey() :
			time(0), translate(), scale(1.0f, 1.0f), position(0.0f, 0.0f),
			skew(0.0f, 0.0f), rotate(0.0f), center(), uv(0), color(),
			tinterp(KIN_LINEAR), sinterp(KIN_LINEAR), skinterp(KIN_LINEAR),
			rinterp(KIN_LINEAR), cinterp(KIN_LINEAR),
			tchange(KAV_SET), schange(KAV_SET), skchange(KAV_SET), rchange(KAV_SET),
			trChannel(false), scaleChannel(false), skewChannel(false),
			rotateChannel(false), uvChannel(false), colorChannel(false)
		{}

		friend KBytesArray &operator>>(KBytesArray &In, KAnimeKey &Value) {
			In >> Value.translate >> Value.scale >> Value.skew >> Value.position >>
				Value.rotate >> Value.center >> Value.uv >> Value.trChannel >>
				Value.scaleChannel >> Value.skewChannel >> Value.rotateChannel >>
				Value.uvChannel >> Value.colorChannel >> Value.tchange >>
				Value.schange >> Value.skchange >> Value.rchange >>
				Value.tinterp >> Value.sinterp >> Value.skinterp >>
				Value.rinterp >> Value.cinterp;
			return In;
		}

		friend KBytesArray &operator<<(KBytesArray &Out, const KAnimeKey &Value) {
			Out << Value.translate << Value.scale << Value.skew << Value.position <<
				Value.rotate << Value.center << Value.uv << Value.trChannel <<
				Value.scaleChannel << Value.skewChannel << Value.rotateChannel <<
				Value.uvChannel << Value.colorChannel << Value.tchange <<
				Value.schange << Value.skchange << Value.rchange <<
				Value.tinterp << Value.sinterp << Value.skinterp <<
				Value.rinterp << Value.cinterp;
			return Out;
		}
	};

	//! OpenGL vecrsion holder
    struct KOGLVersion{
        U8 major;	//!< OpenGL major version
        U8 minor;	//!< OpenGL minor version

		KOGLVersion(U8 Major, U8 Minor) :
			major(Major), minor(Minor) 
		{}

		//KMETA_DATA(KOGLVersion);
    };

	//! Particle attribute
    struct KParticle{
		KVector2F32 pos;	//!< Position
		F32 size;			//!< Size
		F32 angle;			//!< Angle
		KColor color;		//!< Color
        F32 speed;			//!< Speed
        F32 life;			//!< Life
		F32 timer;			//!< Timer
		KRectF32 uv;		//!< Texture uv
		F32 spin;			//!< Spin

		KParticle() :
			pos(), size(), angle(0),
			color(), speed(0), life(0),
			timer(0), uv(), spin(0)
		{}

		KParticle(KVector2F32 Pos, F32 Size, F32 Angle,
				  KColor Color, F32 Speed, F32 Life,
				  F32 Timer, KRectF32 UV) :
				  pos(Pos), size(Size), angle(Angle),
				  color(Color), speed(Speed), life(Life),
				  timer(Timer), uv(UV)
		{}

		//KMETA_DATA(KParticle);
    };

	//! Configuration of Batch objects.
	/*!
		Specifies how to configure the vertex buffer objects (VBO).
	*/
    struct KBatchConfig{
		U8 index;		//!< Index buffer config (KVertexBufferTypes)
		U8 vertex;		//!< Vertex (position, uv, color) buffer config
		U8 point;		//!< Point buffer config ( set it true when you want render particle emitters )

		//! Default constructors
		KBatchConfig(KVertexBufferTypes Index = KVB_STREAM,
					 KVertexBufferTypes Vertex = KVB_STREAM,
					 KVertexBufferTypes Point = KVB_STREAM) :
            index(Index), vertex(Vertex) , point(Point)
        {}

		//KMETA_DATA(KBatchConfig);
    };

	struct KBatchUpdate{
		bool vertex;	//!< Enable/Disable updating vertex (position, uv, color) buffer
		bool point;		//!< Enable/Disable updating point (uv, size) buffer

		//! Default constructors
		KBatchUpdate(bool Vertex = true,
					 bool Point = true) :
					 vertex(Vertex),
					 point(Point)
		{}

		//KMETA_DATA(KBatchUpdate);
	};

	//! isometric tile structure for drawing purpose
	/*struct KIsometricTile{
		U32 textureId;		//!< Texture id (atlas id)
		U32 baseSpace;		//!< Space between texture and ground(y = 0)
		U32 height;			//!< Height of tile
		U16 leftExtraUnit;	//!< Extera width from left (merged tile)
		U16 rightExtraUnit; //!< Extera width from right (merged tile)
		bool stacked;		//!< Is stacked (stakced tile will draw on previous tile)
	};*/

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

		struct KUpdateSender {
			U32 arraySize;
			const void *firstObject;

			KUpdateSender(U32 ArraySize = 0, void *FirstObject = 0) :
				arraySize(ArraySize), firstObject(FirstObject) 
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
