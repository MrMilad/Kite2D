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
#include "Kite/core/kcorestructs.h"
#include "Kite/core/kistream.h"
#include "Kite/core/kostream.h"
#include "Kite/meta/kmetadef.h"
#include <cstring>
#include <vector>
#include <memory>
#include "ktypes.khgen.h"
#include "kresource.khgen.h"

KMETA
namespace Kite{
	class KResourceManager;
	KM_CLASS(RESOURCE, ABSTRACT)
	class KITE_FUNC_EXPORT KResource{
		friend class KResourceManager;
		KM_INFO(KI_NAME = "ResourceBase");
		KRESOURCE_BODY();
	public:
		/// all drived resources can constructed inside an SharedResource in lua
		KResource(const std::string &Name, const std::string &Address);

		KResource() = delete;
		KResource(const KResource &Copy) = delete;
		KResource &operator=(const KResource &Right) = delete;

		virtual ~KResource();

		KM_FUN()
		virtual bool saveStream(KOStream &Stream, const std::string &Address) = 0;

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "drivedType", KP_TYPE = rtypes)
		virtual inline Resource getDrivedType() const = 0;

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "typeName", KP_TYPE = std::string)
		virtual inline const std::string &getTypeName() const = 0;

		/// will be implemented by KHParser
		/// usage: access base in lua
		//virtual KResource *getBase() const = 0;

		KM_PRO_GET(KP_NAME = "resourceName", KP_TYPE = std::string)
		inline const std::string &getResourceName() const { return _kname; }

		KM_PRO_GET(KP_NAME = "address", KP_TYPE = std::string)
		inline const std::string &getAddress() const { return _kaddress; }

	protected:
		// loading resource without resource manager is not allowed
		// use RManager only for loading dependencies
		// you can catch Stream (ex: stream lined resource) or free it
		// use address variable for input address
		// this fuction should be used only once and re-loading a resource object is not allowed (undefined behavior)
		virtual bool _loadStream(std::unique_ptr<KIStream> Stream, KResourceManager *RManager) = 0;

	private:
		const std::string _kname;
		const std::string _kaddress;
	};
}

#endif // KRESOURCE_H