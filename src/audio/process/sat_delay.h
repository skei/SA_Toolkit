#ifndef sat_delay_included
#define sat_delay_included
//----------------------------------------------------------------------

#include "base/util/sat_math.h"
#include "audio/sat_audio_math.h"
//#include "audio/filters/sat_dc_filter.h"

//----------------------------------------------------------------------

template<typename T>
struct SAT_NoLoopFx {
  T process(T x) { return x; }
};

//----------

template<typename T>
struct SAT_NoClipFx {
  T process(T x) { return x; }
};

// template<typename T>
// struct SAT_HardClipFx {
//   T process(T x) { return x;/*SAT_Clamp(x,-1,1);*/ }
// };

//----------------------------------------------------------------------
//
// interpolated delay
//
//----------------------------------------------------------------------


template <typename T, int MAX_DELAY, typename LOOPFX=SAT_NoLoopFx<T>, typename CLIPFX=SAT_NoClipFx<T>>
class SAT_InterpolatedDelay {

//------------------------------
private:
//------------------------------

  T               MBuffer[MAX_DELAY]  = {0};
  int             MCounter            = 0;
  T               MDelayPos           = 0.0f;
  bool            MWrapped            = false;
  LOOPFX          MLoopFX             = {};
  CLIPFX          MClipFX             = {};
  T               MPhase              = 0.0;

  //SAT_DcFilter  MDC;

//------------------------------
public:
//------------------------------

  SAT_InterpolatedDelay() {
    clear();
  }

  //----------

  ~SAT_InterpolatedDelay() {
  }

//------------------------------
public:
//------------------------------

  LOOPFX* getLoopFX() {
    return &MLoopFX;
  }

  //----------

  LOOPFX* getClipFX() {
    return &MLoopFX;
  }

  //----------

  bool hasWrapped() {
    return MWrapped;
  }

  //----------

  T getPhase() {
    return MPhase;
  }

  //----------

  void reset() {
    MCounter = 0;
    MWrapped = false;
  }

  //----------

  void clear() {
    MCounter = 0;
    memset(MBuffer,0,MAX_DELAY*sizeof(T));
  }

  //----------

  void start() {
    MWrapped = false;
    /*MPhase*/MDelayPos = 0.0f;
  }

  //----------

  //void restart() {
  //  MCounter = 0;
  //  MWrapped = false;
  //  ///*MPhase*/MDelayPos = 0.0f;
  //}

  //----------

  T process(T AInput, T ADelay, T AFeedback) {

    SAT_Assert( ADelay > 0 );
    SAT_Assert( ADelay < MAX_DELAY );

    // calculate delay offset
    T back = (T)MCounter - ADelay;
    if (back < 0.0) back += MAX_DELAY;            // + back;
    int index0 = (int)back;
    int index_1 = index0-1;
    int index1 = index0+1;
    int index2 = index0+2;
    if (index_1 < 0) index_1 += MAX_DELAY;        // index_1 = MAX_DELAY - 1;
    if (index1 >= MAX_DELAY) index1 -= MAX_DELAY; // index1 = 0;
    if (index2 >= MAX_DELAY) index2 -= MAX_DELAY; // index2 = 0;

    // interpolate
    T y_1 = MBuffer[index_1];
    T y0  = MBuffer[index0];
    T y1  = MBuffer[index1];
    T y2  = MBuffer[index2];
    T x   = (T)back - (T)index0;
    T c0  = y0;
    T c1  = 0.5f * (y1 - y_1);
    T c2  = y_1 - 2.5f * y0 + 2.0f * y1 - 0.5f * y2;
    T c3  = 0.5f * (y2 - y_1) + 1.5f * (y0 - y1);
    T delayed = ((c3 * x + c2) * x + c1) * x + c0;

    //output = MDC.process(output);
    //output = SAT_KillDenormal(output);

    //T flt = fb;
    //T out = filtered_feedback;// + (AInput * AMix);
    //out = atan(out); // KClamp((AInput + flt), -1, 1);
    // hard-clip
    //if (out >  1.0f) out =  1.0f;
    //if (out < -1.0f) out = -1.0f;

    T feedback = delayed * AFeedback;
    
//    feedback = MLoopFX.process(feedback);
//    feedback = MClipFX.process(feedback);

    //-----

    SAT_Assert( MCounter >= 0 );
    SAT_Assert( MCounter < MAX_DELAY );

    // if only part of next sample 'fits' inside delay length...
    //if ((MDelayPos + 1.0) >= ADelay) {
    //  T diff = ADelay - MDelayPos;
    //  out *= diff;
    //}

    MBuffer[MCounter] = AInput + feedback;

    MCounter++;
    if (MCounter >= MAX_DELAY) {
      MCounter -= MAX_DELAY;// 0;
    }

    MDelayPos += 1.0f;
    if (MDelayPos >= ADelay) {
      MWrapped = true;
      while (MDelayPos >= ADelay) MDelayPos -= ADelay;
    }

    MPhase = MDelayPos / ADelay;

    return delayed;
  }

};

//----------------------------------------------------------------------
//
// simple delay
//
//----------------------------------------------------------------------

//template <int MAX_DELAY, typename FBLOOPFX=SAT_NoDelayFx>
//class KSimpleDelay {
//
//  private:
//
//    float       MBuffer[MAX_DELAY];
//    int32       MCounter;
//    FBLOOPFX    MFBLoopFX;
//
//  public:
//
//    KSimpleDelay() {
//      clear();
//    }
//
//    ~KSimpleDelay() {
//    }
//
//    FBLOOPFX* getFeedbackFX(void) {
//      return &MFBLoopFX;
//    }
//
//    void restart(void) {
//      MCounter = 0;
//    }
//
//    void clear(void) {
//      MCounter = 0;
//      KMemset(MBuffer,0,MAX_DELAY*sizeof(float));
//    }
//
//    float process(float AInput, float AFeedback, int32 ADelay) {
//      int32 back = MCounter - ADelay;
//      if (back < 0) back = MAX_DELAY + back;
//      int index = (int)back;
//      float output = MBuffer[index];
//      float fb = AInput + output * AFeedback;
//      fb = MFBLoopFX.process(fb);
//      MBuffer[MCounter] = fb;
//      MCounter++;
//      if (MCounter >= MAX_DELAY) MCounter = 0;
//      return output;
//    }
//
//};

//----------------------------------------------------------------------
#endif
