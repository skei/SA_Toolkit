#ifndef sat_voice_processor_included
#define sat_voice_processor_included
//----------------------------------------------------------------------

/*
  keep track of 'age' of voices..
  - number of process frames since note on
  - count samples, or process blocks

*/

#include "plugin/sat_voice.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class VOICE, int NUM_VOICES>
class SAT_VoiceProcessor {

//------------------------------
private:
//------------------------------

  SAT_Voice<VOICE>  MVoices = {};

//------------------------------
public
//------------------------------

  SAT_VoiceProcessor() {
  }

  //----------

  ~SAT_VoiceProcessor() {
  }

};

//----------------------------------------------------------------------
#endif

