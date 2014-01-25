#ifndef KTHREAD_H
#define KTHREAD_H

#include "include/Kite/system/ksystemdef.h"
#include "include/Kite/system/knoncopyable.h"
#include "include/Kite/system/ksystemstructs.h"

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
