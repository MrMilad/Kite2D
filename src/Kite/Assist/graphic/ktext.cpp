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
/*#include "Kite/Assist/graphic/ktext.h"

namespace Kite{
	KText::KText(const std::vector<KAtlasObject> &Font) :
		_kfont(&Font),
		_ktext(""),
		_kposition(),
		_kwidth(0.0f),
        _kneedUpdate(true)
    {}

    KText::~KText(){}

	void KText::setPositon(const KVector2F32 &Position){
		KQuad *qtemp;
		_kposition = Position;
		_kwidth = 0.0f;
		for (U32 i = 0; i < _kquads.size(); i++){
			qtemp = &_kquads[i];

			qtemp->setPosition(KVector2F32(_kposition.x + _kwidth, _kposition.y));
			_kwidth += qtemp->getQuadAttribute()->topRight.x;
		}
	}

    const std::vector<KQuad> &KText::getQuads(){
        if (_kneedUpdate){
            const KAtlasObject *atemp;
            KQuad *qtemp;
            KQuadAttrib attrib;
			char ascii;
			
			_kquads.resize(_ktext.size());
            attrib.bottomLeft = KVector2F32(0.0f, 0.0f);

			for (U32 i = 0; i < _kquads.size(); i++){
				ascii = _ktext[i];
				atemp = &_kfont->at(ascii - 32);
                qtemp = &_kquads[i];

                //size
				attrib.topRight.x = atemp->w;
				attrib.topRight.y = atemp->h;

                // uv
				attrib.bottomLeftUV.x = atemp->blu;
				attrib.bottomLeftUV.y = atemp->blv;
				attrib.topRightUV.x = atemp->tru;
				attrib.topRightUV.y = atemp->trv;

                qtemp->setQuadAttribute(attrib);

            }
			// set position
			setPositon(_kposition);
            _kneedUpdate = false;
        }
        return _kquads;
    }
}*/
