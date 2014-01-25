#ifndef KFPSFIXER_H
#define KFPSFIXER_H

#include "Kite/system/ksystemdef.h"
#include "Kite/system/ktimer.h"

namespace Kite{
    class KFPSFixer{
    public:
        KFPSFixer(F64 fTimeInMillsec = 0.0f, bool useSleep = false);

        bool start();

        F64 fps;
        bool useSleep;
    private:
        KTimer m_timer;
        F64 m_startTime;
        bool m_firstTime;
    };
}

#endif // KFPSFIXER_H
