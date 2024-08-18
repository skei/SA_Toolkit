#ifndef sat_thread_pool_included
#define sat_thread_pool_included
//----------------------------------------------------------------------

// todo:
// - thread priority

#include "sat.h"
#include <semaphore.h>

#define SAT_THREAD_POOL_MAX_THREADS 32
#define SAT_THREAD_POOL_MAX_VOICES  4096

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
  typedef moodycamel::LightweightSemaphore              SAT_ThreadPoolSemaphore;

//------------------------------
private:
//------------------------------

  SAT_ThreadPoolListener* MListener = nullptr;
  uint32_t                MNumThreads = 0;
  pthread_t               MThreads[SAT_THREAD_POOL_MAX_THREADS];
  SAT_ThreadPoolQueue     MQueue;
  std::atomic<uint32_t>   MTasksRemaining {0};
  // sem_t                MSemaphore;
  SAT_ThreadPoolSemaphore MSemaphore;
  uint32_t                MBulkEnqueueBuffer[SAT_THREAD_POOL_MAX_VOICES];

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

      // int policy;
      // struct sched_param param;
      // pthread_getschedparam(/*pthread_self()*/MThreads[i], &policy, &param);
      // param.sched_priority = sched_get_priority_max(policy);
      // pthread_setschedparam(/*pthread_self()*/MThreads[i], policy, &param);  

    }
    //int res = sem_init(&MSemaphore,0,0);
  }

  //----------

  virtual ~SAT_ThreadPool() {
    SAT_TRACE;
    for (uint32_t i=0; i<MNumThreads; i++) {
      //pthread_join(MThreads[i],nullptr);
      pthread_kill(MThreads[i],0);
    }
    //sem_destroy(&MSemaphore);
  }

//------------------------------
public:
//------------------------------

  // called in process(), after we have prepared the list of active voices
  // (audio thread)

  bool request_exec(uint32_t ACount) {
    MTasksRemaining = ACount;

    // for (uint32_t i=0; i<ACount; i++) MQueue.enqueue(i);
    for (uint32_t i=0; i<ACount; i++) MBulkEnqueueBuffer[i] = i;
    MQueue.enqueue_bulk<uint32_t*>(MBulkEnqueueBuffer,ACount);

    // while (MTasksRemaining > 0) {}
    // int res = sem_wait(&MSemaphore);
    MSemaphore.wait();
    return true;
  }

  //----------

  // called by threadpool for each task (in its own thread)
  // calls listener, and decreases remaining tasks by one

  void exec_task(uint32_t AIndex) {
    if (MListener) MListener->on_ThreadPoolListener_exec(AIndex);
    // MTasksRemaining -= 1;
    // if (!--MTasksRemaining) sem_post(&MSemaphore);
    if (!--MTasksRemaining) MSemaphore.signal();


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

    // int policy;
    // struct sched_param param;
    // pthread_getschedparam(pthread_self(), &policy, &param);
    // int max_priority = sched_get_priority_max(policy);
    // // SAT_PRINT("max_priority: %i\n",max_priority); // prints 0
    // param.sched_priority = max_priority; // -90
    // pthread_setschedparam(pthread_self(), policy, &param);

    SAT_ThreadPool* threadpool = (SAT_ThreadPool*)argument;
    threadpool->thread_exec();
    return nullptr;
  }

};

//----------------------------------------------------------------------
#endif
