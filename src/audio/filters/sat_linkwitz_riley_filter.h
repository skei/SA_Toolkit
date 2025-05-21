#ifndef sat_linkwitz_riley_filter_included
#define sat_linkwitz_riley_filter_included
//----------------------------------------------------------------------

/*
  http://musicdsp.org/showArchiveComment.php?ArchiveID=266
*/

// fc -> cutoff frequency
// pi -> 3.14285714285714
// srate -> sample rate

//----------------------------------------------------------------------

template <typename T>
class SAT_LinkwitzRileyFilter {

  private:

    T fc;
    T srate;
    T a0,a1,a2,a3,a4;
    T    b1,b2,b3,b4;
    T xm1,xm2,xm3,xm4;
    T ym1,ym2,ym3,ym4;

  public:

    T getFrequency(void)  { return fc; }
    T getSampleRate(void) { return srate; }

  private:

    void setSampleRate(T sr) {
      srate = sr;
    }

    void setFrequency(T v) {
      fc = v;
    }

  public:

    SAT_LinkwitzRileyFilter() {
      fc = 0.0f;
      srate = 0.0f;
      a0 = a1 = a2 = a3 = a4 = 0.0f;
      b1 = b2 = b3 = b4 = 0.0f;
      xm1 = xm2 = xm3 = xm4 = 0.0f;
      ym1 = ym2 = ym3 = ym4 = 0.0f;
    }

    ~SAT_LinkwitzRileyFilter() {
    }

    void calcCoeffs(int32_t AMode=0) {
      T wc      = SAT_PI2 * fc;
      T wc2     = wc*wc;
      T wc3     = wc2*wc;
      T wc4     = wc2*wc2;
      T k       = wc / tan(SAT_PI*fc/srate);
      T k2      = k*k;
      T k3      = k2*k;
      T k4      = k2*k2;
      T sqrt2   = sqrt(2);
      T sq_tmp1 = sqrt2*wc3*k;
      T sq_tmp2 = sqrt2*wc*k3;
      T a_tmp   = 4*wc2*k2+2*sq_tmp1+k4+2*sq_tmp2+wc4;
      b1        = (4*(wc4+sq_tmp1-k4-sq_tmp2))/a_tmp;
      b2        = (6*wc4-8*wc2*k2+6*k4)/a_tmp;
      b3        = (4*(wc4-sq_tmp1+sq_tmp2-k4))/a_tmp;
      b4        = (k4-2*sq_tmp1+wc4-2*sq_tmp2+4*wc2*k2)/a_tmp;
      switch(AMode) {
        case 0: // lowpass
          a0 = wc4/a_tmp;
          a1 = 4*wc4/a_tmp;
          a2 = 6*wc4/a_tmp;
          a3 = a1;
          a4 = a0;
          break;
        case 1: // hipass
          a0 = k4/a_tmp;
          a1 = -4*k4/a_tmp;
          a2 = 6*k4/a_tmp;
          a3 = a1;
          a4 = a0;
          break;
      }
    }

    T process(T AValue/*, T* L, T* B, T* N, T* H*/) {
      T tempx = AValue;
      T tempy = a0*tempx + a1*xm1 + a2*xm2 + a3*xm3 + a4*xm4
                         - b1*ym1 - b2*ym2 - b3*ym3 - b4*ym4;
      xm4   = xm3;
      xm3   = xm2;
      xm2   = xm1;
      xm1   = tempx;
      ym4   = ym3;
      ym3   = ym2;
      ym2   = ym1;
      ym1   = tempy;
      return tempy;
    }

};

//----------------------------------------------------------------------
#endif
