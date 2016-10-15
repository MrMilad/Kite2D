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

#include "Kite/core/kcoredef.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/graphic/kgraphictypes.h"
#include "Kite/math/kmath.h"
#include "Kite/meta/kmetadef.h"
#include "kgraphicstructs.khgen.h"

KMETA
/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{
	KM_CLASS(POD)
	struct KRenderState {
		KMETA_KRENDERSTATE_BODY();

		KM_VAR() bool culling;
		KM_VAR() bool zSorting;
		KM_VAR() bool camDepth;
		KM_VAR() U32 vertexSize;
		KM_VAR() U32 indexSize;
		KM_VAR() U32 objectSize;

		KM_CON()
			KRenderState(bool Culling = false, bool ZSorting = true, bool CamDepth = true,
						 U32 VertexSize = 4000, U32 IndexSize = 6000, U32 ObjectSize = 1000) :
			culling(Culling), zSorting(ZSorting), camDepth(CamDepth),
			vertexSize(VertexSize), indexSize(IndexSize), objectSize(ObjectSize) {}
	};

	//! Utility struct for manipulating RGBAs color.
	/*! 
		KColor is a simple color struct composed of 4 components: r, g, b, a (opacity)
		each component is an byte in the range [0, 255] (set*()/get*()), also in OpenGL Range [0, 1] (setGL*()/getGL*())
	*/
	KM_CLASS(POD)
	struct KColor{
		friend class KRenderSys;
		KMETA_KCOLOR_BODY();
	//! Construct the color from its 4 RGBA components.
	/*!
	Range [0 to 255]
	default value is (255, 255, 255, 255)
	*/
		KM_CON(U8, U8, U8, U8)
		KColor(U8 R = 255, U8 G = 255, U8 B = 255, U8 A = 255):
			r(R), g(G), b(B), a(A) {}

	//! Construct the color from a hexadecimal color code.
	/*!
	A large number of color codes defined in Colors enum
	However, the hexadecimal code can be passed directly
	*/
		KColor(Colors HexCode){
			r = ((U8)(((U32)HexCode >> 16) & 0xFF));
			g = ((U8)(((U32)HexCode >> 8) & 0xFF));
			b = ((U8)(((U32)HexCode)& 0xFF));
			a = 255;
		}

		inline F32 getGLR() const { return r / 255.0f; }
		inline F32 getGLG() const { return g / 255.0f; }
		inline F32 getGLB() const { return b / 255.0f; }
		inline F32 getGLA() const { return a / 255.0f; }

		//! Set R component. range [0 to 255]
		KM_PRO_GET(KP_NAME = "r", KP_TYPE = U8, KP_CM = "R component")
		inline U8 getR() const { return r; }

		//! Set R component. range [0 to 255]
		KM_PRO_SET(KP_NAME = "r")
		inline void setR(U8 R) { r = R; }

		//! Set G component. range [0 to 255]
		KM_PRO_GET(KP_NAME = "g", KP_TYPE = U8, KP_CM = "G component")
		inline U8 getG() const { return g; }

		//! Set G component. range [0 to 255]
		KM_PRO_SET(KP_NAME = "g")
		inline void setG(U8 G) { g = G; }

		//! Set B component. range [0 to 255]
		KM_PRO_GET(KP_NAME = "b", KP_TYPE = U8, KP_CM = "B component")
		inline U8 getB() const { return b; }

		//! Set B component. range [0 to 255]
		KM_PRO_SET(KP_NAME = "b")
		inline void setB(U8 B) { b = B; }

		//! Set A component. range [0 to 255]
		KM_PRO_GET(KP_NAME = "a", KP_TYPE = U8, KP_CM = "A component")
		inline U8 getA() const { return a; }

		//! Set A component. range [0 to 255]
		KM_PRO_SET(KP_NAME = "a")
		inline void setA(U8 A) { a = A; }

		//! lua side function. in C++ use constructure instead.
		KM_FUN()
			static KColor fromName(Colors HexCode) {
			return KColor(HexCode);
		}

		//! Equal operator
		inline bool operator==(const KColor& rhs) const {
			if (rhs.r == r && rhs.g == g &&
				rhs.b == b && rhs.a == a) {
				return true;
			}

			return false;
		}

		//! unequal operator
		inline bool operator!=(const KColor& rhs) const {
			if (rhs.r != r || rhs.g != g ||
				rhs.b != b || rhs.a != a) {
				return true;
			}

			return false;
		}

	private:
		KM_VAR(UNBIND) U8 r;	//!< Red component
		KM_VAR(UNBIND) U8 g;	//!< Green component
		KM_VAR(UNBIND) U8 b;	//!< Blue component
		KM_VAR(UNBIND) U8 a;	//!< Alpha component (opacity)
	};


	//! Utility struct for manipulating vertex attributes.
	/*!
		KVertex is a struct to store the vertex attributes in RAM
		in order to prepare for graphics memory
	*/
	/*KM_CLASS(POD)
    struct KVertex{
		KMETA_KVERTEX_BODY();

		KM_VAR() KVector2F32 pos;	//!< Position
		KM_VAR() KVector2F32 uv;	//!< Texture UV
		KM_VAR() KColor color;		//!< Color


		//! Default constructors
		KM_CON()
        KVertex():
            pos(), uv(), color()
        {}

		//! Construct the vertex from its 3 attributes.
		KVertex(const KVector2F32 &Pos, const KVector2F32 &UV, const KColor &Color) :
            pos(Pos), uv(UV), color(Color)
        {}
    };*/

	struct KGLVertex {
		KVector2F32 pos;
		KVector2F32 uv;
		U8 r;
		U8 g;
		U8 b;
		U8 a;
		U16 tindex;

		KGLVertex() :
			pos(0.0f, 0.0f), uv(0.0f), tindex(0),
			r(0), g(0), b(0), a(255) {}

		friend KBaseSerial &operator<<(KBaseSerial &Out, const KGLVertex &Value) {
			Out << Value.pos;
			Out << Value.uv;
			Out << Value.tindex;
			Out << Value.r;
			Out << Value.g;
			Out << Value.b;
			Out << Value.a;
			return Out;
		}
		friend KBaseSerial &operator>>(KBaseSerial &In, KGLVertex &Value) {
			In >> Value.pos;
			In >> Value.uv;
			In >> Value.tindex;
			In >> Value.r;
			In >> Value.g;
			In >> Value.b;
			In >> Value.a;
			return In;
		}
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
	KM_CLASS(POD)
	struct KAtlasItem{
		KMETA_KATLASITEM_BODY();

		KM_VAR() U32 id;	//!< Unique ID
		KM_VAR() F32 blu;	//!< Bottom-left U texture position with range [0, 1]
		KM_VAR() F32 blv;	//!< Bottom-left V texture position with range [0, 1]
		KM_VAR() F32 tru;	//!< Top-right U texture position with range [0, 1]
		KM_VAR() F32 trv;	//!< Top-right V texture position with range [0, 1]
		KM_VAR() U32 width;	//!< Width in pixel
		KM_VAR() U32 height;//!< Height in pixel
		KM_VAR() U32 xpos;	//!< XPos in pixel
		KM_VAR() U32 ypos;	//!< XPos in pixel

		//! Construct the atlas object from its attributes.
		KM_CON()
		KAtlasItem() :
			id(0),
			blu(0), blv(0),
			tru(1), trv(1),
			width(0), height(0),
			xpos(0), ypos(0),
			_kfliph(false), _kflipv(false){}

		KM_FUN()
		inline void KAtlasItem::flipH() {
			// swap left and right
			F32 temp = blu;
			blu = tru;
			tru = temp;
			_kfliph = !_kfliph;
		}

		KM_FUN()
		inline void KAtlasItem::flipV() {
			// swap bootom and top
			F32 temp = blv;
			blv = trv;
			trv = temp;
			_kflipv = !_kflipv;
		}

		KM_PRO_GET(KP_NAME = "flippedH", KP_TYPE = bool, KP_CM = "horizontal flip")
		inline bool getFlipH() const { return _kfliph; }

		KM_PRO_GET(KP_NAME = "flippedV", KP_TYPE = bool, KP_CM = "vertical flip")
		inline bool getFlipV() const { return _kflipv; }

		//! Equal operator
		inline bool operator==(const KAtlasItem& rhs) {
			if (rhs.xpos == xpos &&
				rhs.ypos == ypos &&
				rhs.width == width &&
				rhs.height == height&&
				rhs._kfliph == _kfliph &&
				rhs._kflipv == _kflipv) {
				return true;
			}
			return false;
		}

	private:
		KM_VAR(UNBIND) bool _kfliph;
		KM_VAR(UNBIND) bool _kflipv;
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
			tchange(0), schange(0), skchange(0), rchange(0)
		{}

		/*friend KBytesArray &operator>>(KBytesArray &In, KAnimeValue &Value) {
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
		}*/
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
			//tinterp(KIN_LINEAR), sinterp(KIN_LINEAR), skinterp(KIN_LINEAR),
			//rinterp(KIN_LINEAR), cinterp(KIN_LINEAR),
			tchange(0), schange(0), skchange(0), rchange(0),
			trChannel(false), scaleChannel(false), skewChannel(false),
			rotateChannel(false), uvChannel(false), colorChannel(false)
		{}

		/*friend KBytesArray &operator>>(KBytesArray &In, KAnimeKey &Value) {
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
		}*/
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
		VBufferType index;		//!< Index buffer config (KVertexBufferTypes)
		VBufferType vertex;		//!< Vertex (position, uv, color) buffer config
		VBufferType point;		//!< Point buffer config ( set it true when you want render particle emitters )

		//! Default constructors
		KBatchConfig(VBufferType Index = VBufferType::STREAM,
					 VBufferType Vertex = VBufferType::STREAM,
					 VBufferType Point = VBufferType::STREAM) :
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

	KM_CLASS(POD)
	struct KOrthoLayer {
		KMETA_KORTHOLAYER_BODY();
		KM_VAR() KColor blend;
		KM_VAR() KAtlasItem atlas;
		KM_VAR() U16 textureID;
		KM_VAR() U16 layerIndex;

#if defined(KITE_EDITOR)
		void *sceneItem;
#endif

		KOrthoLayer() :
#if defined(KITE_EDITOR)
			sceneItem(nullptr),
#endif
			blend(Colors::WHITE),
			textureID(0), layerIndex(0) {}
	};

	KM_CLASS(POD)
	struct KOrthoNode {
		KMETA_KORTHONODE_BODY();
		KM_VAR(UNBIND) bool fliph;
		KM_VAR(UNBIND) bool flipv;
		KM_VAR(UNBIND) U16 layerIndex;
		KM_VAR(UNBIND) U32 atlasID;
		KM_VAR(UNBIND) U32 tileID;
		KM_VAR(UNBIND) KGLVertex verts[4];
		KM_VAR(UNBIND) SIZE nextNode;

#if defined(KITE_EDITOR)
		void *sceneItem;
#endif

		KOrthoNode() :
#if defined(KITE_EDITOR)
			sceneItem(nullptr),
#endif
			fliph(false), flipv(false),
			layerIndex(0),atlasID(0),
			tileID(0), nextNode(0) {}
	};

	KM_CLASS(POD)
	struct KRootTileMap {
		KMETA_KROOTTILEMAP_BODY();

		KM_VAR() SIZE firstNode;
		KM_VAR() U16 layerSize;

		KRootTileMap() :
			firstNode(0), layerSize(0) {}
	};

	KM_CLASS(POD)
	struct KTileStamp {
		KMETA_KTILESTAMP_BODY();

		KM_VAR() KAtlasItem atlas;
		KM_VAR() I16 row;	// releative row to anchor row (-+)
		KM_VAR() I16 col;	// releative column to anchor column (-+)

#if defined(KITE_EDITOR)
		I16 textureIndex;
		KColor blend;
#endif

		KM_CON()
		KTileStamp():
#if defined(KITE_EDITOR)
			textureIndex(-1),
#endif
			row(0), col(0)
		{}
	};

	/*! \namespace Kite::Internal
		\brief Private namespace.
	*/
    namespace Internal{

		//! Catch last OpenGL state. (internally use)
		struct KGLState{
			KColor lastColor;
			KVector2U32 lastViewSize;
			KVector2I32 lastViewPos;
			U32 lastTexId;			//!< Last used texture id
			U32 lastShdId;			//!< Last used shader id
			GLPrimitive lastGeo;	//!< Last used geometric type

			//! Default constructors
			KGLState(U32 LastTextureID = 0, U32 LastSeaderID = 0,
					   GLPrimitive GeoType = GLPrimitive::TRIANGLES) :
				lastTexId(LastTextureID),
				lastShdId(LastSeaderID),
				lastGeo(GeoType)
			{}
			
			//! Equal operator
			inline bool operator==(const KGLState& rhs){
				if (rhs.lastTexId == lastTexId &&
					rhs.lastShdId == lastShdId &&
					rhs.lastGeo == lastGeo) {
					return true;
				}
				return false;
			}

			//! Not equal operator
			inline bool operator!=(const KGLState& rhs){ return !(*this == rhs); }
		};
    }
}

#endif // KGRAPHICSTRUCTS_H
