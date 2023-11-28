#ifndef sat_queue_included
#define sat_queue_included
//----------------------------------------------------------------------

/*

  "..nothing will read the value until the size of the container is
  incremented, so you don't care if the orders of writes of the value itself
  happen in a random order, you only care that the value must be fully written
  before the count is increased.."

*/

//----------------------------------------------------------------------

#include "sat.h"

//----------------------------------------------------------------------
//
// lock free (spsc)
//
//----------------------------------------------------------------------

/*
  assumptions:
  - single producer
  - single consumer
  - reading/writing single uint32_t is atomic (x86)
  - todo: malloc'ed buffer?
*/

//----------

template<class T, int SIZE>
class SAT_Queue {

//------------------------------
private:
//------------------------------

  T*        MBuffer;
  uint32_t  MBufferSize;
  uint32_t  MBufferMask;
  uint32_t  MReadPos;
  uint32_t  MWritePos;

//------------------------------
public:
//------------------------------

  SAT_Queue() {
    MBuffer     = (T*)malloc(SIZE*sizeof(T));
    MBufferSize = SIZE;
    MBufferMask = SIZE - 1;
    MReadPos    = 0;
    MWritePos   = 0;
  }

  //----------

  ~SAT_Queue() {
    free(MBuffer);
  }

//------------------------------
public:
//------------------------------

  bool write(T AItem) {
    uint32_t rp = MReadPos;
    uint32_t wp = MWritePos;
    if (((wp - rp) & MBufferMask) < (MBufferSize - 1)) {
      MBuffer[wp] = AItem;
      uint32_t next = (wp + 1) & MBufferMask;
      MWritePos = next;
      return true;
    }
    return false;
  }

  //----------

  bool read(T* AItem) {
    uint32_t wp = MWritePos;
    uint32_t rp = MReadPos;
    if (((wp - rp) & MBufferMask) > 0) {
      *AItem = MBuffer[rp];
      uint32_t next = (rp + 1) & MBufferMask;
      MReadPos = next;
      return true;
    }
    return false;
  }

  //----------

  int32_t size() {
    uint32_t rp = MReadPos;
    uint32_t wp = MWritePos;
    return (wp - rp) & MBufferMask;
  }

};

//----------------------------------------------------------------------
//
// lock free (spsc, buffer)
//
//----------------------------------------------------------------------

/*
  http://linux-audio.4202.n7.nabble.com/Realtime-inter-thread-communication-td99157.html
  thread-safe, lock/wait-free queue
  single producer, single consumer..
  todo: MReadPos/MWritePos volatile?
  can something be optimized by the compiler?
  reordering of instructions?
*/


// write single, read buffer (all)

template<class _T, int SIZE>
class SAT_QueueBuffer {

//------------------------------
private:
//------------------------------

  _T                MData[SIZE];
  /*volatile*/ int  MWritePos;  // volatile?
  /*volatile*/ int  MReadPos;

//------------------------------
  public:
//------------------------------

  SAT_QueueBuffer() {
    memset(MData,0,SIZE*(sizeof(_T)));
    MWritePos = 0;
    MReadPos = 0;
  }

  void write(_T AData) {
    MData[MWritePos] = AData;
    MWritePos = (MWritePos + 1) % SIZE; // & SIZE if ^2
  }

  int read(_T* ABuffer) {
    int count = 0;
    int writepos = MWritePos;
    if (MReadPos > writepos) {
      memcpy(
        (char*)&ABuffer[count],
        (char*)&MData[MReadPos],
        (SIZE - MReadPos) * sizeof(_T)
      );
      count = SIZE - MReadPos;
      MReadPos = 0;
    }
    memcpy(
      (char*)&ABuffer[count],
      (char*)&MData[MReadPos],
      (writepos - MReadPos) * sizeof(_T)
    );
    count += writepos - MReadPos;
    MReadPos = writepos;
    return count;
  }

};


//----------------------------------------------------------------------
//
// atomic queue (spsc)
//
//----------------------------------------------------------------------

// https://www.linkedin.com/pulse/lock-free-single-producer-consumer-queue-c11-sander-jobing

/*
  File:   SpScLockFreeQueue.h
  Author: Sander Jobing
  Created on July 29, 2017, 5:17 PM
  This class implements a Single Producer - Single Consumer lock-free and
  wait-free queue. Only 1 thread can fill the queue, another thread can read
  from the queue, but no more threads are allowed. This lock-free queue
  is a fifo queue, the first element inserted is the first element which
  comes out.
  Thanks to Timur Doumler, Juce
  https://www.youtube.com/watch?v=qdrp6k4rcP4
*/

