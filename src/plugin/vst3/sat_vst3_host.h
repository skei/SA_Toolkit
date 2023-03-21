#ifndef sat_vst3_host_included
#define sat_vst3_host_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/sat_host_implementation.h"

class SAT_Vst3Host
: public SAT_Host {
  
public:

  SAT_Vst3Host(const clap_host_t* host)
  : SAT_Host(host) {
  }

  virtual ~SAT_Vst3Host() {
  }

public:

  const clap_host_t* getHost() { return nullptr; }
  
};

//----------------------------------------------------------------------
#endif
