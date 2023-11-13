#ifndef mip_apple_filter_included
#define mip_apple_filter_included
//----------------------------------------------------------------------

// original author: lubomir i ivanov (for axonlib)
// used with permission

//----------------------------------------------------------------------

#include "common/mip_math.h"

class MIP_AppleFilter {

  private:

    float mX1,mX2,mY1,mY2;
    float mA0,mA1,mA2;
    float mB1,mB2;
    float res,sx,cx;
    float srate;

  public:

    MIP_AppleFilter() {
      mX1 = 0;
      mX2 = 0;
      mY1 = 0;
      mY2 = 0;
      mA0 = 0;
      mA1 = 0;
      mA2 = 0;
      mB1 = 0;
      mB2 = 0;
      res = 0;
      sx = 0;
      cx = 0;
      srate = 0;
    }

    //----------

    void setSamplerate(float ASampleRate) {
      srate = ASampleRate;
    }

    //----------

    /*
      ACutoff := 0..100, default 100
    */

    void setCutoff(float ACutoff) {
      sx = 16 + ACutoff * 1.20103;
      //cx = trunc(exp(sx*{log2}ln(1.059))*8.17742); // ln,log2,log10,logn
      cx = MIP_Trunc(exp(sx*log2(1.059))*8.17742); // ln,log2,log10,logn
    }

    //----------

    /*
      AResonance := -25..25,, default 0
    */

    void setResonance(float AResonance) {
      res = AResonance;
    }

    //----------

    void calcCoefficients(void) {
      //coeffcients
      float cutoff = 2 * cx / srate;
      res = pow(10, 0.05 * -res);
      float k = 0.5 * res * sin(MIP_PI * cutoff);
      float c1 = 0.5 * (1 - k) / (1 + k);
      float c2 = (0.5 + c1) * cos(MIP_PI * cutoff);
      float c3 = (0.5 + c1 - c2) * 0.25;
      mA0 = 2 * c3;
      mA1 = 2 * 2 * c3;
      mA2 = 2 * c3;
      mB1 = 2 * -c2;
      mB2 = 2 * c1;
    }

    //----------

    float process(float AInput) {
      float result = mA0*AInput + mA1*mX1 + mA2*mX2 - mB1*mY1 - mB2*mY2;
      mX2 = mX1;
      mX1 = AInput;
      mY2 = mY1;
      mY1 = result;
      return result;
    }

};

//----------------------------------------------------------------------
#endif

