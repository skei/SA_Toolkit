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

//----------------------------------------------------------------------
//
// interpolated delay
//
//----------------------------------------------------------------------


template <typename T, int MAX_DELAY, typename LOOPFX=SAT_NoLoopFx<T>>
class SAT_InterpolatedDelay {

//------------------------------
private:
//------------------------------

  T               MBuffer[MAX_DELAY]  = {0};
  int             MWritePos           = 0;
  LOOPFX          MLoopFX             = {};
  T               MPhase              = 0.0;
  T               MPhaseCounter       = 0.0f;
  bool            MPhaseWrapped       = false;
//SAT_DcFilter    MDcFilter           = {};

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

  bool hasWrapped() {
    return MPhaseWrapped;
  }

  //----------

  T getPhase() {
    return MPhase;
  }

//------------------------------
public:
//------------------------------

  void setSampleRate(double ASampleRate) {
    //MLoopFX.filter.setSampleRate(ASampleRate);
  }


  void reset() {
    MWritePos = 0;
    MPhaseWrapped = false;
  }

  //----------

  void clear() {
    MWritePos = 0;
    memset(MBuffer,0,MAX_DELAY*sizeof(T));
  }

  //----------

  void start() {
    MPhaseWrapped = false;
    MPhaseCounter = 0.0f;
  }

  //----------

  // input = value to write into buffer (at writepos)
  // delay = delay length in samples (read from earlier in buffer)
  // feedback = how much to feed back back to the buffer
  // offset = tap position (offset) 0..1

  T process(T AInput, T ADelay, T AFeedback/*, T ATapOffset=0.0*/) {

    SAT_Assert( ADelay > 0 );
    SAT_Assert( ADelay < MAX_DELAY );

    //T offset = (ATapOffset * ADelay);

    // calculate delay offset
    T back = (T)MWritePos - ADelay;// + offset;
    if (back < 0.0) back += MAX_DELAY;
    if (back >= MAX_DELAY) back -= MAX_DELAY;

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

    T feedback = delayed * AFeedback;
    feedback = MLoopFX.process(feedback);

    // clip?

    SAT_Assert( MWritePos >= 0 );
    SAT_Assert( MWritePos < MAX_DELAY );
    MBuffer[MWritePos] = AInput + feedback;

    MWritePos++;
    if (MWritePos >= MAX_DELAY) {
      MWritePos -= MAX_DELAY;// 0;
    }

    MPhaseCounter += 1.0f;
    if (MPhaseCounter >= ADelay) {
      MPhaseWrapped = true;
      while (MPhaseCounter >= ADelay) MPhaseCounter -= ADelay;
    }

    MPhase = MPhaseCounter / ADelay;

    return delayed;
  }

  //----------

  // T tap(T ADelay, T ATapOffset) {
  //   SAT_Assert( ADelay > 0 );
  //   SAT_Assert( ADelay < MAX_DELAY );
  //   T offset = (ATapOffset * ADelay);
  //   // calculate delay offset
  //   T back = (T)(MWritePos-1) - ADelay + offset;
  //   if (back < 0.0) back += MAX_DELAY;
  //   if (back >= MAX_DELAY) back -= MAX_DELAY;
  //   int index0 = (int)back;
  //   int index_1 = index0-1;
  //   int index1 = index0+1;
  //   int index2 = index0+2;
  //   if (index_1 < 0) index_1 += MAX_DELAY;        // index_1 = MAX_DELAY - 1;
  //   if (index1 >= MAX_DELAY) index1 -= MAX_DELAY; // index1 = 0;
  //   if (index2 >= MAX_DELAY) index2 -= MAX_DELAY; // index2 = 0;
  //   // interpolate
  //   T y_1 = MBuffer[index_1];
  //   T y0  = MBuffer[index0];
  //   T y1  = MBuffer[index1];
  //   T y2  = MBuffer[index2];
  //   T x   = (T)back - (T)index0;
  //   T c0  = y0;
  //   T c1  = 0.5f * (y1 - y_1);
  //   T c2  = y_1 - 2.5f * y0 + 2.0f * y1 - 0.5f * y2;
  //   T c3  = 0.5f * (y2 - y_1) + 1.5f * (y0 - y1);
  //   T delayed = ((c3 * x + c2) * x + c1) * x + c0;
  //   return delayed;
  // }


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
//    int32       MWritePos;
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
//      MWritePos = 0;
//    }
//
//    void clear(void) {
//      MWritePos = 0;
//      KMemset(MBuffer,0,MAX_DELAY*sizeof(float));
//    }
//
//    float process(float AInput, float AFeedback, int32 ADelay) {
//      int32 back = MWritePos - ADelay;
//      if (back < 0) back = MAX_DELAY + back;
//      int index = (int)back;
//      float output = MBuffer[index];
//      float fb = AInput + output * AFeedback;
//      fb = MFBLoopFX.process(fb);
//      MBuffer[MWritePos] = fb;
//      MWritePos++;
//      if (MWritePos >= MAX_DELAY) MWritePos = 0;
//      return output;
//    }
//
//};

//----------------------------------------------------------------------
#endif
