#ifndef sat_text_parameter_included
#define sat_text_parameter_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/parameter/sat_int_parameter.h"

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

//------------------------------
public:
//------------------------------

  SAT_TextParameter()
  : SAT_IntParameter() {
    setValueFormat("%.f%");
  }

  //----------

  SAT_TextParameter(clap_param_info_t* AInfo)
  : SAT_IntParameter(AInfo) {
    setValueFormat("%.f%");
  }

  //----------

  SAT_TextParameter(const char* AName, const char* AModule="", sat_param_t AValue=0.0, sat_param_t AMinValue=0.0, sat_param_t AMaxValue=1.0, uint32_t AFlags=CLAP_PARAM_IS_AUTOMATABLE, const char** AText=nullptr)
  : SAT_IntParameter(AName,AModule,AValue,AMinValue,AMaxValue,AFlags) {
    MText = AText;
    setValueFormat("%.f%");
  }

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
