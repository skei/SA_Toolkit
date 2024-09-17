#ifndef sat_thread_pool_included
#define sat_thread_pool_included
//----------------------------------------------------------------------

// todo:
// - thread priority

/*

https://forum.nim-lang.org/t/11776

function thread_func(){
}

attr = attr_init();
setstacksize(attr, PTHREAD_STACK_MIN);
setschedpolicy(attr, SCHED_FIFO);
setschedparam(attr, priority=80);
setinheritsched(attr, PTHREAD_EXPLICIT_SCHED)
pthread_create(thread, attr, thread_func)

*/

//----------------------------------------------------------------------

#include "sat.h"

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
  SAT_ThreadPoolSemaphore MSemaphore;
//uint32_t                MBulkEnqueueBuffer[SAT_THREAD_POOL_MAX_VOICES];

//------------------------------
public:
//------------------------------

  // int policy;
  // struct sched_param param;
  // pthread_getschedparam(pthread_self(), &policy, &param);
  // int max_priority = sched_get_priority_max(policy);
  // // SAT_PRINT("max_priority: %i\n",max_priority); // prints 0
  // param.sched_priority = max_priority; // -90
  // pthread_setschedparam(pthread_self(), policy, &param);

  SAT_ThreadPool(SAT_ThreadPoolListener* AListener, uint32_t ANumThreads) {
    SAT_TRACE;
    MListener = AListener;
    MNumThreads = ANumThreads;

    for (uint32_t i=0; i<ANumThreads; i++) {

      // pthread_attr_t thread_attr;
      // pthread_attr_init(&thread_attr);
      // pthread_attr_setstacksize(&thread_attr,PTHREAD_STACK_MIN);          // setstacksize(attr, PTHREAD_STACK_MIN);
      // pthread_attr_setschedpolicy(&thread_attr, SCHED_FIFO);              // Set attributes to RR policy
      // struct sched_param param;
      // param.sched_priority = 80;
      // pthread_attr_setschedparam(&thread_attr, &param);                   // setschedparam(attr, priority=80);
      // pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED); // setinheritsched(attr, PTHREAD_EXPLICIT_SCHED)
      // int res = pthread_create(&MThreads[i],&thread_attr,thread_pool_callback,this);

      int res = pthread_create(&MThreads[i],nullptr,thread_pool_callback,this);
      if (res != 0) { SAT_PRINT("Error creating thread %i\n",res); }

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

    // ouch.. this crashed, randomly..
    // i don't think i understand the bulk stuff.. :-/

    //for (uint32_t i=0; i<ACount; i++) MBulkEnqueueBuffer[i] = i;
    //MQueue.enqueue_bulk<uint32_t*>(MBulkEnqueueBuffer,ACount);

    for (uint32_t i=0; i<ACount; i++) MQueue.enqueue(i);
    MSemaphore.wait();
    return true;
  }

  //----------

  // called (concurrently) by each worker thread
  // calls listener, and decreases remaining tasks by one

  void exec_task(uint32_t AIndex) {
    if (MListener) MListener->on_ThreadPoolListener_exec(AIndex);
    if (!--MTasksRemaining) MSemaphore.signal();


  };

//------------------------------
private:
//------------------------------

  // called (concurrently) by each worker thread
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
