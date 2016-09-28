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
#ifndef KATLASTEXTUREARRAY_H
#define KATLASTEXTUREARRAY_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kresource.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/graphic/kgraphictypes.h"
#include "Kite/graphic/katlastexture.h"
#include "katlastexturearray.khgen.h"
#include <map>

/*! \namespace Kite
\brief Public namespace.
*/
KMETA
namespace Kite {

	KM_CLASS(RESOURCE)
	class KITE_FUNC_EXPORT KAtlasTextureArray : public KResource {
		KM_INFO(KI_NAME = "TextureGroup");
		KMETA_KATLASTEXTUREARRAY_BODY();
	public:
		KAtlasTextureArray(const std::string &Name);
		~KAtlasTextureArray();

		bool inite() override;

		// diffrent size = clearing continer
		void setTextureSize(const KVector2U32 &Size);

		inline U32 getTextureWidth() const { return _ksize.x; }

		inline U32 getTextureHeight() const { return _ksize.y; }

		inline const KVector2U32 &getTextureSize() const { return _ksize; }

		bool addItem(KAtlasTexture *Atlas);

		inline const KAtlasTexture *getItem(U32 ID) const { return _karray.at(ID); }

		inline void clearItems() { _karray.clear(); }

		inline U32 getSize() const { return _karray.size(); }

		inline TextureFilter getFilter() const { return _kfilter; }

		inline TextureWrap getWrap() const { return _kwrap; }

		inline U32 getGLID() const { return _ktexId; }

		void setFilter(TextureFilter Filter);

		void setWrap(TextureWrap Wrap);

		void bind() const;

		void unbind();

		//! Unbind current texture
		static void unbindTextureArray();

#if defined(KITE_EDITOR)
		inline auto getContiner() { return &_karray; }
#endif

	private:
		bool _saveStream(KOStream &Stream, const std::string &Address) override;
		bool _loadStream(KIStream &Stream, const std::string &Address) override;

		//! create the texture (opengl)
		static void _create(const U8 *Data, const KVector2U32 &Size,
							TextureFilter Filter, TextureWrap Wrap, KAtlasTextureArray *Instance);

		U32 _ktexId;			//!< ogl array texture name
		KVector2U32 _ksize;
		TextureFilter _kfilter;	//!< Texture interpolation
		TextureWrap _kwrap;		//!< Texture wrapping
		std::vector<KAtlasTexture *> _karray;
		static U32 _klastTexId;	//!< Static last texture ID
	};
}

#endif // KATLASTEXTURE_H
