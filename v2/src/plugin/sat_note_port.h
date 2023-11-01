#ifndef sat_note_port_included
#define sat_note_port_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"

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

  clap_note_port_info_t  MInfo = {};

//------------------------------
public:
//------------------------------

  SAT_NotePort() {
  }

  //----------

  SAT_NotePort(clap_note_port_info_t* AInfo) {
    memcpy(&MInfo,AInfo,sizeof(clap_note_port_info_t));
  }

  //----------

  virtual ~SAT_NotePort() {
  }

//------------------------------
public:
//------------------------------

  clap_note_port_info_t* getInfo() {
    return &MInfo;
  }



};

typedef SAT_Array<SAT_AudioPort*> SAT_AudioPorts;

//----------------------------------------------------------------------
#endif
