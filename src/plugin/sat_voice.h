#ifndef sat_voice_included
#define sat_voice_included
//----------------------------------------------------------------------

template <class VOICE>
class SAT_Voice {

//------------------------------
private:
//------------------------------

  VOICE     MVoice        = {};
  uint32_t  MNoteOnTime   = 0;
  uint32_t  MNoteOffTime  = 0;

//------------------------------
public
//------------------------------

  SAT_Voice() {
  }

  //----------

  ~SAT_Voice() {
  }


};

//----------------------------------------------------------------------
#endif


