#ifndef KSEQIO_H
#define KSEQIO_H

#include <cstdio>
#include "Kite/system/kdefinitions.h"
#include "Kite/system/ktypes.h"

namespace Kite{
    class KSeqIO{
    public:
        KSeqIO();
        KSeqIO(const char *fileName, const KSeqIOAccTypes accessMode);
        ~KSeqIO();

        void openFile(const char *fileName, const KSeqIOAccTypes accessMode);
        void closeFile();

        void addText(const char *text);

    private:
        FILE *_kpFile;
        KSeqIOAccTypes _kaccMode;
        bool _kfIsOpen;
    };
}

#endif // KSEQIO_H
