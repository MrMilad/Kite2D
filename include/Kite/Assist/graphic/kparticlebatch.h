/*
Kite2D Game Framework.
Copyright (C) 2010-2014  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef KPARTICLEBATCH_H
#define KPARTICLEBATCH_H

#include "Kite/Core/system/ksystemtypes.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/kshader.h"
#include "Kite/Core/graphic/ktexture.h"
#include "Kite/Core/graphic/kvertexarray.h"
#include "Kite/Core/graphic/kvertexbuffer.h"
#include "Kite/Assist/graphic/kdrawable.h"
#include "Kite/Assist/graphic/kparticleemitter.h"

namespace Kite{
	class KITE_FUNC_EXPORT KParticleBatch : public KDrawable{
	public:

	private:
		const KShader *_kshader;
		const KTexture *_ktexture;
	};
}

#endif // KPARTICLEBATCH_H