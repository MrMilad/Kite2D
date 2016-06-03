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
#include "Kite/core/kcoreutil.h"
#include <hash\mmhash3.h>
#include "ctime"

namespace Kite{
void cpuid(U32 CPUInfo[4],I32 InfoType){
#ifdef _MSC_VER
	__cpuid((int *)CPUInfo, (int)InfoType);
#else
	__asm__ __volatile__(
		"cpuid":
		"=a" (CPUInfo[0]),
		"=b" (CPUInfo[1]),
		"=c" (CPUInfo[2]),
		"=d" (CPUInfo[3]) :
		"a" (InfoType)
		);
#endif
    }

    U32 getCurrentTimeSec(){
        time_t rawtime;
        struct tm * timeinfo;

        time( &rawtime );
        timeinfo = localtime ( &rawtime );
        return timeinfo->tm_sec;
    }

    U32 getCurrentTimeMin(){
        time_t rawtime;
        struct tm * timeinfo;

        time( &rawtime );
        timeinfo = localtime ( &rawtime );
        return timeinfo->tm_min;
    }

    U32 getCurrentTimeHours(){
        time_t rawtime;
        struct tm * timeinfo;

        time( &rawtime );
        timeinfo = localtime ( &rawtime );
        return timeinfo->tm_hour;
    }

    const KCPUInfo *getCPUInfo(){
        static KCPUInfo cpuInfo;

        unsigned int info[4];
        cpuid(info, 0);
        unsigned int nIds = info[0];

        cpuid(info, 0x80000000);
        unsigned int nExIds = info[0];

        //  Detect Instruction Set
        if (nIds >= 1){
            cpuid(info,0x00000001);
            cpuInfo.mmx   = (info[3] & ((int)1 << 23)) != 0;
            cpuInfo.sse   = (info[3] & ((int)1 << 25)) != 0;
            cpuInfo.sse2  = (info[3] & ((int)1 << 26)) != 0;
            cpuInfo.sse3  = (info[2] & ((int)1 <<  0)) != 0;

            cpuInfo.ssse3 = (info[2] & ((int)1 <<  9)) != 0;
            cpuInfo.sse41 = (info[2] & ((int)1 << 19)) != 0;
            cpuInfo.sse42 = (info[2] & ((int)1 << 20)) != 0;

            cpuInfo.avx   = (info[2] & ((int)1 << 28)) != 0;
            cpuInfo.fma3  = (info[2] & ((int)1 << 12)) != 0;
        }

        if (nExIds >= 0x80000001){
            cpuid(info,0x80000001);
            cpuInfo.x64   = (info[3] & ((int)1 << 29)) != 0;
            cpuInfo.sse4a = (info[2] & ((int)1 <<  6)) != 0;
            cpuInfo.fma4  = (info[2] & ((int)1 << 16)) != 0;
            cpuInfo.xop   = (info[2] & ((int)1 << 11)) != 0;
        }
        return &cpuInfo;
    }

    U64 getUniqueNumber(){
        static U64 id = 0;
        return ++id;
    }

	F64 convertStrToDouble(const char *p) {
		double r = 0.0;
		bool neg = false;
		if (*p == '-') {
			neg = true;
			++p;
		}
		while (*p >= '0' && *p <= '9') {
			r = (r*10.0) + (*p - '0');
			++p;
		}
		if (*p == '.') {
			double f = 0.0;
			int n = 0;
			++p;
			while (*p >= '0' && *p <= '9') {
				f = (f*10.0) + (*p - '0');
				++p;
				++n;
			}
			r += f / std::pow(10.0, n);
		}
		if (neg) {
			r = -r;
		}
		return r;
	}

	bool isBigEndian() {
		const I32 i = 1;
		return ((*(char*)&i) == 0);
	}

	U32 getHash32(const void *Data, U32 Lenght, U32 Seed) {
		U32 out = 0;
		MurmurHash3_x86_32(Data, Lenght, Seed, (void *)&out);
		return out;
	}

	void getHash128(const void *Data, U32 Lenght, U32 Seed, void *Out) {
		MurmurHash3_x86_128(Data, Lenght, Seed, Out);
	}
}
