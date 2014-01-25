#include "src/Kite/system/win32/mutex.h"

namespace Kite{
namespace Internal{

    Mutex::Mutex(){
        InitializeCriticalSection(&_ksysmutex);
    }

    Mutex::~Mutex(){
        DeleteCriticalSection(&_ksysmutex);
    }

    void Mutex::lock(){
        EnterCriticalSection(&_ksysmutex);
    }

    void Mutex::unlock(){
        LeaveCriticalSection(&_ksysmutex);
    }

}
}
