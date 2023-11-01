#ifndef sat_audio_port_included
#define sat_audio_port_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"

class SAT_AudioPort;
typedef SAT_Array<SAT_AudioPort*> SAT_AudioPortArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_AudioPort {

//------------------------------
private:
//------------------------------

  clap_audio_port_info_t  MInfo = {};

//------------------------------
public:
//------------------------------

  SAT_AudioPort() {
  }

  //----------

  SAT_AudioPort(clap_audio_port_info_t* AInfo) {
    memcpy(&MInfo,AInfo,sizeof(clap_audio_port_info_t));
  }

  //----------

  virtual ~SAT_AudioPort() {
  }

//------------------------------
public:
//------------------------------

  clap_audio_port_info_t* getInfo() {
    return &MInfo;
  }



};

typedef SAT_Array<SAT_AudioPort*> SAT_AudioPorts;

//----------------------------------------------------------------------
#endif
