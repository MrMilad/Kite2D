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
