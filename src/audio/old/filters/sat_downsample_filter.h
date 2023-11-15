#ifndef mip_downsample_filter_included
#define mip_downsample_filter_included
//----------------------------------------------------------------------


#if 0

//----------------------------------------------------------------------
// http://www.kvraudio.com/forum/viewtopic.php?p=3784986#p3784986
// 32 tap polyphase brick wall FIR filter using Kaiser window.
//----------------------------------------------------------------------

//----------

#define BUFFER_SIZE   256
//#define FACTOR      8
#define FIR_SIZE      32
#define FIR_MASK      (FIR_SIZE-1)

const float KDownSample_Coefficients[32] = {
 -0.0f,         1.06988606E-4f, 4.9367646E-4f,0.0013906843f,
  0.0030779035f,0.0058494993f,  0.009964011f, 0.015586544f,
  0.022733344f, 0.031230453f,   0.040697157f, 0.05056145f,
  0.060109124f, 0.06856181f,    0.07517301f,  0.07932711f,
  0.08062468f,  0.078939274f,   0.07443558f,  0.06754571f,
  0.058907907f, 0.04927845f,    0.039432116f, 0.030067796f,
  0.021733874f, 0.014783548f,   0.009363905f, 0.005436304f,
  0.00282024f,  0.0012496994f,  4.303706E-4f, 8.779568E-5f,
};

//----------

class MIP_DownSampleFilter {
  private:
    // 32 long normalized Kaiser (param = 6.4) windowed sinc function
    float MInput[BUFFER_SIZE];
    int32 MIndex;
  public:
    MIP_DownSampleFilter() {
      reset();
    }
    void reset(void) {
      //Arrays.fill(input, 0);
      MIndex = 0;
    }
    //Down sample buffer to a single output sample. Assume buffer length = FACTOR.
    float process(float* buffer, uint32 FACTOR) {
      float out;
      float s = buffer[0];
      MInput[MIndex] = s;
      s *= SDownSample_Coefficients[0];
      int Z,z;
      for (Z=1,z=MIndex-1; Z<FIR_SIZE; --z,++Z) {
        s += SDownSample_Coefficients[Z] * MInput[z&FIR_MASK];
      }
      MIndex = (MIndex + 1) & FIR_MASK;
      out = s;
      for(uint32 i=1; i<FACTOR; i++) {
        MInput[MIndex] = buffer[i];
        MIndex = (MIndex + 1) & FIR_MASK;
      }
      return out;
    }
};

//----------

#undef BUFFER_SIZE
#undef FACTOR
#undef FIR_SIZE
#undef FIR_MASK

//----------

#endif // 0

//----------------------------------------------------------------------
//----------------------------------------------------------------------


//----------------------------------------------------------------------
#endif

