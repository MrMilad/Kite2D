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
#include "Kite/core/graphic/kgraphicstructs.h"

namespace Kite {
	KMETA_DEF(KColor, KMFLAG_CLASS) {
		KMETA_ADD_MEMBER(r);
		KMETA_ADD_MEMBER(g);
		KMETA_ADD_MEMBER(b);
		KMETA_ADD_MEMBER(a);
	}

	KMETA_DEF(KVertex, KMFLAG_CLASS) {
		KMETA_ADD_MEMBER(pos);
		KMETA_ADD_MEMBER(uv);
		KMETA_ADD_MEMBER(color);
	}

	KMETA_DEF(KPointSprite, KMFLAG_CLASS) {
		KMETA_ADD_MEMBER(pointSize);
		KMETA_ADD_MEMBER(textureSize);
	}

	KMETA_DEF(KAtlasItem, KMFLAG_CLASS) {
		KMETA_ADD_MEMBER(id);
		KMETA_ADD_MEMBER(blu);
		KMETA_ADD_MEMBER(blv);
		KMETA_ADD_MEMBER(tru);
		KMETA_ADD_MEMBER(trv);
		KMETA_ADD_MEMBER(w);
		KMETA_ADD_MEMBER(h);
	}

	KMETA_DEF(KOGLVersion, KMFLAG_CLASS) {
		KMETA_ADD_MEMBER(major);
		KMETA_ADD_MEMBER(minor);
	}	

	KMETA_DEF(KParticle, KMFLAG_CLASS) {
		KMETA_ADD_MEMBER(pos);
		KMETA_ADD_MEMBER(size);
		KMETA_ADD_MEMBER(angle);
		KMETA_ADD_MEMBER(color);
		KMETA_ADD_MEMBER(speed);
		KMETA_ADD_MEMBER(life);
		KMETA_ADD_MEMBER(timer);
		KMETA_ADD_MEMBER(uv);
		KMETA_ADD_MEMBER(spin);
	}

	KMETA_DEF(KBatchConfig, KMFLAG_CLASS) {
		KMETA_ADD_MEMBER(index);
		KMETA_ADD_MEMBER(vertex);
		KMETA_ADD_MEMBER(point);
	}

	KMETA_DEF(KBatchUpdate, KMFLAG_CLASS) {
		KMETA_ADD_MEMBER(vertex);
		KMETA_ADD_MEMBER(point);
	}

	KMETA_DEF(KTileMapInfo, KMFLAG_CLASS) {
		KMETA_ADD_MEMBER(mapType);
		KMETA_ADD_MEMBER(tileSize);
		KMETA_ADD_MEMBER(mapSize);
	}
}
