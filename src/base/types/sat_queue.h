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

#include "base/sat.h"

//----------------------------------------------------------------------
//
// lock free queue
//
//----------------------------------------------------------------------

/*
  assumptions:
  - single producer
  - single consumer
  - reading/writing single uint32_t is atomic (x86)
*/

//----------

template<class T, int SIZE>
class SAT_LockFreeQueue {

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

  SAT_LockFreeQueue() {
    MBuffer     = (T*)malloc(SIZE*sizeof(T));
    MBufferSize = SIZE;
    MBufferMask = SIZE - 1;
    MReadPos    = 0;
    MWritePos   = 0;
  }

  //----------

  ~SAT_LockFreeQueue() {
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
// mpmc queue
//
//----------------------------------------------------------------------

// wrapper for MPMCQueue
#include "extern/rigtorp/MPMCQueue.h"

//----------

template<class T, int SIZE>
class SAT_MPMCQueue {

//------------------------------
private:
//------------------------------

  rigtorp::MPMCQueue<T> MQueue = rigtorp::MPMCQueue<T>(SIZE);

//------------------------------
public:
//------------------------------

  SAT_MPMCQueue() {
  }

  //----------

  ~SAT_MPMCQueue() {
  }

//------------------------------
public:
//------------------------------

  //void clear(void) {
  //}

  bool read(T* AItem) {
    return MQueue.try_pop(AItem);
  }

  bool write(T AItem) {
    return MQueue.try_push(AItem);
  }

};


//----------------------------------------------------------------------
#endif





#if 0

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
class SAT_SPSCQueue {

//------------------------------
private:
//------------------------------

  _T                MData[SIZE];
  /*volatile*/ int  MWritePos;  // volatile?
  /*volatile*/ int  MReadPos;

//------------------------------
  public:
//------------------------------

  SAT_SPSCBufferQueue() {
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

#endif // 0

