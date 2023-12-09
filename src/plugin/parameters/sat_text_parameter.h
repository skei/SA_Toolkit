#ifndef sat_text_parameter_included
#define sat_text_parameter_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/parameters/sat_int_parameter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_TextParameter
: public SAT_IntParameter {

//------------------------------
private:
//------------------------------

  const char** MText = nullptr;

//------------------------------
public:
//------------------------------

  SAT_TextParameter(const char* AName, sat_param_t AValue=0.0, sat_param_t AMin=0.0, sat_param_t AMax=1.0, const char** AText=nullptr, uint32_t AFlags=CLAP_PARAM_IS_AUTOMATABLE)
  : SAT_IntParameter(AName,AValue,AMin,AMax,AFlags) {
    MText = AText;
  }

  SAT_TextParameter(const clap_param_info_t* AInfo, const char** AText=nullptr)
  : SAT_IntParameter(AInfo) {
    MText = AText;
  }



  //----------
  
  // SAT_TextParameter(/*clap_id id,*/ const char* name, const char* module, double defval, double minval, double maxval, clap_param_info_flags flags)
  // : SAT_IntParameter(/*id,*/name,module,defval,minval,maxval,flags) {
  //   // see setText()
  // }
  
  //----------

  virtual ~SAT_TextParameter() {
  }

//------------------------------
public:
//------------------------------

  virtual void setText(const char** AText) {
    MText = AText;
  }

//------------------------------
public:
//------------------------------

  const char* valueToText(sat_param_t value) override {
    uint32_t index = SAT_Trunc(value);
    return MText[index];
  }
  
  // todo: textToValue..
  // type "lowpassp" to get lowpass..
  // (search in string list)

};

//----------------------------------------------------------------------
#endif
