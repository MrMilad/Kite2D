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
	struct KITE_FUNC_EXPORT KStringID  {
		KMETA_KSTRINGID_BODY();

		KM_VAR() std::string str;
		KM_VAR() U32 hash;

		KStringID();

		KM_CON(std::string)
		KStringID(const std::string &String);
	};

	/// dynamic version of std::bitset with lua binding
	/// this class can only read/write from/to an already exist std::bitset 
	/// and can't create a new std::bitset
	KM_CLASS(POD)
	struct KITE_FUNC_EXPORT KBitset {
		KMETA_KBITSET_BODY();

		KM_CON(U32, std::string)
		KBitset(U32 Size, const std::string &Value);

		KM_FUN()
		bool test(U32 Pos) const;

		KM_FUN()
		inline bool all() const { return _kall; }

		KM_FUN()
		inline bool any() const { return _kany; }

		KM_FUN()
		inline bool none() const { return _knone; }

		KM_FUN()
		inline U32 count() const { return _kcount; }

		KM_FUN()
		inline U32 size() const { return _kvalue.size(); }

		KM_FUN()
		void andWith(const KBitset &Other);

		KM_FUN()
		void orWith(const KBitset &Other);

		KM_FUN()
		void xorWith(const KBitset &Other);

		KM_FUN()
		void notWith(const KBitset &Other);

		KM_FUN()
		void setAll(bool Value);

		KM_FUN()
		void set(U32 Pos, bool Value);

		KM_FUN()
		void flipAll();

		KM_FUN()
		void flip(U32 Pos);

		KM_FUN()
		inline const std::string &toString() const { return _kvalue; }

		inline bool operator==(const KBitset &right) const {
			return (_kvalue == right._kvalue);
		}

	private:
		void proc();

		KM_OPE(KO_EQ)
		bool luaEqOper(const KBitset &right) const {
			return operator==(right);
		}

		KM_VAR(UNBIND) bool _kall;
		KM_VAR(UNBIND) bool _kany;
		KM_VAR(UNBIND) bool _knone;
		KM_VAR(UNBIND) U32 _kcount;
		KM_VAR(UNBIND) std::string _kvalue;
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
