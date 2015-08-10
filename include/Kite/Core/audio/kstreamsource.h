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
#ifndef KSTREAMSOURCE_H
#define KSTREAMSOURCE_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/utility/kresource.h"
#include "Kite/Core/utility/kmeminputstream.h"
#include "Kite/Core/audio/ksoundsource.h"
#include <string>

namespace std{
	class thread;
}
namespace Kite{
	namespace Internal{
		class SoundIO;
	}
    class KITE_FUNC_EXPORT KStreamSource : public KSoundSource, public KResource{
    public:
        KStreamSource();
        ~KStreamSource();

        void play();
        void pause();
        void stop();

		void setLoop(bool Loop);
		bool getLoop();

        /// load sound file
		bool loadFile(const std::string &FileName, U32 FileType = 0);

		/// load from stream
		bool loadStream(KInputStream &InputStream, U32 FileType = 0);

		/// load from memory
		bool loadMemory(const void *Data, std::size_t Size, U32 FileType = 0);

    private:
        void loader(); // fill buffers immediately (thread task)
        void fillFirst4Buffer();
        Internal::SoundIO *_kreader;
        std::thread *_kthread;
        U32 _kbuffers[4];
        I8 *_kdata;
        bool _kloop;
        bool _KuserStop;
		KMemInputStream *_kmemstream;
    };
}

#endif // KSTREAMSOURCE_H
