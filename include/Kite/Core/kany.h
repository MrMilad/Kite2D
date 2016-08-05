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
#ifndef KANY_H
#define KANY_H

#pragma warning( disable : 4521 )

#include "Kite/core/kcoredef.h"
#include <type_traits>
#include <utility>
#include <typeinfo>
#include <string>
#include <cassert>

namespace Kite {
	template<class U>
	using StorageType = typename std::decay<typename std::remove_reference<U>::type>::type;

	class KAny {
	public:
		bool is_null() const { if (!ptr) { return true; } return false; }
		bool not_null() const { if (ptr) { return true; } return false; }

		template<typename U> KAny(U&& value, bool CastFree = false):
			ptr(new Derived<StorageType<U>>(std::forward<U>(value))),
			_kcfree(CastFree)
			{}

		template<class U> bool is() const {
			typedef StorageType<U> T;

			if (_kcfree) return true;

			auto derived = dynamic_cast<Derived<T>*> (ptr);

			if (derived) return true;

			return false;
		}

		template<class U>
		StorageType<U>& as() {
			typedef StorageType<U> T;

			if (_kcfree) {
				auto derived = static_cast<Derived<T>*> (ptr);
				return derived->value;
			}

			auto derived = dynamic_cast<Derived<T>*> (ptr);

			if (!derived)
				throw std::bad_cast();

			return derived->value;
		}

		template<class U>
		StorageType<U>& asFreeCast() {
			typedef StorageType<U> T;

			auto derived = static_cast<Derived<T>*> (ptr);
			return derived->value;

		}

		template<class U>
		operator U() {
			return as<StorageType<U>>();
		}

		KAny()
			: ptr(nullptr), _kcfree(false) {

		}

		KAny(KAny& that)
			: ptr(that.clone()) {

		}

		KAny(KAny&& that)
			: ptr(that.ptr) {
			that.ptr = nullptr;
		}

		KAny(const KAny& that)
			: ptr(that.clone()) {

		}

		KAny(const KAny&& that)
			: ptr(that.clone()) {

		}

		KAny& operator=(const KAny& a) {
			if (ptr == a.ptr)
				return *this;

			auto old_ptr = ptr;

			ptr = a.clone();

			if (old_ptr)
				delete old_ptr;

			return *this;
		}

		KAny& operator=(KAny&& a) {
			if (ptr == a.ptr)
				return *this;

			std::swap(ptr, a.ptr);

			return *this;
		}

		~KAny() {
			if (ptr)
				delete ptr;
		}

	private:
		struct Base {
			virtual ~Base() {}

			virtual Base* clone() const = 0;
		};

		template<typename T>
		struct Derived : Base {
			template<typename U> Derived(U&& value) : value(std::forward<U>(value)) {}

			T value;

			Base* clone() const { return new Derived<T>(value); }
		};

		Base* clone() const {
			if (ptr)
				return ptr->clone();
			else
				return nullptr;
		}

		Base* ptr;
		bool _kcfree;
	};
}

#endif // KANY_H