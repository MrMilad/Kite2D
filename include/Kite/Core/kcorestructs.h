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
#ifndef KCORESTRUCTS_H
#define KCORESTRUCTS_H

#include "Kite/core/kcoretypes.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/serialization/kbaseserial.h"
#include "Kite/serialization/types/kstdvector.h"
#include "Kite/serialization/kserialization.h"
#include <string>
#include "kcorestructs.khgen.h"

KMETA
namespace Kite{
    /*struct KPowerState{
            KPowerStateTypes powerType;
            KBatteryStateTypes batteryTypes;
            U16 batteryLife;

            KPowerState(const KPowerStateTypes PowerType = KPS_UNKNOWN,
                        const KBatteryStateTypes BatteryTypes = KBS_UNKNOWN,
                        const U16 BatteryLife = 0):
                powerType(PowerType),
                batteryTypes(BatteryTypes),
                batteryLife(BatteryLife)
            {}
    };*/

    // functor base
    struct KThreadFunc
    {
		virtual ~KThreadFunc() {}
		virtual void start() = 0;
    };

    // a functor with no argument
    template <typename T>
    struct KThreadFunctor : KThreadFunc
    {
        KThreadFunctor(T functor) : m_functor(functor) {}
        virtual void start() {m_functor();}
        T m_functor;
    };

    // a functor with one argument
    template <typename F, typename A>
    struct KThreadFunctorWithArg : KThreadFunc
    {
        KThreadFunctorWithArg(F function, A arg) : m_function(function), m_arg(arg) {}
        virtual void start() {m_function(m_arg);}
        F m_function;
        A m_arg;
    };

    // member function
    template <typename C>
    struct KThreadMemberFunc : KThreadFunc
    {
        KThreadMemberFunc(void(C::*function)(), C* object) : m_function(function), m_object(object) {}
        virtual void start() {(m_object->*m_function)();}
        void(C::*m_function)();
        C* m_object;
    };

    struct KCPUInfo{
        bool x64;
        bool mmx;
        bool sse, sse2, sse3, ssse3, sse41, sse42, sse4a;
        bool avx;
        bool xop;
        bool fma3;
        bool fma4;

        KCPUInfo(bool X64 = false, bool MMX = false,
        bool SSE = false, bool SSE2 = false, bool SSE3 = false, bool SSSE3 = false,
                 bool SSE41 = false, bool SSE42 = false, bool SSE4a = false,
        bool AVX = false, bool XOP = false, bool FMA3 = false, bool FMA4 = false):
            x64(X64), mmx(MMX),
            sse(SSE), sse2(SSE2), sse3(SSE3), ssse3(SSSE3),
            sse41(SSE41), sse42(SSE42), sse4a(SSE4a),
            avx(AVX), xop(XOP), fma3(FMA3), fma4(FMA4)
        {}
    };

	KM_CLASS(POD)
	struct KHandle {
		KMETA_KHANDLE_BODY();

		KM_VAR() U16 signature;
		KM_VAR() U32 index;

		KHandle() :
			signature(0), index(0) {} // 0 reserved for invalid handles

		inline bool operator==(const KHandle& right) const {
			return (signature == right.signature) && (index == right.index);
		}

		inline bool operator!=(const KHandle& right) const  {
			return (signature != right.signature) || (index != right.index);
		}

	};

	KM_CLASS(POD)
	struct KITE_FUNC_EXPORT KStringID  {
		KMETA_KSTRINGID_BODY();

		KM_VAR() std::string str;
		KM_VAR() U32 hash;

		KStringID();

		KM_CON(std::string)
		KStringID(const std::string &String);
	};

	class KStringIDHasher {
	public:
		size_t operator() (KStringID const& key) const {
			return key.hash;
		}
	};
	class KStringIDEq {
	public:
		bool operator() (KStringID const& t1, KStringID const& t2) const {
			return (t1.hash == t2.hash);
		}
	};

	KM_CLASS(POD)
	struct KLayerInfo {
		KMETA_KLAYERINFO_BODY();

		KM_VAR() bool isBuiltin;
		KM_VAR() KStringID name;

		KM_CON(std::string, bool)
		KLayerInfo(const std::string &Name = "", bool IsBuiltin = false);
	};
}

#endif // KCORESTRUCTS_H
