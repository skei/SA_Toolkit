#ifndef sat_parameter_included
#define sat_parameter_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/utils/sat_strutils.h"

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

  clap_param_info_t MInfo                           = {};
  int32_t           MIndex                          = -1;
  sat_param_t       MValue                          = 0.0;
  sat_param_t       MModulation                     = 0.0;

  uint32_t          MLastPainted                    = 0;
  char              MValueText[SAT_MAX_NAME_LENGTH] = {0};
  const char*       MValueFormat                    = "%.2f";


//------------------------------
public:
//------------------------------

  SAT_Parameter(const char* AName, sat_param_t AValue=0.0, sat_param_t AMinValue=0.0, sat_param_t AMaxValue=1.0, uint32_t AFlags=0) {
    MInfo.id                  = 0;      // !!!
    MInfo.flags               = AFlags;
    MInfo.cookie              = this;
    SAT_Strlcpy(MInfo.name,AName,CLAP_NAME_SIZE);
    SAT_Strlcpy(MInfo.module,"",CLAP_PATH_SIZE);
    MInfo.min_value           = AMinValue;
    MInfo.max_value           = AMaxValue;
    MInfo.default_value       = AValue;
    setValue(MValue);
  }

  //----------

  SAT_Parameter(const clap_param_info_t* AInfo) {
    memcpy(&MInfo,AInfo,sizeof(clap_param_info_t));
    setValue(AInfo->default_value);
  }

  //----------

  virtual ~SAT_Parameter() {
  }

//------------------------------
public:
//------------------------------

  virtual int32_t getIndex() {
    return MIndex;
  }

  virtual clap_param_info_t* getInfo() {
    return &MInfo;
  }

  virtual sat_param_t getModulation() {
    return MModulation;
  }

  virtual sat_param_t getValue() {
    return MValue;
  }

//------------------------------
public:
//------------------------------

  virtual void setIndex(int32_t AIndex) {
    MIndex    = AIndex;
    MInfo.id  = AIndex;   // !!!!!
  }

  virtual void setModulation(sat_param_t AValue) {
    MModulation = AValue;
  }

  virtual void setValue(sat_param_t AValue) {
    MValue = AValue;
  }

  virtual void setValueFormat(const char* AFormat) {
    MValueFormat = AFormat;
  }

//------------------------------
public:
//------------------------------

  virtual sat_param_t normalize(sat_param_t AValue) {
    sat_param_t range = MInfo.max_value - MInfo.min_value;
    if (range > 0.0) {
      return (AValue - MInfo.min_value) / range;
    }
    return 0.0;
  }

  virtual sat_param_t denormalize(sat_param_t AValue) {
    sat_param_t range = MInfo.max_value - MInfo.min_value;
    return MInfo.min_value + (AValue * range);
  }

  virtual const char* valueToText(sat_param_t AValue) {
    sprintf(MValueText,MValueFormat,getValue());
    return MValueText;
  }

  virtual sat_param_t textToValue(const char* AText) {
    return atof(AText);
  }

};

//----------------------------------------------------------------------
#endif
