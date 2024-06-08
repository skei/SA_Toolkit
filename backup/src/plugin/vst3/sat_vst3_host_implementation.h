#ifndef sat_vst3_host_implementation_included
#define sat_vst3_host_implementation_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/sat_host_implementation.h"

class SAT_Vst3HostImplementation
: public SAT_HostImplementation {
  
public:

  SAT_Vst3HostImplementation()
  : SAT_HostImplementation() {
  }

  virtual ~SAT_Vst3HostImplementation() {
  }

public:

  //const clap_host_t* getHost() { return nullptr; }
  
};

//----------------------------------------------------------------------
#endif
