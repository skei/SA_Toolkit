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

    SAT_PRINT("host name    : %s\n", getName() );
    SAT_PRINT("host vendor  : %s\n", getVendor() );
    SAT_PRINT("host url     : %s\n", getUrl() );
    SAT_PRINT("host version : %s\n", getVersion() );

  };

  //----------

  virtual ~SAT_Host() {
  }

};

//----------------------------------------------------------------------
#endif
