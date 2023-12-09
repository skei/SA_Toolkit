#ifndef mip_dc_filter_included
#define mip_dc_filter_included
//----------------------------------------------------------------------

// original author: lubomir i ivanov (for axonlib)
// used with permission

//----------------------------------------------------------------------

#include "base/mip.h"
#include "base/mip_const.h"

/*
  DC offset filter.
  one pole, one zero DC offset filter from julius orion smith (jos).
  http://www-ccrma.stanford.edu/~jos/filters/
*/

class MIP_DcFilter {

  private:

    float x1,y1;
    float R;

  public:

    MIP_DcFilter() {
      x1 = 0;
      y1 = 0;
      R = 0.999;
    }

    //----------

    float process(float AValue) {
      float y = AValue - x1 + (R*y1) + MIP_FLOAT_DENORM;
      x1 = AValue;
      y1 = y;
      return y - MIP_FLOAT_DENORM;
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
  void DCBlocker::setSamplerate(float r) {
    R = 1.0f - (M_PI * 2.0f * 20.0f / r);
  }
  void DCBlocker::process(float* input, float* output, int samples) {
    for (uint i = 0; i < samples; i++) {
      // y(n) = x(n) - x(n - 1) + R * y(n - 1)
      float y = input[i] - x1 + R * y1;
      x1 = input[i];
      y1 = y;
      output[i] = y;
    }
  }
*/

class MIP_Dc2Filter {

  private:
    float R;
    float x1,y1;

  public:

    MIP_Dc2Filter() {
      x1 = 0.0f;
      y1 = 0.0f;
    }

    //----------

    void setSamplerate(float r) {
      R = 1.0f - (MIP_PI2 * 20.0f / r);
    }

    //----------

    float process(float AInput) {
      float y = AInput - x1 + R * y1;
      x1 = AInput;
      y1 = y;
      return y;
    }

};

