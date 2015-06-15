/* 
 Kite2D Game Framework.
 Copyright (C) 2010-2014  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or 
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef KSYSTEMTYPES_H
#define KSYSTEMTYPES_H

namespace Kite{

	/* Power Sourece State */
	enum KPowerStateTypes{
		KPS_ACON,
		KPS_ACOFF,
		KPS_UNKNOWN
	};

	/* Battery State */
	/*enum KBatteryStateTypes{
		KBS_USING,
		KBS_CHARGING,
		KBS_NOBATTERY,
		KBS_UNKNOWN
	};*/

	enum KSeqIOAccTypes{
		KSIOA_READ,
		KSIOA_WRITE,
		KSIOA_APPEND,
		KSIOA_RW,
		KSIOA_RWEX,
		KSIOA_RWTOEND
	};

	enum KVectorComponent{
		KVC_X = 0,
		KVC_Y = 1,
		KVC_Z = 2,
		KVC_W = 3
	};

	enum KResourcesTypes{
		KRT_IMAGE = 0,
		KRT_TEXTURE,
		KRT_AUDIO,
		KRT_VBO,
		KRT_IBO,
		KRT_PBO,
		KRT_OTHER
	};

	/*typedef U64(*KCallInputStreamRead)(void *Data, U64 DataSize);
	typedef I32(*KCallInputStreamSeek)(I64 Offset, I32 Origin);
	typedef I64(*KCallInputStreamTell)();
	typedef bool(*KCallInputStreamIsOpen)();
	typedef I32(*KCallInputStreamEOF)();
	typedef I32(*KCallInputStreamClose)();*/

}
#endif // KSYSTEMTYPES_H
