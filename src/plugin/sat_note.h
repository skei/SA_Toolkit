#ifndef sat_note_included
#define sat_note_included
//----------------------------------------------------------------------

#include "base/sat.h"

struct SAT_Note {
  int16_t port    = -1;
  int16_t channel = -1;
  int16_t key     = -1;
  int16_t noteid  = -1;
  SAT_Note(int16_t p=-1, int16_t c=-1, int16_t k=-1, int16_t n=-1) {
    port    = p;
    channel = c;
    key     = k;
    noteid  = n;
  }
};

//----------------------------------------------------------------------
#endif
