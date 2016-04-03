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
#include "Kite/serialization/kbaseserial.h"
#include "Kite/serialization/types/kstdvector.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/serialization/kserialization.h"
#include <string>
#include <vector>
#include <deque>
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

	namespace Internal{
		// catch friendly storage!
		template<typename T>
		struct CFStorage {
			KHandle add(const T &Object){
				// create component
				_kcontiner.push_back(Object);

				// create extrenal handle
				KHandle ret;

				// no free handle
				if (_kfreeIndex.empty()) {
					// create internal handle
					KHandle hndl;
					hndl.index = _kcontiner.size() - 1;
					++hndl.signature; // 0 reserved for invalid handles
					_khandle.push_back(hndl);

					ret.index = _khandle.size() - 1;
					++ret.signature;  // 0 reserved for invalid handles

				// there are some free handle
				} else {
					KHandle hndl;
					hndl = _khandle[_kfreeIndex.front()];
					hndl.index = _kcontiner.size() - 1;
					++hndl.signature;

					ret.index = _kfreeIndex.front();
					ret.signature = hndl.signature;
					_kfreeIndex.pop_front();
				}

				return ret;
			}

			void remove(const KHandle &Handle) {
				if (Handle.index >= _khandle.size()) {
					KD_FPRINT("handle index is out of range. ind: %i", Handle.index);
					return;
				}

				if (_khandle[Handle.index].index >= _kcontiner.size()) {
					KD_FPRINT("handle index is out of range. ind: %i", Handle.index);
					return;
				}

				if (Handle.signature != _khandle[Handle.index].signature) {
					KD_FPRINT("handle is not valid. ind: %i", Handle.index);
					return;
				}

				// remove component (replace it with last component in vector)
				U32 ind = _khandle[Handle.index].index;

				// Beware of move assignment to self
				if (ind != _kcontiner.size() - 1) {
					_khandle[_kcontiner.back().getHandle().index].index = ind;
					_kcontiner[ind] = std::move(_kcontiner.back());
				}
				_kcontiner.pop_back();

				// storing free index in free list
				_kfreeIndex.push_back(Handle.index);

				// free handle 
				_khandle[Handle.index] = KHandle();
			}

			T *get(const KHandle &Handle) {
				if (Handle.index >= _khandle.size()) {
					KD_FPRINT("handle index is out of range. ind: %i", Handle.index);
					return nullptr;
				}

				if (_khandle[Handle.index].index >= _kcontiner.size()) {
					KD_FPRINT("handle index is out of range. ind: %i", Handle.index);
					return nullptr;
				}

				if (_khandle[Handle.index].signature != Handle.signature) {
					KD_FPRINT("handle is not valid. ind: %i", Handle.index);
					return nullptr;
				}

				return &_kcontiner[_khandle[Handle.index].index];
			}

			auto begin() { return _kcontiner.begin(); }

			auto end() { return _kcontiner.end(); }

			SIZE getSize() const { return _kcontiner.size(); }

			void clear() {
				_kcontiner.clear();
				_khandle.clear();
				_kfreeIndex.clear();
			}

		private:
			std::vector<T> _kcontiner;
			std::vector<KHandle> _khandle;
			std::deque<U32> _kfreeIndex;
		};

		template<class T>
		struct BaseCHolder {
			/*friend KBaseSerial &operator<<(KBaseSerial &Out, BaseCompHolder &Value) {
			Value.serial(Out, KSerialStateTypes::KST_SERIALIZE); return Out;
			}

			friend KBaseSerial &operator>>(KBaseSerial &In, BaseCompHolder &Value) {
			Value.serial(In, KSerialStateTypes::KST_DESERIALIZE); return In;
			}*/

			//virtual void serial(KBaseSerial &Serializer, KSerialStateTypes State) = 0;
			virtual KHandle add(const std::string &Name) = 0;
			virtual void remove(const KHandle &Handle) = 0;
			virtual T *get(const KHandle &Handle) = 0;
			virtual void clear() = 0;
			virtual SIZE getSize() = 0;
			SIZE type; // type hash code 
		};

		template <class T, class Y>
		struct CHolder : public BaseCHolder<Y> {
			/*void serial(KBaseSerial &Serializer, KSerialStateTypes State) override {
				if (State == KSerialStateTypes::KST_SERIALIZE) {
					Serializer << type;

					Serializer << _kcomp.size();
					Serializer << _kcomp;

					Serializer << _kindex.size();
					Serializer << _kindex;

					Serializer << _kfreeIndex.size();
					Serializer << _kfreeIndex;

				} else if (State == KSerialStateTypes::KST_DESERIALIZE) {
					SIZE tp;
					SIZE size;
					Serializer >> tp;
					if (tp == type) {
						Serializer >> size;
						_kcomp.resize(size, T("", 0));
						Serializer >> _kcomp;

						Serializer >> size;
						_kindex.resize(size);
						Serializer >> _kindex;

						Serializer >> size;
						_kfreeIndex.resize(size);
						Serializer >> _kfreeIndex;
					} else {
						KDEBUG_PRINT("type missmatch. serialization uncomplete");
					}
				}
			}*/

			KHandle add(const std::string &Name) override {
				return _kstorage.add(T(Name));
			}

			void remove(const KHandle &Handle) override {
				_kstorage.remove(Handle);
			}

			Y *get(const KHandle &Handle) override {
				return (Y *)_kstorage.get(Handle);
			}

			void clear() override {
				_kstorage.clear();
			}

			SIZE getSize() override {
				return _kstorage.getSize();
			}

			CFStorage<T> *getStorage(){
				return &_kstorage;
			}

		private:
			CFStorage<T> _kstorage;
		};
	}

}

#endif // KCORESTRUCTS_H
