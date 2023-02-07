#ifndef sat_note_port_included
#define sat_note_port_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/clap/sat_clap.h"

class SAT_NotePort;
typedef SAT_Array<SAT_NotePort*> SAT_NotePortArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_NotePort {

  friend class SAT_Plugin;

//------------------------------
private:
//------------------------------

  clap_note_port_info_t MInfo = {
    /*clap_id  .id                    =*/ 0,
    /*uint32_t .supported_dialects    =*/ CLAP_NOTE_DIALECT_CLAP, // | CLAP_NOTE_DIALECT_MIDI | CLAP_NOTE_DIALECT_MIDI_MPE,
    /*uint32_t .preferred_dialect     =*/ CLAP_NOTE_DIALECT_CLAP,
    /*char     .name[CLAP_NAME_SIZE]  =*/ "Notes"
  };

//------------------------------
public:
//------------------------------

  SAT_NotePort() {
  }

  //----------

  SAT_NotePort(clap_id id, uint32_t supported_dialects, uint32_t preferred_dialect, const char* name) {
    MInfo.id            = id;
    MInfo.supported_dialects  = supported_dialects;
    MInfo.preferred_dialect   = preferred_dialect;
    strcpy(MInfo.name,name);
  }

  //----------

  virtual ~SAT_NotePort() {
  }

//------------------------------
public:
//------------------------------

  clap_note_port_info_t* getInfo() { return &MInfo; }

};

//----------------------------------------------------------------------
#endif
