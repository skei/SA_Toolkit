#ifndef sat_curved_envelope_included
#define sat_curved_envelope_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/util/sat_interpolation.h"
#include "base/util/sat_math.h"

//----------------------------------------------------------------------

#define SAT_ENV_OFF       0
#define SAT_ENV_ACTIVE    1
#define SAT_ENV_SUSTAIN   2
#define SAT_ENV_RELEASED  3
#define SAT_ENV_FINISHED  4

#define SAT_ENV_MAX_SEGMENTS 16

//----------

template <typename T>
struct SAT_CurvedEnvelopeSegment {
  uint32_t  type    = 1;    // linear
  T         target  = 0.0;  // 0..1
  T         length  = 0.0;  // ms
  T         curve   = 0.5;  // 0..1, 0.5=linear
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

  SAT_CurvedEnvelopeSegment<T>  MSegments[SAT_ENV_MAX_SEGMENTS] = {0};
  uint32_t                      MNumSegments                    = 0;
  uint32_t                      MState                          = SAT_ENV_OFF;
  T                             MCurrentValue                   = 0.0;
  T                             MSamplesPerSecond               = 0.0;
  T                             MSecondsPerSample               = 0.0;
//T                             MStartValue                     = 0.0;
  uint32_t                      MSustainSegment                 = 0;
  uint32_t                      MCurrentSegment                 = 0;
  uint32_t                      MSegmentType                    = 0;    // 1=linear
  T                             MTimeInSegment                  = 0.0;  // time (ms) in current segment
  T                             MSegmentStartValue              = 0.0;  // segment start value
  T                             MSegmentDelta                   = 0.0;  // segment delta value
  T                             MSegmentLength                  = 0.0;  // segment duration (ms);

  uint32_t cooldown = 0;

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

  void addSegment(uint32_t AType, T ATarget, T ALength, T ACurve) {
    MSegments[MNumSegments].type    = AType;
    MSegments[MNumSegments].target  = ATarget;
    MSegments[MNumSegments].length  = ALength;
    MSegments[MNumSegments].curve   = ACurve;
    MNumSegments += 1;
  }

  void setSustain(uint32_t AIndex) {
    MSustainSegment = AIndex;
  }

//------------------------------
public:
//------------------------------

  void init(T ASampleRate) {
    MSamplesPerSecond = ASampleRate;
    MSecondsPerSample = 1.0 / MSamplesPerSecond;
  }

  //----------

  void reset() {
    MNumSegments        = 0;
    MState              = SAT_ENV_OFF;
    MCurrentValue       = 0.0;
    //MSamplesPerSecond   = 
    //MSecondsPerSample   = 
    MSustainSegment     = 0;
    MCurrentSegment     = 0;
    MSegmentType        = 0;
    MTimeInSegment      = 0.0;
    MSegmentStartValue  = 0.0;
    MSegmentDelta       = 0.0;
    MSegmentLength      = 0;
  }

  //----------

  void setADSR(sat_param_t AAttack, sat_param_t ADecay, sat_param_t ASustain, sat_param_t ARelease) {
    reset();
    //MStartValue= 0.0;
    addSegment( 1, 1,        AAttack,  0.5 );
    addSegment( 1, ASustain, ADecay,   0.5 );
    addSegment( 1, 0,        ARelease, 0.5 );
    setSustain(1);
  }

  //----------

  T getValue() {
    return MCurrentValue;
  }

  //----------

  uint32_t getState() {
    return MState;
  }

  //----------

  void noteOn(T velocity) {
    //SAT_PRINT;
    if (MNumSegments > 0) {
      MState              = SAT_ENV_ACTIVE;
      start(0,0.0);
      // MCurrentSegment     = 0;
      // MSegmentType        = MSegments[0].type;
      // MTimeInSegment      = 0.0;
      // MSegmentStartValue  = 0.0;
      // MSegmentDelta       = MSegments[0].target * MSegments[0].length;
      // MSegmentLength      = MSegments[0].length;
      // MCurrentValue       = 0.0;
    }
  }

  //----------

  void noteOff(T velocity) {
    //if (MCurrentSegment < MSustainSegment) {
      MState = SAT_ENV_RELEASED;
      //nextSegment();
      release();
    //}
  }

  //----------

  T processSample() {
    if (MState == SAT_ENV_OFF)      return 0.0;
    if (MState == SAT_ENV_FINISHED) return 0.0;
    if (MState == SAT_ENV_SUSTAIN)  return MCurrentValue;
    MCurrentValue = calc(MSegmentType,MTimeInSegment,MSegmentStartValue,MSegmentDelta,MSegmentLength);
    //if (cooldown == 0) {
    //  SAT_Print("segment %i time %.2f value %.2f\n",MCurrentSegment,MTimeInSegment,MCurrentValue);
    //  cooldown = 1000;
    //}
    //if (cooldown > 0) cooldown -= 1;
    MTimeInSegment += MSecondsPerSample;
    if (MTimeInSegment >= MSegmentLength) {
      if ((MCurrentSegment) == MSustainSegment) {
        MState = SAT_ENV_SUSTAIN;
      }
      else {
        next();
      }
    }

    return MCurrentValue;

  }

//------------------------------
private:
//------------------------------

  T calc(uint32_t AType, T ATime, T AStartValue, T ADeltaValue, T ALength) {
    T t = ATime / MSegmentLength; // t = 0..1
    T value = SAT_Easing(AType,t,AStartValue,ADeltaValue,ALength);
    T curve = MSegments[MCurrentSegment].curve;
    value = SAT_Curve(value,curve);
    return value;
  }

  //----------

  void start(uint32_t AIndex, T AStartValue) {
    MCurrentSegment     = AIndex;
    MSegmentType        = MSegments[AIndex].type;
    MTimeInSegment      = 0.0;
    MSegmentStartValue  = AStartValue;
    MSegmentDelta       = MSegments[AIndex].target * MSegments[0].length;
    MSegmentLength      = MSegments[AIndex].length;
    MCurrentValue       = 0.0;
  }

  //----------

  void next() {
    T length = 0.0;
    while (length <= 0) {
      MCurrentSegment += 1;
      if (MCurrentSegment >= MNumSegments) {
        MState = SAT_ENV_FINISHED;
        MCurrentValue = 0.0;
      }
      else {
        length              = MSegments[MCurrentSegment].length;
        if (length > 0) {
          MSegmentType        = MSegments[MCurrentSegment].type;
          //MTimeInSegment      = 0.0 + (MTimeInSegment - MSegmentLength);
          MTimeInSegment      = 0.0;
          MSegmentStartValue  = MSegments[MCurrentSegment-1].target;
          MSegmentDelta       = (MSegments[MCurrentSegment].target - MCurrentValue) * length;
          MSegmentLength      = MSegments[MCurrentSegment].length;
        }
      }
    } // end
  }

  //----------

  void release() {
    MCurrentSegment     = MSustainSegment + 1;
    MSegmentType        = MSegments[MCurrentSegment].type;
    MTimeInSegment      = 0.0;
    MSegmentStartValue  = MCurrentValue;//MSegments[MCurrentSegment].target;
    MSegmentDelta       = (MSegments[MCurrentSegment].target - MCurrentValue) * MSegments[MCurrentSegment].length;
    MSegmentLength      = MSegments[MCurrentSegment].length;
  }

};

//----------------------------------------------------------------------
#endif
