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
#include "Kite/Core/graphic/ktransformablegroup.h"

namespace Kite{
	void KTransformableGroup::addObject(KTransformable *Object){
		if (Object){
			_klist.push_back(Object);
		}
	}

	void KTransformableGroup::removeAllObjects(){
		_klist.clear();
	}

	void KTransformableGroup::setPosition(const KVector2F32& Position){
		for (U32 i = 0; i < _klist.size(); i++){
			_klist[i]->setPosition(Position);
		}
	}

	void KTransformableGroup::move(const KVector2F32 &Steps){
		for (U32 i = 0; i < _klist.size(); i++){
			_klist[i]->move(Steps);
		}
	}

	void KTransformableGroup::setRotation(F32 Angle){
		for (U32 i = 0; i < _klist.size(); i++){
			_klist[i]->setRotation(Angle);
		}
	}

	void KTransformableGroup::rotate(F32 Angle){
		for (U32 i = 0; i < _klist.size(); i++){
			_klist[i]->rotate(Angle);
		}
	}

	void KTransformableGroup::setScale(const KVector2F32 &Scale){
		for (U32 i = 0; i < _klist.size(); i++){
			_klist[i]->setScale(Scale);
		}
	}

	void KTransformableGroup::scale(const KVector2F32 &Scale){
		for (U32 i = 0; i < _klist.size(); i++){
			_klist[i]->scale(Scale);
		}
	}

	void KTransformableGroup::setCenter(const KVector2F32 &Center){
		for (U32 i = 0; i < _klist.size(); i++){
			_klist[i]->setCenter(Center);
		}
	}
}