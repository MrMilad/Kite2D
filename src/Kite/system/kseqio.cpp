#include "Kite/system/kseqio.h"

namespace Kite{

    KSeqIO::KSeqIO():
        _kfIsOpen(false)
    {}

    KSeqIO::~KSeqIO(){
        this->closeFile();
    }

    KSeqIO::KSeqIO(const char *fileName, const KSeqIOAccTypes accessMode):
        _kfIsOpen(false)
    {
        this->openFile(fileName, accessMode);
    }

    void KSeqIO::openFile(const char *fileName, const KSeqIOAccTypes accessMode){
        if (_kfIsOpen)
            this->closeFile();
        const char *aMode[] = {"r", "w", "a", "w+", "r+", "a+"};
        int aModeCount = 0;
        switch(accessMode){
        case KSEQIO_ACC_READ:
            aModeCount = 0;
            break;
        case KSEQIO_ACC_WRITE:
            aModeCount = 1;
            break;
        case KSEQIO_ACC_APPEND:
            aModeCount = 2;
            break;
        case KSEQIO_ACC_RW:
            aModeCount = 3;
            break;
        case KSEQIO_ACC_RWEX:
            aModeCount = 4;
            break;
        case KSEQIO_ACC_RWTOEND:
            aModeCount = 5;
            break;
        }
        KDEBUG_ASSERT((_kpFile = fopen(fileName, aMode[aModeCount])));
        _kfIsOpen = true;
        _kaccMode = accessMode;
    }

    void KSeqIO::closeFile(){
        KDEBUG_ASSERT(fclose(_kpFile));
    }

    void KSeqIO::addText(const char *text){
        if (!_kfIsOpen || _kaccMode == KSEQIO_ACC_READ){
            KDEBUG_PRINT("file is not open or opened with ""read"" flag.");
            KDEBUG_BREAK();
        }
        KDEBUG_ASSERT(fputs(text, _kpFile));
    }

}
