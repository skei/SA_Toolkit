#ifndef sat_string_included
#define sat_string_included
//----------------------------------------------------------------------

#include "base/types/sat_buffer.h"


//----------------------------------------------------------------------
//
// static string
//
//----------------------------------------------------------------------

// SIZE = buffer size


template <int SIZE>
class SAT_StaticString
: public SAT_StaticBuffer<char,SIZE> {
};

//----------------------------------------------------------------------
//
// dynamic string
//
//----------------------------------------------------------------------

// SIZE = initial size

template <int SIZE>
class SAT_DynamicString
: public SAT_DynamicBuffer<char,SIZE> {
};

//----------------------------------------------------------------------
#endif
