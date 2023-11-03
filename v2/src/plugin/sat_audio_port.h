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
  int32_t                 MIndex = -1;

//------------------------------
public:
//------------------------------

  SAT_AudioPort() {
  }

  //----------

  SAT_AudioPort(const char* name, uint32_t flags, uint32_t channel_count, const char* port_type) {
    MInfo.id            = 0;  // !!!
    SAT_Strlcpy(MInfo.name,name,CLAP_NAME_SIZE);
    MInfo.flags         = flags;
    MInfo.channel_count = channel_count;
    MInfo.port_type     = port_type;
    MInfo.in_place_pair = CLAP_INVALID_ID;
  }

  //----------

  SAT_AudioPort(const clap_audio_port_info_t* AInfo) {
    memcpy(&MInfo,AInfo,sizeof(clap_audio_port_info_t));
  }

  //----------

  virtual ~SAT_AudioPort() {
  }

//------------------------------
public:
//------------------------------

  virtual int32_t getIndex() {
    return MIndex;
  }

  virtual const clap_audio_port_info_t* getInfo() {
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
