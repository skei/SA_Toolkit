#ifndef sat_parameter_included
#define sat_parameter_included
//----------------------------------------------------------------------

#include "sat.h"

class SAT_Parameter;
typedef SAT_Array<SAT_Parameter*> SAT_ParameterArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Parameter {

//------------------------------
private:
//------------------------------

  clap_param_info_t MInfo   = {};
  double             MValue = 0.0;

//------------------------------
public:
//------------------------------

  SAT_Parameter() {
  }

  SAT_Parameter(const clap_param_info_t* AInfo) {
    memcpy(&MInfo,AInfo,sizeof(clap_param_info_t));
  }

  virtual ~SAT_Parameter() {
  }

//------------------------------
public:
//------------------------------

  clap_param_info_t* getInfo() {
    return &MInfo;
  }

  sat_param_t getValue() {
    return MValue;
  }

};

//----------------------------------------------------------------------
#endif
