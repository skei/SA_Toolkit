#ifndef sat_buffer_included
#define sat_buffer_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
// static buffer
//
//----------------------------------------------------------------------

// SIZE = buffer size


template <typename T, int SIZE>
class SAT_StaticBuffer {
private:
  T MBuffer[SIZE] = {0};
};

//----------------------------------------------------------------------
//
// dynamic buffer
//
//----------------------------------------------------------------------

// SIZE = initial size

template <typename T, int SIZE>
class SAT_DynamicBuffer {
private:
  T* MBuffer = nullptr;
};


//----------------------------------------------------------------------
#endif
