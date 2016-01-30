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
#include "Kite/core/bsp/klayersort.h"

namespace Kite {
	KLayerSort::KLayerSort(U32 MaximumLayers):
		_klayers(MaximumLayers, 0)
	{}

	void KLayerSort::addObject(U32 LayerNumber) {
		if (LayerNumber < _klayers.size()) {

			// increase number of objects in that layer
			++_klayers[LayerNumber];
		}
	}

	void KLayerSort::removeObject(U32 LayerNumber) {
		if (LayerNumber < _klayers.size()) {
			
			if (_klayers[LayerNumber] > 0) {

				// decrease number of objects in that layer
				--_klayers[LayerNumber];
			}
		}
	}

	U32 KLayerSort::getObjectPlace(U32 LayerNumber) {
		if (LayerNumber < _klayers.size()) {
			U32 place = 0;
			for (U32 i = 0; i <= LayerNumber; i++) {
				place += _klayers[i];
			}
			return place;
		}
		KDEBUG_PRINT("LayerNumber is out of range");
		return 0;
	}

	void KLayerSort::resize(U32 Size) {
		if (Size > _klayers.size()) {
			_klayers.resize(Size);
		}
	}

	void KLayerSort::clear() {
		_klayers.clear();
	}
}