#include <array>
#include <atomic>
#include <cassert>

template <typename T, size_t SIZE>
class SAT_AtomicQueue {

//------------------------------
private:
//------------------------------

  static constexpr size_t     MBufferSize  = SIZE + 1;
  std::array<T, MBufferSize>  MBuffer;
  std::atomic<size_t>         MReadPos    = {0};
  std::atomic<size_t>         MWritePos   = {0};

//------------------------------
public:
//------------------------------

  // Asserts when the underlying type is not lock free

  SAT_AtomicQueue() {
    std::atomic<size_t> test;
    assert(test.is_lock_free());
  }

  //----------

  //SAT_AtomicQueue(const SAT_AtomicQueue& src) = delete;

  //----------

  virtual ~SAT_AtomicQueue() {
  }

//------------------------------
public:
//------------------------------

  // Pushes an element to the queue
  // element  The element to add
  // True when the element was added, false when the queue is full

  bool write(const T& element) {
    const size_t oldWritePosition = MWritePos.load();
    const size_t newWritePosition = getPositionAfter(oldWritePosition);
    const size_t readPosition = MReadPos.load();
    if (newWritePosition == readPosition) {
      // The queue is full
      return false;
    }
    MBuffer[oldWritePosition] = element;
    MWritePos.store(newWritePosition);
    return true;
  }

  //----------

  // Pops an element from the queue
  // element The returned element
  // True when succeeded, false when the queue is empty

  bool read(T& element) {
    if (empty()) {
      // The queue is empty
      return false;
    }
    const size_t readPosition = MReadPos.load();
    element = std::move(MBuffer[readPosition]);
    MReadPos.store(getPositionAfter(readPosition));
    return true;
  }

  //----------

  // return True when empty

  bool empty() const noexcept {
    bool isEmpty = false;
    const size_t readPosition = MReadPos.load();
    const size_t writePosition = MWritePos.load();
    if (readPosition == writePosition) {
      isEmpty = true;
    }
    return isEmpty;
  }

  //----------

  // Clears the content from the queue

  void clear() noexcept {
    const size_t readPosition = MReadPos.load();
    const size_t writePosition = MWritePos.load();
    if (readPosition != writePosition) {
      MReadPos.store(writePosition);
    }
  }

  //----------

  // Returns the maximum size of the queue
  // The maximum number of elements the queue can hold

  constexpr size_t max_size() const noexcept {
    return MBufferSize - 1;
  }

  //----------

  // Returns the actual number of elements in the queue
  // The actual size or 0 when empty

  size_t size() const noexcept {
    const size_t readPosition = MReadPos.load();
    const size_t writePosition = MWritePos.load();
    if (readPosition == writePosition) {
      return 0;
    }
    size_t size = 0;
    if (writePosition < readPosition) {
      size = MBufferSize - readPosition + writePosition;
    }
    else {
      size = writePosition - readPosition;
    }
    return size;
  }

  //----------

  static constexpr size_t getPositionAfter(size_t pos) noexcept {
    return ((pos + 1 == MBufferSize) ? 0 : pos + 1);
  }

};

//----------------------------------------------------------------------
//
// atomic (mpmc)
//
//----------------------------------------------------------------------

// // wrapper for MPMCQueue
// #include "extern/rigtorp/MPMCQueue.h"
//
// //----------
//
// template<class T, int SIZE>
// class SAT_MPMCQueue {
//
// //------------------------------
// private:
// //------------------------------
//
//   rigtorp::MPMCQueue<T> MQueue = rigtorp::MPMCQueue<T>(SIZE);
//
// //------------------------------
// public:
// //------------------------------
//
//   SAT_MPMCQueue() {
//   }
//
//   //----------
//
//   ~SAT_MPMCQueue() {
//   }
//
// //------------------------------
// public:
// //------------------------------
//
//   //void clear(void) {
//   //}
//
//   bool read(T* AItem) {
//     return MQueue.try_pop(AItem);
//   }
//
//   bool write(T AItem) {
//     return MQueue.try_push(AItem);
//   }
//
// };


//----------------------------------------------------------------------
#endif





#if 0


#endif // 0
