#ifndef sat_audio_port_included
#define sat_audio_port_included
//----------------------------------------------------------------------

#include "sat.h"

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

  clap_audio_port_info_t  MInfo = {
    .id             = 0,
    .name           = "",
    .flags          = CLAP_AUDIO_PORT_IS_MAIN,
    .channel_count  = 2,
    .port_type      = CLAP_PORT_STEREO,
    .in_place_pair  = CLAP_INVALID_ID
  };

//------------------------------
public:
//------------------------------

  // CLAP_AUDIO_PORT_IS_MAIN
  // CLAP_AUDIO_PORT_SUPPORTS_64BITS
  // CLAP_AUDIO_PORT_PREFERS_64BITS
  // CLAP_AUDIO_PORT_REQUIRES_COMMON_SAMPLE_SIZE

  //----------

  // SAT_AudioPort() {
  //   SAT_TRACE;
  // }

  //----------

  SAT_AudioPort(const char* AName, uint32_t AId) {
    MInfo.id            = AId;
    SAT_Strlcpy(MInfo.name,AName,CLAP_NAME_SIZE);
    MInfo.flags         = CLAP_AUDIO_PORT_IS_MAIN;
    MInfo.channel_count = 2;
    MInfo.port_type     = CLAP_PORT_STEREO;
    MInfo.in_place_pair = CLAP_INVALID_ID;    
  }

//------------------------------
public:
//------------------------------

  clap_audio_port_info_t* getInfo() {
    // SAT_PRINT("id %i\n",MInfo.id);
    // SAT_PRINT("name %s\n",MInfo.name);
    // SAT_PRINT("flags %i\n",MInfo.flags);
    // SAT_PRINT("channel_count %i\n",MInfo.channel_count);
    // SAT_PRINT("port_type %s\n",MInfo.port_type);
    // SAT_PRINT("in_place_pair %i\n",MInfo.in_place_pair);
    return &MInfo;
  }

};

//----------------------------------------------------------------------
#endif
