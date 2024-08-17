#ifndef sat_types_included
#define sat_types_included
//----------------------------------------------------------------------

#include "base/type/sat_array.h"
#include "base/type/sat_buffer.h"
#include "base/type/sat_color.h"
#include "base/type/sat_complex.h"
#include "base/type/sat_dictionary.h"
#include "base/type/sat_list.h"
#include "base/type/sat_matrix.h"
#include "base/type/sat_point.h"
#include "base/type/sat_quaternion.h"
#include "base/type/sat_queue.h"
#include "base/type/sat_rect.h"
#include "base/type/sat_stack.h"
#include "base/type/sat_string.h"
#include "base/type/sat_tree.h"
#include "base/type/sat_vector.h"

//----------------------------------------------------------------------

typedef float             sat_sample_t;
typedef double            sat_param_t;
typedef std::atomic<bool> sat_atomic_bool_t;

//----------------------------------------------------------------------
// TODO: drop-in replacement for SAT_Queue, SAT_AtomicQueue, ...

//moodycamel::ConcurrentQueue<T>
//moodycamel::ReaderWriterQueue<T>

#ifdef SAT_DEBUG_MEMTRACE
  #define HAD_DEBUG_MEMTRACE
  #undef SAT_DEBUG_MEMTRACE
#endif

// these don't like our new/delete overloads..
#include "extern/cameron314/blockingconcurrentqueue.h"
#include "extern/cameron314/concurrentqueue.h"
#include "extern/cameron314/readerwriterqueue.h"

#ifdef HAD_DEBUG_MEMTRACE
  #define SAT_DEBUG_MEMTRACE
#endif

//----------------------------------------------------------------------
#endif