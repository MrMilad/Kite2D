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
#ifndef KMETABASE_H
#define KMETABASE_H

#include "Kite/core/kcoredef.h"
#include "Kite/Meta/kmetatypes.h"
#include <list>
#include <string>

namespace Kite {
	class KITE_FUNC_EXPORT KMetaBase {
	public:
		KMetaBase(const std::string & Name, U32 Flag, U32 Size, KMetaTypes MetaType);

		virtual ~KMetaBase();

		void addInfo(const std::pair<std::string, std::string> &Info);

		inline const auto getInfo() const { return &_kinfo; }
		inline const std::string &getName() const { return _kname; }
		inline U32 getFlag() const { return _kflag; }
		inline U32 getSize() const { return _ksize; }
		inline KMetaTypes getMetaType() const { return _kmetatype; }

	protected:
		std::string _kname;
		U32 _kflag;
		U32 _ksize;
		KMetaTypes _kmetatype;
		std::list<std::pair<std::string, std::string>> _kinfo;
	};
}


#endif // KMETABASE_H