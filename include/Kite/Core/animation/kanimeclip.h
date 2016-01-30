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
#ifndef KANIMECLIP_H
#define KANIMECLIP_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/utility/kresource.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include <vector>

namespace Kite {
	class KITE_FUNC_EXPORT KAnimeClip : public KResource {
	public:
		KAnimeClip();

		// pas KAF_XML for adobeflash XML motion files
		bool loadMemory(const void *Data, std::size_t Size, U32 FileType = 0);

		// pas KAF_XML for adobeflash XML motion files
		bool loadStream(KIStream &Stream, U32 FileType = 0);

		bool saveStream(KOStream &Stream);

		// const access
		inline const std::vector<KAnimeKey> *getKeys() const { return &_kkeys; }

		inline std::vector<KAnimeKey> *getVector() { return &_kkeys; }

		inline const KRect2F32 *getXMLShape() const { return &_kshape; }

		inline void setRelativeXMLPosition(bool Relative) { _krelative = Relative; }
		inline bool getRelativeXMLPosition() const { return _krelative; }

	private:
		bool _loadXML(const std::string &FileName);
		bool _loadXML(KIStream &Stream);
		bool _xmlParser(std::string &content);
		std::vector<KAnimeKey> _kkeys;
		KRect2F32 _kshape;
		bool _krelative;
	};
}


#endif // KANIMECLIP_H