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
#ifndef KCFSTORAGE_H
#define KCFSTORAGE_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcorestructs.h"
#include "Kite/serialization/kserialization.h"
#include "Kite/serialization/types/kstdvector.h"
#include "Kite/serialization/types/kstddeque.h"
#include <vector>
#include <deque>

namespace Kite {
	template <typename T>
	class KCFStorage {
		friend KBaseSerial &operator<<(KBaseSerial &Out, const KCFStorage<T> &Value) {
			Value.serial(Out); return Out;
		}

		friend KBaseSerial &operator>>(KBaseSerial &In, KCFStorage<T> &Value) {
			Value.deserial(In); return In;
		}

	public:
		KCFStorage() :
			_kmodified(false) 
		{
			_kcontiner.reserve(KCFSTORAGE_CHUNK_SIZE);
			_kexhandle.reserve(KCFSTORAGE_CHUNK_SIZE);
			_khandle.reserve(KCFSTORAGE_CHUNK_SIZE);
		}

		KHandle add(const T &Object) {
			// create component
			_kcontiner.push_back(Object);

			// create external handle
			_kexhandle.push_back(KHandle());

			// no free handle
			if (_kfreeIndex.empty()) {
				// create internal handle
				KHandle hndl;
				hndl.index = _kcontiner.size() - 1;
				++hndl.signature; // 0 reserved for invalid handles
				_khandle.push_back(hndl);

				// set external handle
				_kexhandle.back().index = _khandle.size() - 1;
				++_kexhandle.back().signature;  // 0 reserved for invalid handles

			// there are some free handle				  
			} else {
				_khandle[_kfreeIndex.front()].index = _kcontiner.size() - 1;
				++_khandle[_kfreeIndex.front()].signature;

				// set external handle
				_kexhandle.back().index = _kfreeIndex.front();
				_kexhandle.back().signature = _khandle[_kfreeIndex.front()].signature;

				// consume free handle
				_kfreeIndex.pop_front();
			}

			// set storage as modified
			_kmodified = true;

			// return external handle
			return _kexhandle.back();
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
			// retrieve internal handle
			U32 ind = _khandle[Handle.index].index;

			// Beware of move assignment to self
			if (ind != _kcontiner.size() - 1) {

				// replace removed object with last object
				_khandle[_kexhandle.back().index].index = ind;
				_kcontiner[ind] = std::move(_kcontiner.back());
				_kexhandle[ind] = std::move(_kexhandle.back());
			}
			_kcontiner.pop_back();
			_kexhandle.pop_back();

			// storing free index in free list
			_kfreeIndex.push_back(Handle.index);

			// free handle 
			_khandle[Handle.index] = KHandle();

			// set storage as modified
			_kmodified = true;
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

		inline std::vector<T> *getContiner() { return &_kcontiner; }

		SIZE getSize() const { return _kcontiner.size(); }

		void clear() {
			_kcontiner.clear();
			_khandle.clear();
			_kfreeIndex.clear();
			_kmodified = true;
		}

		inline bool getModified() const { return _kmodified; }
	private:

		void serial(KBaseSerial &Out) const {
			Out << _kcontiner;
			Out << _khandle;
			Out << _kexhandle;
			Out << _kfreeIndex;
		}

		void deserial(KBaseSerial &In) {
			In >> _kcontiner;
			In >> _khandle;
			In >> _kexhandle;
			In >> _kfreeIndex;
		}

		std::vector<T> _kcontiner;
		std::vector<KHandle> _khandle;
		std::vector<KHandle> _kexhandle;
		std::deque<U32> _kfreeIndex;
		bool _kmodified;
	};

	namespace Internal {
		template<class T>
		struct BaseCHolder {
			friend KBaseSerial &operator<<(KBaseSerial &Out, const BaseCHolder &Value) {
				Value.serial(Out); return Out;
			}

			friend KBaseSerial &operator>>(KBaseSerial &In, BaseCHolder &Value) {
				Value.deserial(In); return In;
			}

			virtual ~BaseCHolder() {}

			//virtual void serial(KBaseSerial &Serializer, KSerialStateTypes State) = 0;
			virtual KHandle add(const std::string &Name = "") = 0;
			virtual void remove(const KHandle &Handle) = 0;
			virtual T *get(const KHandle &Handle) = 0;
			virtual void clear() = 0;
			virtual SIZE getSize() = 0;
			virtual bool getModified() = 0;
			virtual void serial(KBaseSerial &Out) const = 0;
			virtual void deserial(KBaseSerial &In) = 0;
			SIZE type; // type hash code 
		};

		template <class T, class Y>
		struct CHolder : public BaseCHolder<Y> {
			virtual ~CHolder() {
				_kstorage.clear();
			}
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

			bool getModified() override {
				return _kstorage.getModified();
			}

			KCFStorage<T> *getStorage() {
				return &_kstorage;
			}

			void serial(KBaseSerial &Out) const override {
				Out << _kstorage;
			}

			void deserial(KBaseSerial &In) override {
				In >> _kstorage;
			}

		private:
			KCFStorage<T> _kstorage;
		};
	}
}

#endif // KCFSTORAGE_H