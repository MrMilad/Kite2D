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
#ifndef KTHREAD_H
#define KTHREAD_H

#include "Kite/core/system/ksystemdef.h"
#include "Kite/core/system/knoncopyable.h"
#include "Kite/core/system/ksystemstructs.h"

namespace Kite{
namespace Internal{
    class Thread;
}
    class KITE_FUNC_EXPORT KThread : KNonCopyable{
        friend class Internal::Thread;
    public:

        template<typename F>
        KThread(F Function):
            _kimpl(0),
            _kthreadFunc(new KThreadFunctor<F>(Function))
        {}

        template<typename F, typename A>
        KThread(F Function, A Argument):
            _kimpl(0),
            _kthreadFunc(new KThreadFunctorWithArg<F, A>(Function, Argument))
        {}

        template<typename C>
        KThread(void(C::*Function)(), C* Object):
            _kimpl(0),
            _kthreadFunc(new KThreadMemberFunc<C>(Function, Object))
        {}

        ~KThread();

        void lunch();
        void wait();
        void terminate();

        bool isRunning();

    private:
        void runFunc();

        Internal::Thread *_kimpl; // platform-specific implementation
        KThreadFunc *_kthreadFunc;
    };
}

#endif // KTHREAD_H
