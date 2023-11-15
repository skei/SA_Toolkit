#ifndef mip_linkwitz_riley_filter_included
#define mip_linkwitz_riley_filter_included
//----------------------------------------------------------------------

/*
  http://musicdsp.org/showArchiveComment.php?ArchiveID=266
*/

// fc -> cutoff frequency
// pi -> 3.14285714285714
// srate -> sample rate

//----------------------------------------------------------------------

class MIP_LinkwitzRileyFilter {

  private:

    float fc;
    float srate;
    float a0,a1,a2,a3,a4;
    float    b1,b2,b3,b4;
    float xm1,xm2,xm3,xm4;
    float ym1,ym2,ym3,ym4;

  public:

    float getFrequency(void)  { return fc; }
    float getSampleRate(void) { return srate; }

  private:

    void setSampleRate(float sr) {
      srate = sr;
    }

    void setFrequency(float v) {
      fc = v;
    }

  public:

    MIP_LinkwitzRileyFilter() {
      fc = 0.0f;
      srate = 0.0f;
      a0 = a1 = a2 = a3 = a4 = 0.0f;
      b1 = b2 = b3 = b4 = 0.0f;
      xm1 = xm2 = xm3 = xm4 = 0.0f;
      ym1 = ym2 = ym3 = ym4 = 0.0f;
    }

    ~MIP_LinkwitzRileyFilter() {
    }

    void calcCoeffs(int32_t AMode=0) {
      float wc      = MIP_PI2 * fc;
      float wc2     = wc*wc;
      float wc3     = wc2*wc;
      float wc4     = wc2*wc2;
      float k       = wc / tan(MIP_PI*fc/srate);
      float k2      = k*k;
      float k3      = k2*k;
      float k4      = k2*k2;
      float sqrt2   = sqrt(2);
      float sq_tmp1 = sqrt2*wc3*k;
      float sq_tmp2 = sqrt2*wc*k3;
      float a_tmp   = 4*wc2*k2+2*sq_tmp1+k4+2*sq_tmp2+wc4;
      b1  = (4*(wc4+sq_tmp1-k4-sq_tmp2))/a_tmp;
      b2  = (6*wc4-8*wc2*k2+6*k4)/a_tmp;
      b3  = (4*(wc4-sq_tmp1+sq_tmp2-k4))/a_tmp;
      b4  = (k4-2*sq_tmp1+wc4-2*sq_tmp2+4*wc2*k2)/a_tmp;
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

    float process(float AValue/*, float* L, float* B, float* N, float* H*/) {
      float tempx = AValue;
      float tempy = a0*tempx + a1*xm1 + a2*xm2 + a3*xm3 + a4*xm4
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

//#undef sDenorm

//----------------------------------------------------------------------
#endif
