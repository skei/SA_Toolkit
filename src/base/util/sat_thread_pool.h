#ifndef sat_thread_pool_included
#define sat_thread_pool_included
//----------------------------------------------------------------------

/*
  // Threadpool task queue
  BlockingConcurrentQueue<Task> q;
  // To create a task from any thread:
  q.enqueue(...);
  // On threadpool threads:
  Task task;
  while (true) {
    q.wait_dequeue(task);
    // Process task...
  }
*/

//----------------------------------------------------------------------

#include "sat.h"
#include "base/system/sat_thread.h"
//#include "base/system/sat_lock.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>

#include <condition_variable>

#define SAT_THREAD_POOL_MAX_THREADS 32

//----------------------------------------------------------------------

class SAT_ThreadPoolListener {
public:
  virtual void on_ThreadPoolListener_exec(uint32_t AIndex) {};
};

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
  std::atomic<uint32_t>   MTasksRemaining;

//------------------------------
public:
//------------------------------

  SAT_ThreadPool(SAT_ThreadPoolListener* AListener, uint32_t ANumThreads) {
    SAT_TRACE;
    MListener = AListener;
    MNumThreads = ANumThreads;
    for (uint32_t i=0; i<ANumThreads; i++) {
      int res = pthread_create(&MThreads[i],nullptr,thread_pool_func,this);
      if (res != 0) {
        SAT_PRINT("Error creating thread #%i\n",res);
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

  bool request_exec(uint32_t ANum) {
    MTasksRemaining = ANum;
    for (uint32_t i=0; i<ANum; i++) {
      MQueue.enqueue(i);
    }
    //while (MTasksRemaining > 0) { SAT_Sleep(1); }
    while (MTasksRemaining > 0) {}
    return true;
  }

  //----------

  void exec_task(uint32_t AIndex) {
    if (MListener) MListener->on_ThreadPoolListener_exec(AIndex);
    MTasksRemaining -= 1;
  };

//------------------------------
private:
//------------------------------

  // run on each thread..

  void thread_exec_tasks() {
    uint32_t index;
    while (true) {
      MQueue.wait_dequeue(index);
      exec_task(index);
    }
  }

  //----------

  static
  void* thread_pool_func(void* argument) {
    SAT_ThreadPool* threadpool = (SAT_ThreadPool*)argument;
    threadpool->thread_exec_tasks();
    return nullptr;
  }

};

//----------------------------------------------------------------------
#endif
