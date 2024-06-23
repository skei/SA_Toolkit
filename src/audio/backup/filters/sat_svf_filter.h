#ifndef sat_svf_filter_included
#define sat_svf_filter_included
//----------------------------------------------------------------------

#include "base/utils/sat_math.h"

/*
  http://www.kvraudio.com/forum/viewtopic.php?p=6371172#p6371172
  Take a look at the State Variable Filters described here:
  http://www.cytomic.com/technical-papers
  Those have the same phase/frequency response as Direct Form filters, but
  coefficients are easier to calculate, sweep without side effects and are
  generally much more useful for musical applications.
*/

//----------------------------------------------------------------------

#define SAT_SVF_NONE  0
#define SAT_SVF_LP    1
#define SAT_SVF_HP    2
#define SAT_SVF_BP    3
#define SAT_SVF_N     4

class SAT_SvfFilter {

  private:

    uint32_t  MMode = SAT_SVF_NONE;
    float     MFreq = 1;
    float     MBW   = 0;
    float     z1    = 0;
    float     z2    = 0;

  public:

    SAT_SvfFilter() {
      //MMode = kft_none;
      //MFreq = 1;
      //MBW   = 0;
      reset();
      //z1    = 0;
      //z2    = 0;
    }

  public:

    uint32_t getMode()  { return MMode; }
    uint32_t getFreq()  { return MFreq; }
    uint32_t getBW()    { return MBW; }

  public:

    void reset(void) {
      z1 = 0;
      z2 = 0;
    }

    void setMode(uint32_t AMode) {
      //SAT_Print("%i\n",AMode);
      MMode = AMode;
      //z1 = 0;
      //z2 = 0;
      //reset();
    }

    void setFreq(float AFreq) {
      MFreq = AFreq;
      //z1 = 0;
      //z2 = 0;
    }

    void setBW(float ABW) {
      MBW = ABW;
      //z1 = 0;
      //z2 = 0;
    }

    float process(float AValue) {
      //result := 0;
      if (MMode==SAT_SVF_NONE) return AValue;
      else {
        float L   = z2 + MFreq * z1;
        float H   = AValue - L - MBW   * z1;
        float B   = MFreq * H + z1;
        float N   = H + L;
        z1 = B;
        z2 = L;
        switch(MMode) {
          case SAT_SVF_LP:  return L; //break;
          case SAT_SVF_HP:  return H; //break;
          case SAT_SVF_BP:  return B; //break;
          case SAT_SVF_N:   return N; //break;
        }
      }
      return 0;
    }

    float process(float AValue, uint32_t AType) {
      MMode = AType;
      return process(AValue);
    }

    float processMod(float AValue, float AFreq, float ABW) {
      MFreq += AFreq;
      //MBW   += ABW;
      SAT_Clamp(MFreq,0.0f,1.0f);
      //KClamp(MBW,0,1);
      float result = process(AValue);
      MFreq -= AFreq;
      //MBW   -= ABW;
      return result;
    }

};


//----------------------------------------------------------------------
#endif
