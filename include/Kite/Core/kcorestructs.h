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
#include <string>
#include <vector>

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

	// Input stream
	/*struct InputStream{
		KCallInputStreamRead read;
		KCallInputStreamSeek seek;
		KCallInputStreamTell tell;
		KCallInputStreamIsOpen isOpen;
		KCallInputStreamEOF endOfFile;
		KCallInputStreamClose close;

		InputStream():
			read(0),
			seek(0),
			tell(0),
			isOpen(0),
			endOfFile(0),
			close(0)
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

	namespace Internal{

		struct BaseCompHolder {
			virtual void *create(const std::string &ComName) = 0;
			virtual void remove(U32 Index) = 0;
			virtual void *get(U32 Index) = 0;
			SIZE type; // type hash code 
		};

		template <class T>
		struct CompHolder : public BaseCompHolder {
			void *create(const std::string &ComName) override{
				// create an index
				U32 ind = 0;
				if (_kfreeIndex.empty()) {
					_kindex.push_back(_kcomp.size());
					ind = _kindex.size() - 1;
				} else {
					ind = _kfreeIndex.back();
					_kfreeIndex.pop_back();

					_kindex[ind] = _kcomp.size();
				}

				// create component
				_kcomp.push_back(T(ComName, ind));
				return &_kcomp.back();

			}

			void remove(U32 Index) override{
				KDEBUG_ASSERT(Index < _kindex.size());
				KDEBUG_ASSERT(_kindex[Index] < _kcomp.size());

				// remove component (replace it with last component in vector)
				U32 ind = _kindex[Index];
				// Beware of move assignment to self
				if (ind != _kcomp.size() - 1) {
					_kindex[_kcomp.back().getIndex()] = ind;
					_kcomp[ind] = std::move(_kcomp.back());
					
				}
				_kcomp.pop_back();

				// storing free index in free list
				_kfreeIndex.push_back(Index);
			}

			void *get(U32 Index) override{
				KDEBUG_ASSERT(Index < _kindex.size());
				KDEBUG_ASSERT(_kindex[Index] < _kcomp.size());

				return &_kcomp[_kindex[Index]];
			}

		private:
			std::vector<T> _kcomp;
			std::vector<U32> _kindex;
			std::vector<U32> _kfreeIndex;
		};
	}
}

#endif // KCORESTRUCTS_H
