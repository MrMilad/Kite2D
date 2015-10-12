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
#ifndef KFTRAITS_H
#define KFTRAITS_H

#include <tuple>

namespace Kite {
	template<class F>
	struct kf_traits;

	// function pointer
	template<class R, class... Args>
	struct kf_traits<R(*)(Args...)> : public kf_traits<R(Args...)> {
	};

	template<class R, class... Args>
	struct kf_traits<R(Args...)> {
		using return_type = R;

		static constexpr std::size_t arity = sizeof...(Args);

		template <std::size_t N>
		struct argument {
			static_assert(N < arity, "error: invalid parameter index.");
			using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
		};
	};

	// member function pointer
	template<class C, class R, class... Args>
	struct kf_traits<R(C::*)(Args...)> : public kf_traits<R(C&, Args...)> {
	};

	// const member function pointer
	template<class C, class R, class... Args>
	struct kf_traits<R(C::*)(Args...) const> : public kf_traits<R(C&, Args...)> {
	};

	// member object pointer
	template<class C, class R>
	struct kf_traits<R(C::*)> : public kf_traits<R(C&)> {
	};
}
#endif // KFTRAITS_H