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
#ifndef KMESSAGE_H
#define KMESSAGE_H

#include "Kite/core/kcoredef.h"
#include "Kite/meta/kmetadef.h"
#include <memory>
#include <string>

KMETA
namespace Kite {
	KM_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KMessage{
	public:
		KM_CON()
		KMessage();

		KMessage(const std::string &Type);
		virtual ~KMessage();
		
		KM_PRO_SET("type")
		void setType(const std::string &Type);

		KM_PRO_GET("type", std::string, , "message type")
		inline const std::string &getType() const { return _ktype; }

		KM_PRO_GET("getHash", U32, "32 bit hash code of message type")
		inline U32 getHash() const { return _khash; }

		KM_PRO_GET("getConsume", bool, "consume state of message")
		inline bool getConsume() const { return _kconsume; }

		KM_FUN()
		inline void consume() { _kconsume = true; }

		// increment per component on recieve
		KM_FUN()
		inline void use() { ++_kused; }

		KM_PRO_GET("getUse", U32, "use counter")
		inline U32 getUse() const { return _kused; }

		KM_PRO_GET("getSize", U32, "size of message")
		inline U32 getSize() const { return _ksize; }

		// c++ only
		inline void *getData() const { return _kdata; }
		inline SIZE getDataSize() const { return _ksize; }

		// set massege data. (c++ only)
		void setData(void *Data, SIZE Size);

		// internal use only (in KMessenger)
		inline const std::string &getLuaTable() const { return _ktblName; }

		// copy assignment
		KMessage& operator=(const KMessage& other);

	private:
		// set lua data table. (script only)
		KM_FUN()
		inline void setDataTable(const std::string &TableName) { _ktblName = TableName; }

		U32 _khash;
		U32 _kused;
		SIZE _ksize;
		void *_kdata;
		bool _kconsume;
		std::string _ktype;
		std::string _ktblName;
	};
}

#endif // KMESSAGE_H