#ifndef sat_apple_filter_included
#define sat_apple_filter_included
//----------------------------------------------------------------------

// original author: lubomir i ivanov (for axonlib)
// used with permission

//----------------------------------------------------------------------

#include "base/utils/sat_math.h"

template <typename T>
class SAT_AppleFilter {

  private:

    T mX1,mX2,mY1,mY2;
    T mA0,mA1,mA2;
    T mB1,mB2;
    T res,sx,cx;
    T srate;

  public:

    SAT_AppleFilter() {
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

    void setSamplerate(T ASampleRate) {
      srate = ASampleRate;
    }

    //----------

    /*
      ACutoff := 0..100, default 100
    */

    void setCutoff(T ACutoff) {
      sx = 16 + ACutoff * 1.20103;
      //cx = trunc(exp(sx*{log2}ln(1.059))*8.17742); // ln,log2,log10,logn
      cx = SAT_Trunc(exp(sx*log2(1.059))*8.17742); // ln,log2,log10,logn
    }

    //----------

    /*
      AResonance := -25..25,, default 0
    */

    void setResonance(T AResonance) {
      res = AResonance;
    }

    //----------

    void calcCoefficients(void) {
      //coeffcients
      T cutoff = 2 * cx / srate;
      res = pow(10, 0.05 * -res);
      T k = 0.5 * res * sin(SAT_PI * cutoff);
      T c1 = 0.5 * (1 - k) / (1 + k);
      T c2 = (0.5 + c1) * cos(SAT_PI * cutoff);
      T c3 = (0.5 + c1 - c2) * 0.25;
      mA0 = 2 * c3;
      mA1 = 2 * 2 * c3;
      mA2 = 2 * c3;
      mB1 = 2 * -c2;
      mB2 = 2 * c1;
    }

    //----------

    T process(T AInput) {
      T result = mA0*AInput + mA1*mX1 + mA2*mX2 - mB1*mY1 - mB2*mY2;
      mX2 = mX1;
      mX1 = AInput;
      mY2 = mY1;
      mY1 = result;
      return result;
    }

};

//----------------------------------------------------------------------
#endif

