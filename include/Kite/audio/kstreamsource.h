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
#ifndef KSTREAMSOURCE_H
#define KSTREAMSOURCE_H

#include "Kite/system/ksystemdef.h"
#include "Kite/audio/ksoundsource.h"
#include "Kite/system/kthread.h"
#include <string>

namespace Kite{
namespace Internal{
class WaveIO;
}
    class KITE_FUNC_EXPORT KStreamSource : public KSoundSource{
    public:
        KStreamSource();
        ~KStreamSource();

        void play();
        void pause();
        void stop();

        inline void setLoop(bool Loop) {_kloop = Loop;}
        inline bool getLoop() {return _kloop;}

        void load(const std::string &FileName);

    private:
        void loader(); // fill buffers immediately
        void fillFirst4Buffer();
        Internal::WaveIO *_kreader;
        KThread *_kthread;
        U32 _kbuffers[4];
        bool _kloop;
        bool _KuserStop;
    };
}

#endif // KSTREAMSOURCE_H
