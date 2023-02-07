#ifndef sat_audio_port_included
#define sat_audio_port_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/clap/sat_clap.h"

class SAT_AudioPort;
typedef SAT_Array<SAT_AudioPort*> SAT_AudioPortArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_AudioPort {

  friend class SAT_Plugin;

//------------------------------
private:
//------------------------------

  clap_audio_port_info_t MAudioPortInfo = {
    /*clap_id     id                    =*/ 0,
    /*char        name[CLAP_NAME_SIZE]  =*/ "Audio",
    /*uint32_t    flags                 =*/ CLAP_AUDIO_PORT_IS_MAIN,
    /*uint32_t    channel_count         =*/ 2,
    /*const char* port_type             =*/ CLAP_PORT_STEREO,
    /*clap_id     in_place_pair         =*/ CLAP_INVALID_ID
  };

//------------------------------
public:
//------------------------------

  clap_audio_port_info_t* getInfo() {
    return &MAudioPortInfo;
  }

};

//----------------------------------------------------------------------
#endif
