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
#include <memory>
#include <bitset>

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
		KM_INFO(KI_NAME = "StringID");
		KSTRINGID_BODY();

		KM_VAR() std::string str;
		KM_VAR() U32 hash;

		KStringID();

		KM_CON(std::string)
		KStringID(const std::string &String);

		KM_FUN()
		void clear();
	};

	/// dynamic version of std::bitset with lua binding
	template <U8 S>
	KM_CLASS(POD)
	struct KITE_FUNC_EXPORT KBitset {
		KM_TEM_PARAM(S);
		KM_TEM_DEF("Bitset8", 8);
		KM_TEM_DEF("Bitset16", 16);
		KM_TEM_DEF("Bitset32", 32);
		KBITSET_BODY();

		KBitset() {}

		KM_CON(U32)
		KBitset(U32 Value) :
			_kbitset(Value) {}

		KM_FUN()
		bool test(U32 Pos) const {
			return _kbitset.test(Pos);
		}

		KM_PRO_GET(KP_NAME = "all", KP_TYPE = bool)
		inline bool all() const { return _kbitset.all(); }

		KM_PRO_GET(KP_NAME = "any", KP_TYPE = bool)
		inline bool any() const { return _kbitset.any(); }

		KM_PRO_GET(KP_NAME = "none", KP_TYPE = bool)
		inline bool none() const { return _kbitset.none(); }

		KM_PRO_GET(KP_NAME = "count", KP_TYPE = bool)
		inline U32 count() const { return _kbitset.count(); }

		KM_PRO_GET(KP_NAME = "size", KP_TYPE = bool)
		inline U32 size() const { return _kbitset.size(); }

		KM_FUN()
		inline auto andWith(const KBitset<S> &Other) { return _kbitset.operator&=( Other._kbitset); }

		KM_FUN()
		inline auto orWith(const KBitset<S> &Other) { return _kbitset.operator|=(Other._kbitset); }

		KM_FUN()
		inline auto xorWith(const KBitset<S> &Other) { return _kbitset.operator^=(Other._kbitset); }

		KM_FUN()
		inline auto not() const { return _kbitset.operator~(); }

		KM_FUN()
		void setAllTrue() { _kbitset.set(); }

		KM_FUN()
		inline void set(SIZE Pos, bool Value) { _kbitset.set(Pos, Value); }

		KM_FUN()
		inline void flipAll() { _kbitset.flip(); }

		KM_FUN()
		inline void flip(SIZE Pos) { _kbitset.flip(Pos); }

		KM_FUN()
		inline const std::string &toString() const { return _kvalue; }

		inline bool operator==(const KBitset &right) const {
			return (_kbitset == right._kbitset);
		}

	private:
		KM_OPE(KO_EQ)
		bool luaEqOper(const KBitset &right) const {
			return operator==(right);
		}
		
		std::bitset<S> _kbitset;
	};

	typedef KBitset<8> KBitset8;
	typedef KBitset<16> KBitset16;
	typedef KBitset<32> KBitset32;

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
}

#endif // KCORESTRUCTS_H
