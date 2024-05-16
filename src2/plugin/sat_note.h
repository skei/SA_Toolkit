#ifndef sat_note_included
#define sat_note_included
//----------------------------------------------------------------------

#include "sat.h"

// make it 'trivially copyable'

struct SAT_Note {

  union {
    struct {
      int16_t port;       //  = -1;
      int16_t channel;    //  = -1;
      int16_t key;        //  = -1;
      int16_t dummy;      //  = -1;   // clap uses 32 bit note_id's.. ???
    };
    uint64_t  data;       //  = 0;
  };

  //SAT_Note(int16_t n=-1, int16_t k=-1, int16_t c=-1, int16_t p=-1) {
  //  noteid  = n;
  //  key     = k;
  //  channel = c;
  //  port    = p;
  //}

};

//----------------------------------------------------------------------
#endif
