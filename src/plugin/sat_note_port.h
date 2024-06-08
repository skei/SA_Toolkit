#ifndef sat_note_port_included
#define sat_note_port_included
//----------------------------------------------------------------------

#include "sat.h"

class SAT_NotePort;
typedef SAT_Array<SAT_NotePort*> SAT_NotePortArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_NotePort {

//------------------------------
private:
//------------------------------

  clap_note_port_info_t  MInfo = {
  #ifdef SAT_WIN32
    0,
    CLAP_NOTE_DIALECT_CLAP,
    CLAP_NOTE_DIALECT_CLAP,
    ""
  #else
    .id                 = 0,
    .supported_dialects = CLAP_NOTE_DIALECT_CLAP,
    .preferred_dialect  = CLAP_NOTE_DIALECT_CLAP,
    .name               = ""
  #endif
  };

//------------------------------
public:
//------------------------------

  SAT_NotePort() {
  }

  //----------

  SAT_NotePort(const char* AName, uint32_t AId) {
    MInfo.id                  = AId;
    MInfo.supported_dialects  = CLAP_NOTE_DIALECT_CLAP;
    MInfo.preferred_dialect   = CLAP_NOTE_DIALECT_CLAP;
    SAT_Strlcpy(MInfo.name,AName,CLAP_NAME_SIZE);
  }

//------------------------------
public:
//------------------------------

  clap_note_port_info_t* getInfo() {
    return &MInfo;
  }

};

//----------------------------------------------------------------------
#endif
