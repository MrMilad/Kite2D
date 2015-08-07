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

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/ksystemtypes.h"
#include "Kite/Core/system/knoncopyable.h"
#include "Kite/Core/utility/kinputstream.h"
#include <cstring>

namespace Kite{
	class KITE_FUNC_EXPORT KResource : KNonCopyable{
	template < class T > friend class KResourceManager;
	public:
		KResource():
			_kref(0)
		{}

		virtual ~KResource(){};

		//! Load the resource from a file on disk
		/*!
		\param FileName Address of the image file on the disk

		\return True if loading was successful
		*/
		virtual bool loadFile(const std::string &FileName) = 0;

		//! Load the resource from a memory block
		/*!

		\param Data Pointer to the file data in memory
		\param Size Size of the data (in bytes)

		\return True if loading was successful
		*/
		virtual bool loadMemory(const void *Data, std::size_t Size) = 0;

		//! Read the resource from input stream.
		/*!
		\param Stream Input stream.

		\return True if loading was successful
		*/
		virtual bool loadStream(KInputStream &Stream) = 0;

		inline U32 getReferencesCount() const { return _kref; }

	protected:
		std::string _kfileName;

	private:
		U32 _kref;
		inline void incRef() { ++_kref; }
		inline void decRef() { _kref > 0 ? --_kref : _kref; }
	};
}

#endif // KRESOURCE_H