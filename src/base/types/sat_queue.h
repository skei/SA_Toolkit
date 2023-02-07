#ifndef sat_queue_included
#define sat_queue_included
//----------------------------------------------------------------------

/*
  see:
  http://linux-audio.4202.n7.nabble.com/Realtime-inter-thread-communication-td99157.html

  thread-safe, lock/wait-free queue
  single producer, single consumer..

  todo: MReadPos/MWritePos volatile?

  todo:
    can something be optimized by the compiler?
    reordering of instructions?
*/

#include "base/sat.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// read/write single items

template<class _T, int SIZE>
class SAT_SPSCQueue {

//------------------------------
private:
//------------------------------

  _T*       MBuffer;
  uint32_t  MBufferSize;
  uint32_t  MBufferMask;
  uint32_t  MReadPos;
  uint32_t  MWritePos;

//------------------------------
public:
//------------------------------

  SAT_SPSCQueue() {
    MBuffer     = (_T*)malloc(SIZE*sizeof(_T));
    MBufferSize = SIZE;
    MBufferMask = SIZE - 1;
    MReadPos    = 0;
    MWritePos   = 0;
  }

  //----------

  ~SAT_SPSCQueue() {
    free(MBuffer);
  }

//------------------------------
public:
//------------------------------

  /*
    WARNING:
    - loses (new) events if buffer is full (when writing)
  */

  void clear(void) {
    MReadPos  = 0;
    MWritePos = 0;
  }

  //----------

  /*
    notes to self:
    TODO: remove when we are 110% sure it works

    (wp-rp) & FBufferMask = 'items in buffer'
    in write(), we avoid filling the entire buffer, by letting there always be
    at least one free item, because read() could misinterpret a full buffer
    as empty..
    if the write fail (returns false), what should we do?
    - ignore, losing events..
    - sleep a little and try again? (spin?)
    - the reading happens at the start of process, so in worst case,
      if buffer is full already, we must wait until the next processBlock,
      (reaper's pause doesn't call process, and doesn't let us know it's paused, i think..)
    - buffer/cache updates in case the original buffer is full? double-buffer..
    buffer is emptied at the start of process()
  */

  bool read(_T* AItem) {
    uint32_t wp = MWritePos;
    uint32_t rp = MReadPos;
    if (( (wp-rp) & MBufferMask ) > 0) {
      *AItem = MBuffer[rp];
      uint32_t next = (rp+1) & MBufferMask;
      MReadPos = next;
      return true;
    }
    return false;
  }

  //----------

  /*
    returns true if item written to queue,
    false if queue is full
  */

  bool write(_T AItem) {
    uint32_t rp = MReadPos;
    uint32_t wp = MWritePos;
    if (((wp-rp) & MBufferMask) < (MBufferSize-1)) { // -1 to avoid filling the entire buffer.. .. which could be mis-interpreted by read() as empty..
      MBuffer[wp] = AItem;
      uint32_t next = (wp+1) & MBufferMask;
      MWritePos = next;
      return true;
    }
    return false;
  }

  //----------

  /*
    check if there is an event with same index already in the queue
    todo: a) update value (replace event)
          b) remove old event, insert new at end of queue
  */

  // this is not thread-safe !!

  //bool hasEventWithIndex(int32_t AIndex) {
  //  //result := False;
  //  uint32_t = MReadPos;
  //  uint32_t = MWritePos;
  //  while (rpos != wpos) {
  //    if FBuffer[rpos].FIndex =;
  //  }
  //}

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// write single, read buffer (all)

template<class _T, int SIZE>
class SAT_SPSCBufferQueue {

  //------------------------------
  //
  //------------------------------

//------------------------------
private:
//------------------------------

  _T                MData[SIZE];
  /*volatile*/ int  MWritePos;  // volatile?
  /*volatile*/ int  MReadPos;

//------------------------------
  public:
//------------------------------

  /*
    safe for single-reader, single-writer ??
    (thread-safe?) circular/ring-buffer
    - write one item
    - read multiple items
  */

  SAT_SPSCBufferQueue() {
    memset(MData,0,SIZE*(sizeof(_T)));
    MWritePos = 0;
    MReadPos = 0;
  }

  //----------

  void write(_T AData) {
    MData[MWritePos] = AData;
    MWritePos = (MWritePos + 1) % SIZE; // & SIZE if ^2
  }

  //----------

  // return num items copied into buffer

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

  //----------

  //TODO: return one:
  //_T* read(void) {
  //}

};

//----------------------------------------------------------------------
#endif
