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

#ifndef KDELEGATE_H
#define KDELEGATE_H

#include "Kite/core/kcoredef.h"

namespace Kite {
#if defined (KITE_CPP_VER11)
	//! we have C++11 support...yeah!
	/*!
	* non specialized template declaration for delegate
	*/
	template <typename T>
	class KDelegate;

	//! specialization for member functions
	/*!
	* \tparam T            class-type of the object who's member function to call
	* \tparam R            return type of the function that gets captured
	* \tparam params       variadic template list for possible arguments
	*                      of the captured function
	*/
	template <typename T, typename R, typename... Params>
	class KITE_FUNC_EXPORT KDelegate<R(T::*)(Params...)>{
	public:
		typedef R(T::*func_type)(Params...);

		KDelegate(func_type Func, T& Callee)
			: _kcallee(Callee)
			, _kfunc(Func) {}

		R operator()(Params... args) const {
			return (_kcallee.*_kfunc)(args...);
		}

		bool operator==(const KDelegate& other) const {
			return (&_kcallee == &other._kcallee) && (_kfunc == other._kfunc);
		}
		bool operator!= (const KDelegate& other) const {
			return !((*this) == other);
		}

	private:
		T& _kcallee;
		func_type _kfunc;
	};

	//!specialization for const member functions
	template <typename T, typename R, typename... Params>
	class KITE_FUNC_EXPORT KDelegate<R(T::*)(Params...) const>
	{
	public:
		typedef R(T::*func_type)(Params...) const;

		KDelegate(func_type Func, const T& Callee)
			: _kcalle(Callee)
			, _kfunc(Func) {}

		R operator()(Params... args) const {
			return (_kcalle.*_kfunc)(args...);
		}

		bool operator==(const KDelegate& other) const {
			return (&_kcalle == &other._kcalle) && (_kfunc == other._kfunc);
		}
		bool operator!= (const KDelegate& other) const {
			return !(*this == other);
		}

	private:
		const T& _kcalle;
		func_type _kfunc;
	};

	//! specialization for free functions
	/*!
	* \tparam R            return type of the function that gets captured
	* \tparam params       variadic template list for possible arguments
	*                      of the captured function
	*/
	template <typename R, typename... Params>
	class KITE_FUNC_EXPORT KDelegate<R(*)(Params...)>
	{
	public:
		typedef R(*func_type)(Params...);

		KDelegate(func_type Func)
			: _kfunc(Func) {}

		R operator()(Params... args) const {
			return (*_kfunc)(args...);
		}

		bool operator==(const KDelegate& other) const {
			return _kfunc == other._kfunc;
		}
		bool operator!= (const KDelegate& other) const {
			return !((*this) == other);
		}

	private:
		func_type _kfunc;
	};

	//! function to deduce template parameters from call-context
	template <typename F, typename T>
	KDelegate<F> make_delegate(F Func, T& obj) {
		return KDelegate<F>(Func, obj);
	}

	template <typename T>
	KDelegate<T> make_delegate(T Func) {
		return KDelegate<T>(Func);
	}

	// a little backward compatilbility layer
#define DELEGATE make_delegate
#define DELEGATE_CONST make_delegate
#define DELEGATE_FREE make_delegate

#else // no C++11 support (no variadic template)
	template <typename T>
	class KDelegate;

	template <typename R>
	class KITE_FUNC_EXPORT KDelegate<R(*)()>
	{
	public:
		typedef R(*func_type)();

		KDelegate(func_type Func)
			: _kfunc(Func) {}

		R operator()() const {
			return (*_kfunc)();
		}

		bool operator==(const KDelegate& other) const {
			return (_kfunc == other._kfunc);
		}
		bool operator!= (const KDelegate& other) const {
			return !(*this == other);
		}

	private:
		func_type _kfunc;
	};
	template <typename R, typename P>
	class KITE_FUNC_EXPORT KDelegate<R(*)(P)>
	{
	public:
		typedef R(*func_type)(P);

		KDelegate(func_type Func)
			: _kfunc(Func) {}

		R operator()(P x) const {
			return (*_kfunc)(x);
		}

		bool operator==(const KDelegate& other) const {
			return (_kfunc == other._kfunc);
		}
		bool operator!= (const KDelegate& other) const {
			return !((*this) == other);
		}

	private:
		func_type _kfunc;
	};
	//!specialization for member functions with 0 parmeter
	/*!
	* \tparam T            class-type of the object who's member function to call
	* \tparam R            return type of the function that gets captured
	*/
	template <typename T, typename R>
	class KITE_FUNC_EXPORT KDelegate<R(T::*)()>
	{
	public:
		typedef R(T::*func_type)();

		KDelegate(func_type Func, T& Callee)
			: _kcalle(Callee)
			, _kfunc(Func) {}

		R operator()() const {
			return (_kcalle.*_kfunc)();
		}

		bool operator==(const KDelegate& other) const {
			return (&_kcalle == &other._kcalle) && (_kfunc == other._kfunc);
		}
		bool operator!= (const KDelegate& other) const {
			return !((*this) == other);
		}

