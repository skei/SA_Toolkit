#ifndef mip_karplus_strong_included
#define mip_karplus_strong_included
//----------------------------------------------------------------------

#if 0

#include "audio/synthesis/mip_waveguide.h"

//----------

#define kks_off
#define kkm_exciting
#define kkm_resonating
//#define kkm_decaying

//----------

class MIP_KarplusStrong {

  private:

    KWaveGuide  MWaveGuide;
    uint32        MMode;

    float         MDelay;
    float         MFeedback;

  public:

    void size(float ASize) {
    }

    void excite(float AInput) {
      //MExcitation = MWaveGuide.feed(AInput,MFeedback,MDelay);
    }

    void resonate(void) {
    }

    float output(void) {
      return 0;
    }

};

#endif // 0

//----------------------------------------------------------------------
#endif
