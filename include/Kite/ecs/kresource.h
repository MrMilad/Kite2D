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
		/// Name: unique resource name
		/// CatchSream: store input stream in resource (stream resource such as audio-source need its input stream for updating its buffers)
		/// IsComposite: resource has dependencies
		KResource(const std::string &Name, bool CatchStream, bool IsComposite);

		virtual ~KResource();

		// SaveDependency = save all sub resources to the stream.
		// *.dep list (names) will be stored separately.
		KM_FUN()
		bool saveStream(KOStream &Stream, const std::string &Address);

		// this function will not load any dependency for composite resources. (use resource manager instead)
		// this function will use an clone of stream in streamed resources
		KM_FUN()
		bool loadStream(KIStream &Stream, const std::string &Address);

		/// second phase initialization. (OpenGL, OpenAL, ... object initialization).
		/// in the simple resources (ex: script) there is no need to implement this function. just return true.
		/// will not called with resource manager.
		virtual bool inite() = 0;

		KM_PRO_GET(KP_NAME = "refCount", KP_TYPE = I32)
		inline I32 getReferencesCount() const { return _kref; }

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
		/// usage: access base in lua
		virtual KResource *getBase() const = 0;

		KM_PRO_GET(KP_NAME = "name", KP_TYPE = std::string)
		inline const std::string &getName() const { return _kname.str; }

		KM_PRO_GET(KP_NAME = "nameID", KP_TYPE = U32)
		inline const U32 &getNameID() const { return _kname.hash; }

		KM_PRO_GET(KP_NAME = "address", KP_TYPE = std::string)
		inline const std::string &getAddress() const { return _kaddress; }

		KM_PRO_GET(KP_NAME = "isComposite", KP_TYPE = bool)
		inline bool isComposite() const { return _kcomposite; }

		KM_PRO_GET(KP_NAME = "isInite", KP_TYPE = bool)
		inline bool isInite() const { return _kisInite; }

		KM_PRO_GET(KP_NAME = "isCatchStream", KP_TYPE = bool)
		inline bool isCatchStream() const { return _kisCatchStream; }

		KM_PRO_GET(KP_NAME = "isOnFly", KP_TYPE = bool)
		inline bool isOnFly() const { return _kisOnFly; }

		KM_PRO_GET(KP_NAME = "catchStream", KP_TYPE = KIStream)
		inline const KIStream *getCatchStream() const { return _kcatchStream; }

	protected:
		inline void setInite(bool Inite) { _kisInite = Inite; }

		// return dependency index
		// all resource must save its dependency indexes
		I32 addDependency(KResource * Resource);
		KResource * getDependency(I32 Index) const;

	private:
		// internal functions (used by manager)
		inline void setName(const std::string &Name) { _kname = KStringID(Name); }
		inline void setAddress(const std::string &Address) { _kaddress = Address; }
		inline void setCatchStream(KIStream *Stream) { _kcatchStream = Stream; }
		inline void setOnFly(bool OnFly) { _kisOnFly = OnFly; }

		// save/load dependencies without resource manager
		bool saveDepList(KOStream &Stream, const std::string &Address);

		// use addDependency() in _saveStream()
		virtual bool _saveStream(KOStream &Stream, const std::string &Address) = 0;

		// use getDependency() in _loadStream()
		// loading resource without resource manager is not allowed
		virtual bool _loadStream(KIStream &Stream, const std::string &Address) = 0;

		// ref counter
		inline void incRef() { ++_kref; }
		inline void decRef() { --_kref; }

		bool _kisCatchStream;
		bool _kisInite;
		bool _kcomposite;
		bool _kisOnFly;
		I32 _kref;
		KStringID _kname;
		std::string _kaddress;
		std::vector<KResource *> _kclist;
		KIStream *_kcatchStream;
	};
}

#endif // KRESOURCE_H