#include "Kite/system/ksystemutil.h"
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

namespace Kite{
    void sleep(U64 MicroSec){
        // usleep is not reliable enough (it might block the
       // whole process instead of just the current thread)
       // so we must use pthread_cond_timedwait instead

       // this implementation is inspired from Qt
       Kite::U64 usecs = MicroSec;

       // get the current time
       timeval tv;
       gettimeofday(&tv, NULL);

       // construct the time limit (current time + time to wait)
       timespec ti;
       ti.tv_nsec = (tv.tv_usec + (usecs % 1000000)) * 1000;
       ti.tv_sec = tv.tv_sec + (usecs / 1000000) + (ti.tv_nsec / 1000000000);
       ti.tv_nsec %= 1000000000;

       // create a mutex and thread condition
       pthread_mutex_t mutex;
       pthread_mutex_init(&mutex, 0);
       pthread_cond_t condition;
       pthread_cond_init(&condition, 0);

       // wait...
       pthread_mutex_lock(&mutex);
       pthread_cond_timedwait(&condition, &mutex, &ti);
       pthread_mutex_unlock(&mutex);

       // destroy the mutex and condition
       pthread_cond_destroy(&condition);
       pthread_mutex_destroy(&mutex);
    }

    KPowerState getPowerState(){
        // ....
    }

}
