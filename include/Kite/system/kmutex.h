/* 
 Kite2D Game Framework.
 Copyright (C) 2010-2014  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or 
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef KMUTEX_H
#define KMUTEX_H

#include "include/Kite/system/ksystemdef.h"
#include "include/Kite/system/knoncopyable.h"

namespace Kite{
namespace Internal{
    class Mutex;
}
    class KITE_FUNC_EXPORT KMutex : KNonCopyable{
    public:
        KMutex();
        ~KMutex();

        void lock();
        void unlock();

    private:
        Internal::Mutex *_kimpl;
    };
}

#endif // KMUTEX_H
