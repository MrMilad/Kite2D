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
#ifndef KSOUNDBUFFER_H
#define KSOUNDBUFFER_H

/*! \file ksoundbuffer.h */

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/kcoreinstance.h"
#include "Kite/Core/system/kresource.h"
#include "Kite/Core/audio/kaudiotypes.h"

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{

	//! The KSoundBuffer class encapsulates an OpenAL buffer object with features such as reading/decoding audio formats.
	/*!
		A buffer encapsulates OpenAL state related to storing sample data.
		the application can request and release buffer objects,
		and fill them with data. Data can be supplied compressed and encoded as long as the format is supported.
		Unlike source (KSoundSource) and listener (KListener) objects,
		buffer objects can be shared among AL contexts.
		buffers are referenced by sources. a single buffer can be referred to by multiple sources.
		supported format: OGG
	*/
    class KITE_FUNC_EXPORT KSoundBuffer : public KCoreInstance, public KResource{
    public:

		//! Constructs an empty buffer object.
        KSoundBuffer();

		//! Destructor
        ~KSoundBuffer();

		//! Load the sound from a file on disk
		/*!
			Supported formats: OGG

			\param FileName Address of the sound file on the disk

			\return True if loading was successful
		*/
        bool loadFile(const std::string &FileName);

		//! Load the sound from input stream
		/*!
			Supported formats: OGG

			\param InputStream Input stream

			\return True if loading was successful
		*/
		bool loadStream(KInputStream &InputStream);

		//! Load the sound from memory
		/*!
			Supported formats: OGG

			\param Data Pointer to the file data in memory
			\param Size Size of the data (in bytes)

		\return True if loading was successful
		*/
		bool loadMemory(const void *Data, std::size_t Size);

		//! Get sample rate of the sound track
		/*!
			The sample rate is in samples per second

			\return Sample rate of the sound track
		*/
        inline I64 getSampleRate() const {return _ksampleRate;}

		//! Get number of channel of the sound track
		/*!
			The number of channels in the audio data. For example, Stereo 2 channels

			\return The number of channels in the sound track
		*/
        inline U16 getChannelCount() const {return _kchannelCount;}

		//! Get bits per sample of the sound track
		/*!
			\return Bits per sample of the sound track
		*/
        inline U16 getBitsPerSample() const {return _kbitsPerSample;}

		//! Get size of the buffer (in bytes)
		/*!
			\return Size of the buffer
		*/
        inline I32 getSize() const {return _ksize;}

		//! Get OpenAL ID of the buffer
		/*!
			\return OpenAL ID of the buffer
		*/
        inline U32 getALID() const {return _kID;}

		//! Get size of instance in memory
		/*!
			\return Size of instance in bytes
		*/
		U64 getInstanceSize() const;

    private:
        I64 _ksampleRate;
        U16 _kchannelCount;
        U16 _kbitsPerSample;
        I32 _ksize;
        U32 _kID;
    };
}

#endif // KSOUNDBUFFER_H
