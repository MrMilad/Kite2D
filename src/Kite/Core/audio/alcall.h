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
#ifndef ALCALL_H
#define ALCALL_H

#include "Kite/core/system/ksystemdef.h"
#include "extlibs/headers/al.h"
#include "extlibs/headers/alc.h"

namespace Kite{
namespace Internal{

    class ALDevice{
    public:
        // pass 0 or NULL for select default device
        ALDevice(const char *DeviceName = NULL);
        ~ALDevice();

    private:
        ALCdevice*  _kdevice;
        ALCcontext* _kcontext;
    };
    extern ALDevice _kaldevice;

    extern bool checkLastALErr();
    extern ALenum getFormat(U16 ChannelCount, U16 BitsPerSample);
}
}

// OpenGL debug macro
#if defined(KITE_DEV_DEBUG)
    #define DAL_CALL(call) call; KDEBUG_ASSERT_T(Kite::Internal::checkLastALErr())
#else
    #define DAL_CALL(call) call;
#endif


#endif // ALCALL_H
