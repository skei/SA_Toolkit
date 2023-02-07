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

  clap_audio_port_info_t MInfo = {
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

  SAT_AudioPort() {
  }

  //----------

  SAT_AudioPort(clap_id id, const char* name, uint32_t flags, uint32_t channel_count, const char* port_type, clap_id in_place_pair) {
    MInfo.id            = id;
    strcpy(MInfo.name,name);
    MInfo.flags         = flags;
    MInfo.channel_count = channel_count;
    MInfo.port_type     = port_type;
    MInfo.in_place_pair = in_place_pair;
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

//----------------------------------------------------------------------
#endif
