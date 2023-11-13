#ifndef sat_win32_lock_included
#define sat_win32_lock_included
//----------------------------------------------------------------------

#include "base/sat.h"

//#ifdef SAT_WIN32
//
//  //#include <windows.h>
//
//  class SAT_Lock {
//    private:
//      CRITICAL_SECTION  MCriticalSection;
//
//    public:
//
//      SAT_Lock() {
//        InitializeCriticalSection(&MCriticalSection);
//      }
//
//      ~SAT_Lock() {
//        DeleteCriticalSection(&MCriticalSection);
//      }
//
//      void lock(void) {
//        EnterCriticalSection(&MCriticalSection);
//      }
//
//      bool tryLock(void) {
//        return TryEnterCriticalSection( &MCriticalSection );
//      }
//
//      void unlock(void) {
//        LeaveCriticalSection(&MCriticalSection);
//      }
//
//  };
//
//#endif // SAT_WIN32
//----------------------------------------------------------------------
#endif
