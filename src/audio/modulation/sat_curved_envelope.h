#ifndef sat_curved_envelope_included
#define sat_curved_envelope_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/utils/sat_interpolation.h"
#include "base/utils/sat_math.h"

//----------------------------------------------------------------------

template <typename T>
struct SAT_CurvedEnvelopeSegment {
  T target;
  T length;
  T curve;
  //
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <typename T>
class SAT_CurvedEnvelope {

//------------------------------
private:
//------------------------------

  SAT_CurvedEnvelopeSegment<T>* MSegments             = nullptr;
  uint32_t                      MNumSegments          = 0;
  bool                          MRetrigger            = true;
  uint32_t                      NSustain              = 0;

//T                             MCurrentSegment       = 0.0;
//T                             MCurrentTimeInSegment = 0.0;
//T                             MCurrentValue         = 0.0;
//T                             MSlope                = 0.0;

//------------------------------
public:
//------------------------------

  SAT_CurvedEnvelope() {
  }

  //----------

  virtual ~SAT_CurvedEnvelope() {
  }

//------------------------------
public:
//------------------------------

  void reset() {
    // MCurrentSegment = 0;
    // MCurrentTimeInSegment = 0.0;
    // MCurrentValue = 0.0;
  }

  T getValue() {
    return 0.0;
  }

  void note_on(T velocity) {
    // MCurrentSegment       = 0.0;
    // MCurrentTimeInSegment = 0.0;
    // triggerSegment(0,MRetrigger,MCurrentValue);
  }

  void note_off(T velocity) {
  }

  T process(T ALength) {
    // MCurrentTimeInSegment += ALength;
    // T over = 0.0;
    // bool finished = false;
    // while (!finished)
    //   T current_segment_length = MSegments[MCurrentSegment]->length;
    //   over = MCurrentTimeInSegment - current_segment_length;
    //   if (over >= 0.0) {
    //     MCurrentSegment += 1;
    //     MCurrentTimeInSegment = over;
    //     triggerSegment(MCurrentSegment);
    //   }
    //   else {
    //     finished =true;
    //   }
    // }
    return 0.0;
  }

  T processSample() {
    return 0.0;
  }

  T processSamples(uint32_t ANumSamples) {
    return 0.0;
  }

//------------------------------
private:
//------------------------------

  // ugly and brute-force..
  // todo: cache current_segment, _time, _value, etc.. and iterate..

  T calcValue(T ATime, T AStartVal=0.0) {
    if (MNumSegments < 2) return -1; // no segments
    if (MNumSegments == 1) return 0; // first segment
    T current_time = 0.0;
    T current_value = AStartVal;
    for (uint32_t i=0; i<MNumSegments-1; i++) {
      T next_time = current_time + MSegments[i]->length;
      T next_value = MSegments[i]->target;
      if (ATime < next_time) {
        T current_pos = ATime - current_time;
        T range = next_time - current_time;
        T phase = current_pos / range;
        T current_value = SAT_Interpolate_Linear(phase,current_value,next_value);
        current_value = SAT_Curve(current_value,MSegments[i]->curve);
        return i;
      }
      current_time = next_time;
      current_value = next_value;
    }
    return -2; // finished
  }


};

//----------------------------------------------------------------------
#endif