	private:
		T& _kcalle;
		func_type _kfunc;
	};
	template <typename T, typename R>
	class KITE_FUNC_EXPORT KDelegate<R(T::*)() const>
	{
	public:
		typedef R(T::*func_type)() const;

		KDelegate(func_type Func, T& Callee)
			: _kcalle(Callee)
			, _kfunc(Func) {}

		R operator()() const {
			return (_kcalle.*_kfunc)();
		}

		bool operator==(const KDelegate& other) const {
			return (&_kcalle == &other._kcalle) && (_kfunc == other._kfunc);
		}
		bool operator!= (const KDelegate& other) const {
			return !((*this) == other);
		}

	private:
		T& _kcalle;
		func_type _kfunc;
	};
	//! specialization for member functions with 1 parmeter
	/*!
	* \tparam T            class-type of the object who's member function to call
	* \tparam R            return type of the function that gets captured
	* \tparam P            type of first parameter of the captured function
	*/
	template <typename T, typename R, typename P>
	class KITE_FUNC_EXPORT KDelegate<R(T::*)(P)>
	{
	public:
		typedef R(T::*func_type)(P);

		KDelegate(func_type Func, T& Callee)
			: _kcalle(Callee)
			, _kfunc(Func) {}

		R operator()(P arg) const {
			return (_kcalle.*_kfunc)(arg);
		}

		bool operator==(const KDelegate& other) const {
			return (&_kcalle == &other._kcalle) && (_kfunc == other._kfunc);
		}
		bool operator!= (const KDelegate& other) const {
			return !((*this) == other);
		}

	private:
		T& _kcalle;
		func_type _kfunc;
	};
	template <typename T, typename R, typename P>
	class KITE_FUNC_EXPORT KDelegate<R(T::*)(P) const>
	{
	public:
		typedef R(T::*func_type)(P) const;

		KDelegate(func_type Func, T& Callee)
			: _kcalle(Callee)
			, _kfunc(Func) {}

		R operator()(P arg) const {
			return (_kcalle.*_kfunc)(arg);
		}

		bool operator==(const KDelegate& other) const {
			return (&_kcalle == &other._kcalle) && (_kfunc == other._kfunc);
		}
		bool operator!= (const KDelegate& other) const {
			return !((*this) == other);
		}

	private:
		T& _kcalle;
		func_type _kfunc;
	};
	template <typename T, typename R, typename P1, typename P2>
	class KITE_FUNC_EXPORT KDelegate<R(T::*)(P1, P2)>
	{
	public:
		typedef R(T::*func_type)(P1, P2);

		KDelegate(func_type Func, T& Callee)
			: _kcalle(Callee)
			, _kfunc(Func) {}

		R operator()(P1 arg, P2 arg2) const {
			return (_kcalle.*_kfunc)(arg, arg2);
		}

		bool operator==(const KDelegate& other) const {
			return (&_kcalle == &other._kcalle) && (_kfunc == other._kfunc);
		}
		bool operator!= (const KDelegate& other) const {
			return !((*this) == other);
		}

	private:
		T& _kcalle;
		func_type _kfunc;
	};
	template <typename T, typename R, typename P1, typename P2, typename P3>
	class KITE_FUNC_EXPORT KDelegate<R(T::*)(P1, P2, P3)>
	{
	public:
		typedef R(T::*func_type)(P1, P2, P3);

		KDelegate(func_type Func, T& Callee)
			: _kcalle(Callee)
			, _kfunc(Func) {}

		R operator()(P1 arg, P2 arg2, P3 arg3) const {
			return (_kcalle.*_kfunc)(arg, arg2, arg3);
		}

		bool operator==(const KDelegate& other) const {
			return (&_kcalle == &other._kcalle) && (_kfunc == other._kfunc);
		}
		bool operator!= (const KDelegate& other) const {
			return !((*this) == other);
		}

	private:
		T& _kcalle;
		func_type _kfunc;
	};
	template <typename T, typename R, typename P1, typename P2, typename P3, typename P4>
	class KITE_FUNC_EXPORT KDelegate<R(T::*)(P1, P2, P3, P4)>
	{
	public:
		typedef R(T::*func_type)(P1, P2, P3, P4);

		KDelegate(func_type Func, T& Callee)
			: _kcalle(Callee)
			, _kfunc(Func) {}

		R operator()(P1 arg, P2 arg2, P3 arg3, P4 arg4) const {
			return (_kcalle.*_kfunc)(arg, arg2, arg3, arg4);
		}

		bool operator==(const KDelegate& other) const {
			return (&_kcalle == &other._kcalle) && (_kfunc == other._kfunc);
		}
		bool operator!= (const KDelegate& other) const {
			return !((*this) == other);
		}

	private:
		T& _kcalle;
		func_type _kfunc;
	};
	//! function to deduce template parameters from call-context
	template <typename F, typename T>
	KDelegate<F> make_delegate(F Func, T& obj) {
		return KDelegate<F>(Func, obj);
	}

	template <typename T>
	KDelegate<T> make_delegate(T Func) {
		return KDelegate<T>(Func);
	}

#endif

} // namespace KDelegate

#endif // KDELEGATE_H