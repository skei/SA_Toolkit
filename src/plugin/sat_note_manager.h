#ifndef sat_note_manager_included
#define sat_note_manager_included
//----------------------------------------------------------------------

#include "plugin/sat_note_manager.h"

#define SAT_MAX_NUM_CHANNELS  16
#define SAT_MAX_NUM_KEYS      128

class SAT_NoteManager {

//------------------------------
private:
//------------------------------

  uint32_t MNoteMap[SAT_MAX_NUM_CHANNELS * SAT_MAX_NUM_KEYS] = {0};

//------------------------------
public:
//------------------------------

  void state(int16_t AChannel, int16_t AKey) {
    uint32_t index = (AChannel * SAT_MAX_NUM_CHANNELS) + AKey;
    return MNoteMap[index];
  }

  void noteOn(int16_t AChannel, int16_t AKey) {
    uint32_t index = (AChannel * SAT_MAX_NUM_CHANNELS) + AKey;
    uint32_t prev = MNoteMap[index];
    MNoteMap[index] = 1;
    return prev;
  }

  void noteOff(int16_t AChannel, int16_t AKey) {
    uint32_t index = (AChannel * SAT_MAX_NUM_CHANNELS) + AKey;
    uint32_t prev = MNoteMap[index];
    MNoteMap[index] = 0;
    return prev;
  }

};

//----------------------------------------------------------------------
#endif
