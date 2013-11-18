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
#ifndef THREAD_H
#define THREAD_H

#include "windows.h"
#include "include/Kite/system/ksystemdef.h"
#include "include/Kite/system/knoncopyable.h"
#include "include/Kite/system/kthread.h"

namespace Kite{
namespace Internal{
    class Thread : KNonCopyable{
    public:
        Thread(KThread *ThreadOwner);
        ~Thread();

        void wait();
        void terminate();
        bool isRunning();

    private:
        static unsigned int __stdcall threadEntryPoint(void* anyData);
        HWND _kthreadHandle;
        U32 _kthreadID;
    };
}
}

#endif // KTHREADIMPL_H
