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

  clap_note_port_info_t MInfo   = {};
  int32_t               MIndex  = -1;


//------------------------------
public:
//------------------------------


  //----------

  SAT_NotePort() {
  }

  //----------

  SAT_NotePort(const clap_note_port_info_t* AInfo) {
    memcpy(&MInfo,AInfo,sizeof(clap_note_port_info_t));
  }

  //----------

  SAT_NotePort(const char* name, uint32_t supported_dialects, uint32_t preferred_dialects) {
    MInfo.id                  = 0; // !!!
    MInfo.supported_dialects  = supported_dialects;
    MInfo.preferred_dialect   = preferred_dialects;
    SAT_Strlcpy(MInfo.name,name,CLAP_NAME_SIZE);
  }

  //----------

  virtual ~SAT_NotePort() {
  }

//------------------------------
public:
//------------------------------

  virtual int32_t getIndex() {
    return MIndex;
  }

  virtual const clap_note_port_info_t* getInfo() {
    return &MInfo;
  }

//------------------------------
public:
//------------------------------

  virtual void setIndex(int32_t AIndex) {
    MIndex = AIndex;
  }

};

//----------------------------------------------------------------------
#endif
