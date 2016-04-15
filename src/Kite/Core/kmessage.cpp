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
#include "Kite/core/kmessage.h"
#include "Kite/core/kcoreutil.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include <luaintf\LuaIntf.h>

namespace Kite {
	KMessage::KMessage() :
		_ksize(0), _kused(0), _khash(0),
		_kdata(nullptr),_kconsume(false), _ktype("")
		{}

	KMessage::KMessage(const std::string &Type) :
		_ksize(0), _kused(0), _khash(0),
		_kdata(nullptr), _kconsume(false), _ktype(Type)
	{
		_khash = getHash32((void *)Type.c_str(), Type.size(), KHASH_SEED);
	}

	KMessage::~KMessage() {}

	void KMessage::setType(const std::string &Type) {
		_khash = getHash32((void *)Type.c_str(), Type.size(), KHASH_SEED);
		_ktype = Type;
	}

	void KMessage::setData(void *Data, SIZE Size) {
		if (Data != nullptr && Size > 0) {
			_kdata = Data;
			_ksize = Size;
		}
	}

	KMessage& KMessage::operator=(const KMessage& other){
		if (this != &other) { // self-assignment check expected
			_khash = other.getHash();
			_kused = other.getUse();
			_ksize = other.getDataSize();
			_kdata = other.getData();
			_kconsume = other.getConsume();
			_ktype = other.getType();
			_kdtable = other._kdtable;
		}
		return *this;
	}

	void KMessage::setDataTable(LuaIntf::LuaRef &Table) {
		if (Table.isValid()) {
			_kdtable = Table;
		}
	}

	KMETA_KMESSAGE_SOURCE();

}