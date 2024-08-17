
https://github.com/cameron314/concurrentqueue/tree/master
https://github.com/cameron314/concurrentqueue/blob/master/samples.md

https://github.com/cameron314/readerwriterqueue

//----------------------------------------------------------------------

#include "concurrentqueue.h"
moodycamel::ConcurrentQueue<int> q;
q.enqueue(25);
int item;
bool found = q.try_dequeue(item);
assert(found && item == 25);

//----------------------------------------------------------------------

Threadpool task queue

  BlockingConcurrentQueue<Task> q;

  // To create a task from any thread:
  q.enqueue(...);

  // On threadpool threads:
  Task task;
  while (true) {
    q.wait_dequeue(task);
    // Process task...
  }

//----------------------------------------------------------------------
