#include "src/Kite/audio/soundio.h"

namespace Kite{
namespace Internal{

    SoundIO::SoundIO():
        _kfile(0),
        _kchannelCount(0),
        _ksampleRate(0)
    {}

    SoundIO::~SoundIO(){
        if (_kfile != 0)
            sf_close(_kfile);
    }

    void SoundIO::openReadFromFile(const std::string& FileName){
        // reset file handle (if already opened)
        if (_kfile != 0){
            sf_close(_kfile);
            _kfile = 0;
        }

        // open sound file
        SF_INFO fInfo;
        _kfile = sf_open(FileName.c_str(), SFM_READ, &fInfo);
        KDEBUG_ASSERT(_kfile);

        // set the parameters
        _kchannelCount = fInfo.channels;
        _ksampleRate   = fInfo.samplerate;
        _ksampleCount  = (std::size_t)(fInfo.frames * _kchannelCount);
    }

    void SoundIO::openWriteToFile(const std::string& FileName, U32 ChannelCount, U32 SampleRate){
        // reset file handle (if already opened)
        if (_kfile != 0){
            sf_close(_kfile);
            _kfile = 0;
        }

        // find the file format (from file extension)
        I32 frmt = getFileFormat(FileName);
        KDEBUG_ASSERT(frmt >= 0);

        // fill the sound infos with parameters
        SF_INFO fInfo;
        fInfo.channels   = ChannelCount;
        fInfo.samplerate = SampleRate;
        fInfo.format     = frmt | (frmt == SF_FORMAT_OGG ? SF_FORMAT_VORBIS : SF_FORMAT_PCM_16);

        // open the file for writing
        _kfile = sf_open(FileName.c_str(), SFM_WRITE, &fInfo);
        KDEBUG_ASSERT(_kfile);

        // set our parameters
        _kchannelCount = ChannelCount;
        _ksampleRate   = SampleRate;
        _ksampleCount  = 0;
    }

    std::size_t SoundIO::read(I16* Data, std::size_t SampleCount){
        if (_kfile && Data && SampleCount)
            return (std::size_t)(sf_read_short(_kfile, Data, SampleCount));
        return 0;
    }

    void SoundIO::write(const I16* Data, std::size_t SampleCount){
        if (_kfile && Data && SampleCount){
            // Write small chunks (instead of everything at once)
            // to avoid a stack overflow in libsndfile (happens only with OGG format)
            while (SampleCount > 0){
                std::size_t count = SampleCount > 10000 ? 10000 : SampleCount;
                sf_write_short(_kfile, Data, count);
                Data += count;
                SampleCount -= count;
            }
        }
    }

    void SoundIO::seek(U32 Second){
        if (_kfile){
            sf_count_t frameOffset = (sf_count_t)(Second * _ksampleRate);
            sf_seek(_kfile, frameOffset, SEEK_SET);
        }
    }

    std::string SoundIO::toLower(std::string &str){
        for (std::string::iterator i = str.begin(); i != str.end(); ++i)
            *i = (char)(std::tolower(*i));
        return str;
    }

    I32 SoundIO::getFileFormat(const std::string& FileName){
        // extract the extension from file name (default is wav)
        std::string ext = "wav";
        std::string::size_type pos = FileName.find_last_of(".");
        if (pos != std::string::npos)
            ext = FileName.substr(pos + 1);

        // match extracted extension with its format constant
        toLower(ext);
        if (ext == "wav"  ) return SF_FORMAT_WAV;
        if (ext == "ogg"  ) return SF_FORMAT_OGG;
        if (ext == "raw"  ) return SF_FORMAT_RAW;
        if (ext == "aif"  ) return SF_FORMAT_AIFF;
        if (ext == "aiff" ) return SF_FORMAT_AIFF;
        if (ext == "au"   ) return SF_FORMAT_AU;
        if (ext == "paf"  ) return SF_FORMAT_PAF;
        if (ext == "w64"  ) return SF_FORMAT_W64;
        if (ext == "svx"  ) return SF_FORMAT_SVX;
        if (ext == "nist" ) return SF_FORMAT_NIST;
        if (ext == "voc"  ) return SF_FORMAT_VOC;
        if (ext == "sf"   ) return SF_FORMAT_IRCAM;
        if (ext == "mat4" ) return SF_FORMAT_MAT4;
        if (ext == "mat5" ) return SF_FORMAT_MAT5;
        if (ext == "avr"  ) return SF_FORMAT_AVR;
        if (ext == "sd2"  ) return SF_FORMAT_SD2;
        if (ext == "flac" ) return SF_FORMAT_FLAC;
        if (ext == "caf"  ) return SF_FORMAT_CAF;
        if (ext == "wve"  ) return SF_FORMAT_WVE;
        if (ext == "pvf"  ) return SF_FORMAT_PVF;
        if (ext == "xi"   ) return SF_FORMAT_XI;
        if (ext == "htk"  ) return SF_FORMAT_HTK;
        if (ext == "sds"  ) return SF_FORMAT_SDS;
        if (ext == "mpc2k") return SF_FORMAT_MPC2K;
        if (ext == "rf64" ) return SF_FORMAT_RF64;

        // format not supported
        return -1;
    }

}
}

