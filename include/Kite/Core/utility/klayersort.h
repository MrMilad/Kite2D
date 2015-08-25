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
#ifndef KLAYERSORT_H
#define KLAYERSORT_H

#include "Kite/Core/system/ksystemdef.h"
#include <vector>

namespace Kite {
	class KITE_FUNC_EXPORT KLayerSort {
	public:
		KLayerSort(U32 MaximumLayers);

		// add an object to any layers
		void addObject(U32 LayerNumber);

		// remove an object from any layers
		void removeObject(U32 LayerNumber);

		// get place of added objects
		U32 getObjectPlace(U32 LayerNumber);
		 
		// resize layers (need Reallocating memory if you need greater size)
		void resize(U32 Size);

		// clear all layers
		void clear();

	private:
		std::vector<U32> _klayers;
	};
}

#endif // KLAYERSORT_H