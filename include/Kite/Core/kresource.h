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
#ifndef KRESOURCE_H
#define KRESOURCE_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcoretypes.h"
#include "Kite/core/knoncopyable.h"
#include "Kite/core/kistream.h"
#include "Kite/core/kostream.h"
#include "Kite/meta/kmetadef.h"
#include <cstring>
#include "kresource.khgen.h"

KMETA
namespace Kite{
	KM_CLASS(RESOURCE, ABSTRACT)
	class KITE_FUNC_EXPORT KResource : KNonCopyable{
	friend class KResourceManager;

	KMETA_KRESOURCE_BODY();
	public:
		KResource(const std::string &Name);

		virtual ~KResource();

		//! Read the resource from input stream.
		/*!
		\param Stream Input stream.

		\return True if loading was successful
		*/
		virtual bool loadStream(KIStream *Stream, const std::string &Address, U32 Flag = 0) = 0;

		virtual bool saveStream(KOStream *Stream, const std::string &Address, U32 Flag = 0) = 0;

		inline U32 getReferencesCount() const { return _kref; }

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "type", KP_TYPE = std::string)
			virtual inline std::string getType() const = 0;

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "hashType", KP_TYPE = std::string)
			virtual inline U32 getHashType() const = 0;

		/// will be implemented by KHParser
		/// usage: access base members in lua
		virtual KResource *getBase() = 0;

		KM_PRO_GET(KP_NAME = "name", KP_TYPE = std::string)
		inline const std::string &getName() const { return _kname; }

		KM_PRO_SET(KP_NAME = "name")
		inline void setName(const std::string &Name) { _kname = Name; }

		KM_PRO_GET(KP_NAME = "address", KP_TYPE = std::string)
		inline const std::string &getAddress() const { return _kaddress; }

		KM_PRO_SET(KP_NAME = "address")
		inline void setAddress(const std::string &Address) { _kaddress = Address; }

	private:
		inline void incRef() { ++_kref; }
		inline void decRef() { _kref > 0 ? --_kref : _kref; }

		U32 _kref;
		std::string _kname;
		std::string _kaddress;
	};
}

#endif // KRESOURCE_H