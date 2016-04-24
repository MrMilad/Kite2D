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
		KResource(const std::string &Name, const std::string &Type);

		virtual ~KResource();

		//! Read the resource from input stream.
		/*!
		\param Stream Input stream.

		\return True if loading was successful
		*/
		virtual bool loadStream(KIStream *Stream, U32 Flag = 0) = 0;

		virtual bool saveStream(KOStream *Stream, U32 Flag = 0) = 0;

		inline U32 getReferencesCount() const { return _kref; }

		KM_PRO_GET("name", std::string)
		inline const std::string &getResourceName() const { return _kname; }

		KM_PRO_GET("type", std::string)
		inline const std::string &getResourceType() const { return _ktype; }

	private:
		// internal use by resource manager
		inline void setResourceName(const std::string &Name) { _kname = Name; }
		inline void incRef() { ++_kref; }
		inline void decRef() { _kref > 0 ? --_kref : _kref; }

		U32 _kref;
		std::string _ktype;
		std::string _kname;
	};
}

#endif // KRESOURCE_H