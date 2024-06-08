#ifndef sat_dc_filter_included
#define sat_dc_filter_included
//----------------------------------------------------------------------

// original author: lubomir i ivanov (for axonlib)
// used with permission

//----------------------------------------------------------------------

#include "sat.h"

/*
  DC offset filter.
  one pole, one zero DC offset filter from julius orion smith (jos).
  http://www-ccrma.stanford.edu/~jos/filters/
*/

template <typename T>
class SAT_DcFilter {

  private:

    T x1,y1;
    T R;

  public:

    SAT_DcFilter() {
      x1 = 0;
      y1 = 0;
      R = 0.999;
    }

    //----------

    T process(T AValue) {
      T y = AValue - x1 + (R*y1);// + SAT_FLOAT_DENORM;
      x1 = AValue;
      y1 = y;
      return y;// - SAT_FLOAT_DENORM;
    }

};

//----------------------------------------------------------------------
#endif

//----------------------------------------------------------------------

/*
  https://github.com/timowest/rogue/blob/master/dsp/filter.cpp

  // DCBlocker
  void DCBlocker::clear() {
    x1 = y1 = 0.0f;
  }
  void DCBlocker::setSamplerate(T r) {
    R = 1.0f - (M_PI * 2.0f * 20.0f / r);
  }
  void DCBlocker::process(T* input, T* output, int samples) {
    for (uint i = 0; i < samples; i++) {
      // y(n) = x(n) - x(n - 1) + R * y(n - 1)
      T y = input[i] - x1 + R * y1;
      x1 = input[i];
      y1 = y;
      output[i] = y;
    }
  }
*/

template <typename T>
class SAT_Dc2Filter {

  private:
    T R;
    T x1,y1;

  public:

    SAT_Dc2Filter() {
      x1 = 0.0f;
      y1 = 0.0f;
    }

    //----------

    void setSamplerate(T r) {
      R = 1.0f - (SAT_PI2 * 20.0f / r);
    }

    //----------

    T process(T AInput) {
      T y = AInput - x1 + R * y1;
      x1 = AInput;
      y1 = y;
      return y;
    }

};

