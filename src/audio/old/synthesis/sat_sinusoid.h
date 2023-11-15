#ifndef mip_sinusoid_included
#define mip_sinusoid_included
//----------------------------------------------------------------------

// sinusoid (approximation)
// http://forum.cockos.com/showpost.php?p=348182&postcount=6

#include <math.h> // fabs

//----------

class MIP_Sinusoid {

  private:

    float srate;

  public:
    float t,dt;

  public:

    MIP_Sinusoid() {
      t = 0.0;
      dt = 0.0;
      srate = 0.0;
    }

    void setSampleRate(float ASampleRate) {
      srate = ASampleRate;
    }

    void setFreq(float AFreq) {
      dt = (AFreq/srate) * 4.0;
    }

    void setFreq(float AFreq, float ARate) {
      dt = (AFreq/ARate) * 4.0;
    }

    void setPhase(float APhase) {
      t = APhase;
    }

    float process(void) {
      t += dt;
      if (t > 2) t -= 4.0;
      return t * (2.0 - fabs(t));
    }

};

//----------------------------------------------------------------------
#endif
