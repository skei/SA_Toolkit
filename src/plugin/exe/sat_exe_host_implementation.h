#ifndef sat_exe_host_implementation_included
#define sat_exe_host_implementation_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/sat_host_implementation.h"

//----------------------------------------------------------------------

class SAT_ExeHostImplementation
: public SAT_HostImplementation {

//------------------------------
public:
//------------------------------

  SAT_ExeHostImplementation(int argc, char** argv)
  : SAT_HostImplementation() {
  }

  //----------

  ~SAT_ExeHostImplementation() {
  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif