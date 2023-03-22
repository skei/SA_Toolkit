#ifndef sat_linux_lock_included
#define sat_linux_lock_included
//----------------------------------------------------------------------

#include "base/sat.h"

// -lpthread
#include <pthread.h>

class SAT_Lock {

//------------------------------
private:
//------------------------------

  pthread_mutex_t MMutex; // = PTHREAD_MUTEX_INITIALIZER;

//------------------------------
public:
//------------------------------

  SAT_Lock() {
    pthread_mutex_init(&MMutex,nullptr);
  }

  //----------

  ~SAT_Lock() {
    pthread_mutex_destroy(&MMutex);
  }

  //----------

  void lock(void) {
    pthread_mutex_lock(&MMutex);
  }

  //----------

  bool tryLock(void) {
    return (pthread_mutex_trylock(&MMutex) == 0);
  }

  //----------

  void unlock(void) {
    pthread_mutex_unlock(&MMutex);
  }

//------------------------------
};

//----------------------------------------------------------------------
#endif
