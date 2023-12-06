#ifndef sat_polyblep_included
#define sat_polyblep_included
//----------------------------------------------------------------------

#include "base/utils/sat_math.h"

// tale
// http://www.kvraudio.com/forum/viewtopic.php?p=5269941#p5269941
// subtract result from naive saw..

template <typename T>
T SAT_PolyBlep(T t, T dt) {
  // 0 <= t < 1
  if (t < dt) {
    t /= dt;
    return t+t - t*t - 1.0; // 2 * (t - t^2/2 - 0.5)
  }
  // -1 < t < 0
  else if (t > (1.0 - dt)) {
    t = (t - 1.0) / dt;
    return t*t + t+t + 1.0; // 2 * (t^2/2 + t + 0.5)
  }
  else return 0.0;
}

//----------------------------------------------------------------------
#endif
