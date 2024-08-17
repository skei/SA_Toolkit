#ifndef sat_thread_pool_included
#define sat_thread_pool_included
//----------------------------------------------------------------------

#include "sat.h"
//#include "base/system/sat_thread.h"
//#include "base/system/sat_lock.h"


#define SAT_THREAD_POOL_MAX_THREADS 32

//----------------------------------------------------------------------

class SAT_ThreadPoolListener {
public:
  virtual void on_ThreadPoolListener_exec(uint32_t AIndex) {};
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ThreadPool {

  typedef moodycamel::BlockingConcurrentQueue<uint32_t> SAT_ThreadPoolQueue;

//------------------------------
private:
//------------------------------

  SAT_ThreadPoolListener* MListener = nullptr;
  uint32_t                MNumThreads = 0;
  pthread_t               MThreads[SAT_THREAD_POOL_MAX_THREADS];
  SAT_ThreadPoolQueue     MQueue;
  std::atomic<uint32_t>   MTasksRemaining {0};

//  pthread_mutex_t mutex;
//  pthread_cond_t condition;

//------------------------------
public:
//------------------------------

  SAT_ThreadPool(SAT_ThreadPoolListener* AListener, uint32_t ANumThreads) {
    SAT_TRACE;
    MListener = AListener;
    MNumThreads = ANumThreads;
    for (uint32_t i=0; i<ANumThreads; i++) {
      int res = pthread_create(&MThreads[i],nullptr,thread_pool_callback,this);
      if (res != 0) {
        SAT_PRINT("Error creating thread %i\n",res);
      }
    }
  }

  //----------

  virtual ~SAT_ThreadPool() {
    SAT_TRACE;
    for (uint32_t i=0; i<MNumThreads; i++) {
      //pthread_join(MThreads[i],nullptr);
      pthread_kill(MThreads[i],0);
    }
  }

//------------------------------
public:
//------------------------------

  // called in process(), after we have prepared the list of active voices
  // (audio thread)

  bool request_exec(uint32_t ACount) {
    MTasksRemaining = ACount;
    for (uint32_t i=0; i<ACount; i++) MQueue.enqueue(i);
    // pthread_mutex_lock(&mutex);
    while (MTasksRemaining > 0) {
    // pthread_cond_wait(&condition, &mutex);
    }
    // pthread_mutex_unlock(&mutex);
    return true;
  }

  //----------

  // called by threadpool for each task (in its own thread)
  // calls listener, and decreases remaining tasks by one

  void exec_task(uint32_t AIndex) {
    if (MListener) MListener->on_ThreadPoolListener_exec(AIndex);
    // pthread_mutex_lock(&mutex);
    MTasksRemaining -= 1;
    // pthread_cond_signal(&condition);
    // pthread_mutex_unlock(&mutex);

  };

//------------------------------
private:
//------------------------------

  // waits until there is an event in the queue, and processes it

  void thread_exec() {
    uint32_t index;
    while (true) {
      MQueue.wait_dequeue(index);
      exec_task(index);
    }
  }

  //----------

  // 'trampoline' into the SAT_ThreadPool class

  static
  void* thread_pool_callback(void* argument) {
    SAT_ThreadPool* threadpool = (SAT_ThreadPool*)argument;
    threadpool->thread_exec();
    return nullptr;
  }

};

//----------------------------------------------------------------------
#endif
