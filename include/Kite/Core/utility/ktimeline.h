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
#ifndef KTIMELINE_H
#define KTIMELINE_H

#include "Kite/Core/system/ksystemdef.h"
#include <utility>
#include <set>

namespace Kite {
	// time line based X
	// duplicate on X is not valid
	template <typename Y, typename X>
	class KTimeLine {
	public:
		KTimeLine():
			_knodes(_comp)
		{}

		void addNode(Y YValue, X XValue) {
			std::pair<Y, X> pair;
			pair.first = YValue;
			pair.second = XValue;
			_knodes.insert(pair);
		}

		void removeNode(Y YValue, X XValue) {
			std::pair<Y, X> pair;
			pair.first = YValue;
			pair.second = XValue;

			std::set<std::pair<Y, X>>::iterator it = _knodes.find(pair);
			if (it != _knodes.end()) {
				_knodes.erase(it);
			}
		}
		
		// tween
		Y getValueByX(X XValue, bool Tween) const {
			std::set<std::pair<Y, X>>::iterator it = _knodes.begin();
			std::pair<Y, X> pair1;
			std::pair<Y, X> pair2;

			if (_knodes.empty()) {
				return 0;
			}

			for (it; it != _knodes.end(); ++it) {
				pair1 = *it;

				// befor first node
				if (it == _knodes.begin() && XValue < pair1.second) {
					return pair1.first;
				}

				// equal to node
				if (XValue == pair1.second) {
					return pair1.first;
				}

				// between two node (tween)
				if (XValue < pair1.second) {
					pair1 = *(--it);
					pair2 = *(++it);
					if (Tween) {
						return KTween<X, Y>::linear(XValue - pair1.second, pair1.first, pair2.first, pair2.second - pair1.second);
					} else {
						return pair1.first;
					}
				}
			}

			// after last node
			return pair1.first;
		}

		bool getSectionByX(X XValue, std::pair<Y, Y> &First, std::pair<X, X> &Second) const {
			std::set<std::pair<Y, X>>::iterator it = _knodes.begin();
			std::pair<Y, X> pair1;
			std::pair<Y, X> pair2;

			if (_knodes.empty()) {
				return false;
			}

			for (it; it != _knodes.end(); ++it) {
				pair1 = *it;

				// befor first node
				if (it == _knodes.begin() && XValue < pair1.second) {
					// y
					First.first = First.second = pair1.first;

					// x
					Second.first = Second.second = XValue;
					return true;
				}

				// equal to node
				if (XValue == pair1.second) {
					First.first = First.second = pair1.first;
					Second.first = Second.second = pair1.second;
					return true;
				}

				// between two node (tween)
				if (XValue < pair1.second) {
					pair1 = *(--it);
					pair2 = *(++it);

					// y
					First.first = pair1.first;
					First.second = pair2.first;

					// x
					Second.first = pair1.second;
					Second.second = pair2.second;
					return true;
				}
			}

			// after last node
			// y
			First.first = First.second = pair1.first;

			// x
			Second.first = Second.second = XValue;
			return true;
		}

		void clear() {
			_knodes.clear();
		}

	private:
		static bool _comp(const std::pair<Y, X> &Left, const std::pair<Y, X> &Right) {
			return Left.second < Right.second;
		}

		std::set<std::pair<Y, X>, bool(*)(const std::pair<Y, X> &, const std::pair<Y, X> &)> _knodes;;
	};
}

#endif // KTIMELINE_H