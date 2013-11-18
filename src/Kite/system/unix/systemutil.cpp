/*    
    Kite2D Game Framework.
    Copyright (C) 2010-2013  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
