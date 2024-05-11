#ifndef sat_host_implementation_included
#define sat_host_implementation_included
//----------------------------------------------------------------------
// the host ptr you give to hosted plugins

#include "sat.h"
#include "plugin/lib/sat_clap.h"
#include "plugin/host/sat_clap_host_implementation.h"
#include "plugin/host/sat_host_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_HostImplementation 
: public SAT_ClapHostImplementation {

//------------------------------
public:
//------------------------------

  SAT_HostImplementation()
  : SAT_ClapHostImplementation() {
  };

  //----------

  virtual ~SAT_HostImplementation() {
  }

};

//----------------------------------------------------------------------
#endif
