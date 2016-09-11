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
#include "Kite/core/knoncopyable.h"
#include "Kite/core/kistream.h"
#include "Kite/core/kostream.h"
#include "Kite/meta/kmetadef.h"
#include <cstring>
#include <vector>
#include "ktypes.khgen.h"
#include "kresource.khgen.h"

KMETA
namespace Kite{
	KM_CLASS(RESOURCE, ABSTRACT)
	class KITE_FUNC_EXPORT KResource : KNonCopyable{
	friend class KResourceManager;
	KMETA_KRESOURCE_BODY();
	public:
		KResource(const std::string &Name, bool CatchStream, bool IsComposite);

		virtual ~KResource();

		bool saveStream(KOStream &Stream, const std::string &Address, bool SaveDependency = false);

		/// second phase initialization. (OpenGL, OpenAL, ... object initialization).
		/// in the simple resources (ex: script) there is no need to implement this function. just return true.
		/// will not called with resource manager.
		virtual bool inite() = 0;

		inline U32 getReferencesCount() const { return _kref; }

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "type", KP_TYPE = RTypes)
			virtual inline RTypes getType() const = 0;

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "typeName", KP_TYPE = std::string)
			virtual inline std::string getTypeName() const = 0;

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "hashType", KP_TYPE = std::string)
			virtual inline U32 getHashType() const = 0;

		/// will be implemented by KHParser
		/// usage: access base members in lua
		virtual KResource *getBase() = 0;

		KM_PRO_GET(KP_NAME = "name", KP_TYPE = std::string)
		inline const std::string &getName() const { return _kname.str; }

		KM_PRO_GET(KP_NAME = "nameID", KP_TYPE = KStringID)
		inline const KStringID &getNameID() const { return _kname; }

		KM_PRO_GET(KP_NAME = "address", KP_TYPE = std::string)
		inline const std::string &getAddress() const { return _kaddress; }

		KM_PRO_GET(KP_NAME = "isComposite", KP_TYPE = bool)
		inline bool isComposite() const { return _kcomposite; }

		KM_PRO_GET(KP_NAME = "isInite", KP_TYPE = bool)
		inline bool isInite() const { return _kisInite; }

		KM_PRO_GET(KP_NAME = "catchStream", KP_TYPE = bool)
		inline bool getCatchStream() const { return _kcatchStream; }

#ifdef KITE_EDITOR
		/// main usage: for saving modified resource in editor
		inline bool isModified() { bool temp = _kisModified; _kisModified = false;  return temp; }
		inline void setModified(bool Modify) { _kisModified = Modify; }
#endif

	protected:
		inline void setInite(bool Inite) { _kisInite = Inite; }
		inline void setCompositeList(std::vector<KResource *> List) { _kclist = List; }
		inline std::vector<KResource *> getCompositeList() const { return _kclist; }

	private:
		inline void setName(const std::string &Name) { _kname = KStringID(Name); }
		inline void setAddress(const std::string &Address) { _kaddress = Address; }
		bool saveCompositeList(KOStream &Stream, const std::string &Address, bool SaveDependency);

		/// use setCompositeList() in _saveStream()
		virtual bool _saveStream(KOStream &Stream, const std::string &Address) = 0;

		/// use getCompositeList() in _loadStream()
		/// loading resource without resource manager is not allowed
		virtual bool _loadStream(KIStream &Stream, const std::string &Address) = 0;

		inline void incRef() { ++_kref; }
		inline void decRef() { _kref > 0 ? --_kref : _kref; }

		bool _kcatchStream;
		bool _kisModified;
		bool _kisInite;
		bool _kcomposite;
		U32 _kref;
		KStringID _kname;
		std::string _kaddress;
		std::vector<KResource *> _kclist;
	};
}

#endif // KRESOURCE_H