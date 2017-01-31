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
#include "Kite/core/kcoreutil.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/serialization/kserialization.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf\LuaIntf.h>
#include "Kite/ecs/kecsstructs.h"

KMETA
namespace Kite {
	KMETA_KHANDLE_SOURCE();
	//KMETA_KFILEINFO_SOURCE();

	KStringID::KStringID(const std::string &String) :
		str(String) {
		hash = getHash32(str.c_str(), str.size(), KHASH_SEED);
	}

	KStringID::KStringID() :
		str(""), hash(0) {}
	KMETA_KSTRINGID_SOURCE();

	KBitset::KBitset(U32 Size, const std::string &Value) :
		_kall(false), _kany(false), _knone(true),
		_kcount(0), _kvalue(Size, '0') 
	{
		SIZE size = Size;
		if (Value.size() < size) size = Value.size();

		for (SIZE i = 0; i < size; ++i) {
			if (*(Value.rbegin() + i) == '1') {
				*(_kvalue.rbegin() + i) = '1';
			}
		}

		proc();
	}

	bool KBitset::test(U32 Pos) const {
		if (Pos < _kvalue.size()) {
			if (*(_kvalue.rbegin() + Pos) == '1') return true;
			return false;
		}

		KD_PRINT("bit position is out of range.");
		return false;
	}

	void KBitset::andWith(const KBitset &Other) {
		SIZE size = _kvalue.size();
		if (Other.size() < size) size = Other.size();

		for (SIZE i = 0; i < size; ++i) {
			U8 v = (_kvalue[i] - '0') & (Other._kvalue.at(i) - '0');
			_kvalue[i] = '0' + v;
		}

		proc();
	}

	void KBitset::orWith(const KBitset &Other) {
		SIZE size = _kvalue.size();
		if (Other.size() < size) size = Other.size();

		for (SIZE i = 0; i < size; ++i) {
			U8 v = (_kvalue[i] - '0') | (Other._kvalue.at(i) - '0');
			_kvalue[i] = '0' + v;
		}

		proc();
	}

	void KBitset::xorWith(const KBitset &Other) {
		SIZE size = _kvalue.size();
		if (Other.size() < size) size = Other.size();

		for (SIZE i = 0; i < size; ++i) {
			U8 v = (_kvalue[i] - '0') ^ (Other._kvalue.at(i) - '0');
			_kvalue[i] = '0' + v;
		}

		proc();
	}

	void KBitset::notWith(const KBitset &Other) {
		SIZE size = _kvalue.size();
		if (Other.size() < size) size = Other.size();

		for (SIZE i = 0; i < size; ++i) {
			U8 v = !(_kvalue[i] - '0');
			_kvalue[i] = '0' + v;
		}

		proc();
	}

	void KBitset::setAll(bool Value) {
		if (Value) {
			std::fill_n(_kvalue.begin(), _kvalue.size(), '1');
			_kall = true;
			_kany = true;
			_kcount = _kvalue.size();
			_knone = false;
		} else {
			std::fill_n(_kvalue.begin(), _kvalue.size(), '0');
			_kall = false;
			_kany = false;
			_kcount = 0;
			_knone = true;
		}

		proc();
	}

	void KBitset::set(U32 Pos, bool Value) {
		if (Pos < _kvalue.size()) {
			if (Value) {
				*(_kvalue.rbegin() + Pos) = '1';
			} else {
				*(_kvalue.rbegin() + Pos) = '0';
			}

			proc();
		}

		KD_PRINT("bit position is out of range");
	}

	void KBitset::flipAll() {
		for (auto it = _kvalue.begin(); it != _kvalue.end(); ++it) {
			U8 v = (*it) - '0';
			(*it) = (!v) + '0';
		}

		proc();
	}

	void KBitset::flip(U32 Pos) {
		if (Pos < _kvalue.size()) {
			U8 v = *(_kvalue.rbegin() + Pos) - '0';
			*(_kvalue.rbegin() + Pos) = (!v) + '0';
			proc();
		}

		KD_PRINT("bit position is out of range");
	}

	void KBitset::proc() {
		_kcount = 0;
		for (auto it = _kvalue.begin(); it != _kvalue.end(); ++it) {
			if ((*it) == '1') ++_kcount;
		}

		if (_kcount == 0) {
			_kall = false;
			_kany = false;
			_knone = true;

		} else if (_kcount == _kvalue.size()){
			_kall = true;
			_kany = true;
			_knone = false;

		} else {
			_kall = false;
			_kany = true;
			_knone = false;
		}
	}
	KMETA_KBITSET_SOURCE();

	KLayerInfo::KLayerInfo(const std::string &Name, bool IsBuiltin):
		name(Name), isBuiltin(IsBuiltin){}
	KMETA_KLAYERINFO_SOURCE();
	KMETA_KTARCKITEM_SOURCE();
}