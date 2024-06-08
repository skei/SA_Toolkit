#ifndef sat_sinusoid_included
#define sat_sinusoid_included
//----------------------------------------------------------------------

// sinusoid (approximation)
// http://forum.cockos.com/showpost.php?p=348182&postcount=6

#include <math.h> // fabs

//----------

template <typename T>
class SAT_Sinusoid {

//------------------------------
private:
//------------------------------

  T srate = 0.0;

//------------------------------
public:
//------------------------------

  T t   = 0.0;
  T dt  = 0.0;

//------------------------------
public:
//------------------------------

  void setSampleRate(T ASampleRate) {
    srate = ASampleRate;
  }

  //----------

  void setFreq(T AFreq) {
    dt = (AFreq/srate) * 4.0;
  }

  //----------

  void setFreq(T AFreq, T ARate) {
    dt = (AFreq/ARate) * 4.0;
  }

  //----------

  void setPhase(T APhase) {
    t = APhase;
  }

  //----------

  T process(void) {
    t += dt;
    if (t > 2) t -= 4.0;
    return t * (2.0 - abs(t));
  }

};

//----------------------------------------------------------------------
#endif
