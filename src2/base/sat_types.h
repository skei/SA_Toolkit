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
#endif