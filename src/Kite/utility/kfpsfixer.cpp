#include "Kite/utility/kfpsfixer.h"
#include "Kite/system/ksystemutil.h"
namespace Kite{

    KFPSFixer::KFPSFixer(F64 fTimeInMillsec, bool useSleep):
        fps(fTimeInMillsec), useSleep(useSleep),
        m_startTime(0.0), m_firstTime(true)
    {}

    bool KFPSFixer::start(){
        if (m_firstTime){
            m_timer.start();
            m_startTime = m_timer.getElapsedTimeInMilliSec();
            m_firstTime = false;
            return true;
        }
        F64 mDelta =  m_timer.getElapsedTimeInMilliSec() - m_startTime;
        if (mDelta < fps){
            if (useSleep && (fps - mDelta) >= 2)
                Kite::sleep(100);
            return false;
        }
        m_timer.start();
        m_startTime = m_timer.getElapsedTimeInMilliSec();
        return true;
    }
}
