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

  SAT_IntParameter(const char* AName, sat_param_t AValue=0.0, sat_param_t AMinValue=0.0, sat_param_t AMaxValue=1.0, uint32_t AFlags=0) {
  : SAT_Parameter(AName,AValue,AMinValue,AMaxValue,AFlags) {

    setValueFormat("%i");

  }

//------------------------------
public:
//------------------------------

  //virtual sat_param_t getModulation() {
  //  return MModulation;
  //}

  virtual sat_param_t getValue() {
    return (int)MValue;
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

  //const char* valueToText(sat_param_t AValue) override {
  //  sprintf(MValueText,MValueFormat,getValue());
  //  return MValueText;
  //}

  sat_param_t textToValue(const char* AText) override {
    return atoi(AText);
  }

};

//----------------------------------------------------------------------
#endif
