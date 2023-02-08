#ifndef sat_voice_manager_included
#define sat_voice_manager_included
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
class SAT_VoiceManager {

//------------------------------
private:
//------------------------------

  SAT_Voice<VOICE>  MVoices = {};

//------------------------------
public
//------------------------------

  SAT_VoiceManager() {
  }

  //----------

  ~SAT_VoiceManager() {
  }

};

//----------------------------------------------------------------------
#endif

