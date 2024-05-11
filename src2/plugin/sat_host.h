#ifndef sat_host_included
#define sat_host_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/host/sat_clap_host.h"

class SAT_Host
: public SAT_ClapHost {

//------------------------------
public:
//------------------------------

  SAT_Host(const clap_host_t* AHost)
  : SAT_ClapHost(AHost) {
  };

  //----------

  virtual ~SAT_Host() {
  }

};

//----------------------------------------------------------------------
#endif
