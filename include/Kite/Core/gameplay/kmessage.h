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

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/kobject.h"
#include "Kite/Core/system/krefvariant.h"
#include "Kite/Core/meta/kmetadef.h"
#include <memory>
#include <string>
#include <kmessage.khgen.h>

KMETA
namespace Kite {
	KMETA_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KMessage{
	public:
		KMETA_CONSTRUCTURE()
		KMessage();

		KMessage(const std::string &Type);
		virtual ~KMessage();
		
		KMETA_PROPERTY("type", "message type")
		void setType(const std::string &Type);

		KMETA_PROPERTY("type")
		inline const std::string &getType() const { return _ktype; }

		KMETA_PROPERTY("getHash", "32 bit hash code of message type")
		inline U32 getHash() const { return _khash; }

		KMETA_FUNCTION()
		inline void consume() { _kconsume = true; }

		KMETA_PROPERTY("getConsume", "consume state of message")
		inline bool getConsume() const { return _kconsume; }

		// increment per component on recieve
		KMETA_FUNCTION()
		inline void use() { ++_kused; }

		KMETA_PROPERTY("getUse", "use counter")
		inline U32 getUse() const { return _kused; }

		KMETA_PROPERTY("getSize", "size of message")
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

		KMETA_KMESSAGE_BODY()
	private:
		// set lua data table. (script only)
		KMETA_FUNCTION()
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