#ifndef sat_onepole_filter_included
#define sat_onepole_filter_included
//----------------------------------------------------------------------

// original author: lubomir i ivanov (for axonlib)
// used with permission

//----------------------------------------------------------------------

/*
  brief one pole filters
  leaky integrator with various routings to get shelving and high pass

  0: no filter
  1: lp
  2: hp
  3: ls
  4: hs
*/

template <typename T>
class SAT_OnePoleFilter {

  private:

    uint32_t  FType;
    bool      FInterpolate;
    T     a0, b1, y;
    T     _gain, _gain2;
    T     i_a0, i_b1;
    T     d_a0, d_b1;
    T     s_a0, s_b1;

  private:

    void reset_p(void) {
      i_a0 = 0;
      i_b1 = 0;
      d_a0 = 0;
      d_b1 = 0;
      s_a0 = 0;
      s_b1 = 0;
    }

  public:

    SAT_OnePoleFilter() {
      reset_p();
      FInterpolate = false;
      FType = 0;
      setup();
    }

    /*
      setup filter

      filter types:
      0 - no filter
      1 - low pass
      2 - high pass
      3 - low shelf
      4 - high shelf
      type unsigned int - filter type (0 - 4)
      srate float - sample rate (Hz)
      freq float - frequency (Hz)
      gain float - gain (dB)
      intrp bool - parameter interpolation (on / off)
    */

    void setup(uint32_t AType=0, T ASRate=44100, T AFreq=11000, T AGain=1, bool AInterp=false) {
      //gain
      _gain = 1; //axDB2Lin(gain);
      _gain2 = _gain - 1;
      // reset params if filter has changed
      if (AType!=FType) reset_p();
      // if type out of range set to 0
      FType = AType;
      if ((FType < 0) || (FType > 4)) FType = 0;
      // coeff
      b1 = exp(-SAT_PI2*AFreq / ASRate);
      a0 = 1 - b1;
      // has interpolation
      FInterpolate = true;
      if (!AInterp) {
        i_a0 = a0;
        i_b1 = b1;
        FInterpolate = false;
      }
    }

    /*
      interpolate filter coeff
      call this from axFormat::doProcessBlock(..)
      or each N samples: myfilter.interpolate(N)
    */

    void interpolate(uint32_t AFrames) {
      if ((FInterpolate) && (FType > 0)) {  // ! > ?
        T inv_sb = 1.0f / AFrames;
        // ---
        d_a0 = (a0 - s_a0)*inv_sb;
        i_a0 = s_a0;
        s_a0 = a0;
        // ---
        d_b1 = (b1 - s_b1)*inv_sb;
        i_b1 = s_b1;
        s_b1 = b1;
      }
    }

    T process(T AInput) {
      if (FInterpolate) {
        i_a0 += d_a0;
        i_b1 += d_b1;
      }
      switch (FType) {
        case 0: // no filter
          return AInput * _gain;
          break;
        case 1: // lp
          y = (i_a0 * AInput) + (i_b1*y) + SAT_FLOAT_DENORM;
          return (y - SAT_FLOAT_DENORM) * _gain;
          break;
        case 2: // hp
          y = (i_a0 * AInput) + (i_b1*y) + SAT_FLOAT_DENORM;
          return (AInput - y - SAT_FLOAT_DENORM) * _gain;
          break;
        case 3: // ls
          y = (i_a0*AInput) + (i_b1*y) + SAT_FLOAT_DENORM;
          return AInput + ((y - SAT_FLOAT_DENORM) * _gain2);
          break;
        case 4: // hs
          y = (i_a0*AInput) + (i_b1*y) + SAT_FLOAT_DENORM;
          return AInput + ((AInput - y - SAT_FLOAT_DENORM) * _gain2);
          break;
      }
    }

};

//----------------------------------------------------------------------
#endif
