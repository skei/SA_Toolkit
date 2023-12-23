#ifndef sat_int_parameter_included
#define sat_int_parameter_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/sat_parameter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_IntParameter
: public SAT_Parameter {

//------------------------------
public:
//------------------------------

  SAT_IntParameter(const char* AName, sat_param_t AValue=0.0, sat_param_t AMinValue=0.0, sat_param_t AMaxValue=1.0, uint32_t AFlags=CLAP_PARAM_IS_AUTOMATABLE)
  : SAT_Parameter(AName,AValue,AMinValue,AMaxValue,AFlags) {
    setValueFormat("%.f");
  }

  SAT_IntParameter(const clap_param_info_t* AInfo)
  : SAT_Parameter(AInfo) {
    setValueFormat("%.f");
  }

//------------------------------
public:
//------------------------------

  //virtual sat_param_t getModulation() {
  //  return MModulation;
  //}

  virtual sat_param_t getValue() {
    //return (int)MValue;
    sat_param_t value = SAT_Trunc(MValue);
    //SAT_Print("%f\n",value);
    return value;
  }

//------------------------------
public:
//------------------------------

  //virtual void setModulation(sat_param_t AValue) {
  //  MModulation = (int)AValue;
  //}

  //virtual void setValue(sat_param_t AValue) {
  //  MValue = (int)AValue;
  //}

//------------------------------
public:
//------------------------------

  const char* valueToText(sat_param_t AValue) override {
    sprintf(MValueText,MValueFormat,getValue());
    //SAT_Print("AValue %f (fmt %s) -> %s\n",AValue,MValueFormat,MValueText);
    return MValueText;
  }

  sat_param_t textToValue(const char* AText) override {
    return atoi(AText);
  }

};

//----------------------------------------------------------------------
#endif
