#ifndef sat_curved_envelope_included
#define sat_curved_envelope_included
//----------------------------------------------------------------------

#include "sat.h"

//----------------------------------------------------------------------

template <typename T>
struct SAT_CurvedEnvelopeSegment {
  sat_param_t target;
  sat_param_t length;
  sat_param_t curve;
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

  SAT_CurvedEnvelopeSegment<T>* segments    = nullptr;
  uint32_t                      numSegments = 0;
  bool                          retrigger   = true;
  uint32_t                      sustain     = 0;

  T current_value = 0.0;

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
  }

  void note_on(T velocity) {
  }

  void note_off(T velocity) {
  }

  T process(T timestep) {
    return current_value;
  }

};

//----------------------------------------------------------------------
#endif
