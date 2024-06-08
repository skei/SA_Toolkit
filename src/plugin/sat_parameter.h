#ifndef sat_parameter_included
#define sat_parameter_included
//----------------------------------------------------------------------

#include "sat.h"

class SAT_Parameter;
typedef SAT_Array<SAT_Parameter*> SAT_ParameterArray;

#define SAT_PARAMETER_NUM_VALUES 16

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Parameter {

//------------------------------
private:
//------------------------------

  // CLAP_PARAM_IS_STEPPED
  // CLAP_PARAM_IS_PERIODIC
  // CLAP_PARAM_IS_HIDDEN
  // CLAP_PARAM_IS_READONLY
  // CLAP_PARAM_IS_BYPASS
  // CLAP_PARAM_IS_AUTOMATABLE
  // CLAP_PARAM_IS_AUTOMATABLE_PER_NOTE_ID
  // CLAP_PARAM_IS_AUTOMATABLE_PER_KEY
  // CLAP_PARAM_IS_AUTOMATABLE_PER_CHANNEL
  // CLAP_PARAM_IS_AUTOMATABLE_PER_PORT
  // CLAP_PARAM_IS_MODULATABLE
  // CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID
  // CLAP_PARAM_IS_MODULATABLE_PER_KEY
  // CLAP_PARAM_IS_MODULATABLE_PER_CHANNEL
  // CLAP_PARAM_IS_MODULATABLE_PER_PORT
  // CLAP_PARAM_REQUIRES_PROCESS
  // CLAP_PARAM_IS_ENUM

  clap_param_info_t MInfo = {
  #ifdef SAT_WIN32
                      0,
    CLAP_PARAM_IS_AUTOMATABLE,
    this,
    "param",
    "default",
    0.0,
    1.0,
    0.0
  #else
    .id             = 0,
    .flags          = CLAP_PARAM_IS_AUTOMATABLE,
    .cookie         = this,
    .name           = "param",
    .module         = "default",
    .min_value      = 0.0,
    .max_value      = 1.0,
    .default_value  = 0.0
  #endif
  };

  uint32_t    MIndex                              = 0;    // index into parent's list of parameters
  void*       MWidget                             = nullptr;

  char        MValueText[SAT_MAX_NAME_LENGTH]     = {0};
  sat_param_t MValue                              = {0};
  sat_param_t MModulation                         = 0.0;

  bool        MIndicateMapped                     = false;
  SAT_Color   MIndicateMappedColor                = SAT_Black;
  uint32_t    MIndicateAutomationState            = 0;
  SAT_Color   MIndicateAutomationColor            = SAT_White;

//bool        MGuiAutomationDirty                 = false;
//bool        MGuiModulationDirty                 = false;
//sat_param_t MLastAutomatedValue                 = 0.0;
//sat_param_t MLastModulatedValue                 = 0.0;

//------------------------------
public:
//------------------------------

  SAT_Parameter() {
  }

  //----------

  SAT_Parameter(clap_param_info_t* AInfo) {
    memcpy(&MInfo,AInfo,sizeof(clap_param_info_t));
    MInfo.cookie = this;
    setValue(AInfo->default_value);
  }

  //----------

  SAT_Parameter(/*uint32_t AId=0,*/ const char* AName, const char* AModule="", sat_param_t AValue=0.0, sat_param_t AMinValue=0.0, sat_param_t AMaxValue=1.0, uint32_t AFlags=CLAP_PARAM_IS_AUTOMATABLE) {
    MInfo.id                  = 0;//AId;
    MInfo.flags               = AFlags | CLAP_PARAM_IS_MODULATABLE;
    MInfo.cookie              = this;
    SAT_Strlcpy(MInfo.name,AName,CLAP_NAME_SIZE);
    SAT_Strlcpy(MInfo.module,AModule,CLAP_PATH_SIZE);
    MInfo.min_value           = AMinValue;
    MInfo.max_value           = AMaxValue;
    MInfo.default_value       = AValue;
    // setValue(MValue);
  }

  //----------

  virtual ~SAT_Parameter() {
  }

//------------------------------
public:
//------------------------------

  virtual void setIndex(uint32_t AIndex) {
    MIndex = AIndex;
    MInfo.id = AIndex;
  }
  
  virtual void setWidget(void* AWidget) {
    MWidget = AWidget;
  }

  virtual void setValue(double AValue) {
    MValue = AValue;
  }

  virtual void setNormalizedValue(double AValue) {
    MValue = denormalize(AValue);
  }

  virtual void setModulation(double AValue) {
    MModulation = AValue;
  }

  virtual void setNormalizedModulation(double AValue) {
    MModulation = denormalize(AValue);
  }

  virtual void setIndicateAutomationState(uint32_t AState) {
    MIndicateAutomationState = AState;
  }

  virtual void setIndicateAutomationColor(SAT_Color AColor) {
    MIndicateAutomationColor = AColor;
  }

  virtual void setIndicateMapped(bool AMapped) {
    MIndicateMapped = AMapped;
  }

  virtual void setIndicateMappedColor(SAT_Color AColor) {
    MIndicateMappedColor = AColor;
  }
  
  //virtual void setGuiAutomationDirty(bool ADirty) { MGuiAutomationDirty = ADirty; }
  //virtual void setGuiModulationDirty(bool ADirty) { MGuiModulationDirty = ADirty; }
  //virtual void setLastAutomatedValue(double AValue) { MLastAutomatedValue = AValue; }
  //virtual void setLastModulatedValue(double AValue) { MLastModulatedValue = AValue; }

  virtual void setId(uint32_t AId) {
    MInfo.id = AId;
  }

  virtual void setCookie(void* ACookie) {
    MInfo.cookie = ACookie;
  }

  virtual void setFlag(uint32_t AFlag) {
    MInfo.flags |= AFlag;
  }

  virtual void clearFlag(uint32_t AFlag) {
    MInfo.flags &= ~AFlag;
  }

  virtual void setName(const char* AName) {
    SAT_Strlcpy(MInfo.name,AName,CLAP_NAME_SIZE);
  }

  virtual void setModule(const char* AModule) {
    SAT_Strlcpy(MInfo.module,AModule,CLAP_PATH_SIZE);
  }

  virtual void setMinValue(double AValue) {
    MInfo.min_value = AValue;
  }

  virtual void setMaxValue(double AValue) {
    MInfo.max_value = AValue;
  }

  virtual void setDefaultValue(double AValue) {
    MInfo.default_value = AValue;
  }

//------------------------------
public:
//------------------------------

  virtual const char* getName()   { return MInfo.name; }
  virtual const char* getModule() { return MInfo.module; }
  virtual uint32_t    getId()     { return MInfo.id; }
  virtual uint32_t    getFlags()  { return MInfo.flags; }
  //virtual const char* getCookie() { return MInfo.cookie; }

  virtual uint32_t getIndicateAutomationState() {
    return MIndicateAutomationState;
  }

  virtual SAT_Color getIndicateAutomationColor() {
    return MIndicateAutomationColor;
  }

  virtual bool getIndicateMapped() {
    return MIndicateMapped;
  }

  virtual SAT_Color getIndicateMappedColor() {
    return MIndicateMappedColor;
  }

  virtual clap_param_info_t* getInfo() {
    return &MInfo;
  }

  virtual void* getCookie() {
    return MInfo.cookie;
  }

  virtual uint32_t getIndex() {
    return MIndex;
  }

  virtual void* getWidget() {
    return MWidget;
  }

  virtual sat_param_t getValue() {
    return MValue;
  }

  virtual sat_param_t getNormalizedValue() {
    return normalize(MValue);
  }

  virtual sat_param_t getModulation() {
    return MModulation;
  }

  virtual sat_param_t getNormalizedModulation() {
    return normalize(MModulation);
    //return normalize(MValue + MModulation);
  }

  virtual sat_param_t getMinValue() {
    return MInfo.min_value;
  }

  virtual sat_param_t getMaxValue() {
    return MInfo.max_value;
  }

  virtual sat_param_t getDefaultValue() {
    return MInfo.default_value;
  }

  virtual sat_param_t getValueRange() {
    return (MInfo.max_value - MInfo.min_value);
  }

  virtual bool isStepped() {
    return (MInfo.flags & CLAP_PARAM_IS_STEPPED);
  }

  virtual bool isPeriodic() {
    return (MInfo.flags & CLAP_PARAM_IS_PERIODIC);
  }

  virtual bool isHidden() {
    return (MInfo.flags & CLAP_PARAM_IS_HIDDEN);
  }

  virtual bool isReadOnly() {
    return (MInfo.flags & CLAP_PARAM_IS_READONLY);
  }

  virtual bool isBypass() {
    return (MInfo.flags & CLAP_PARAM_IS_BYPASS);
  }

  virtual bool isEnum() {
    return (MInfo.flags & CLAP_PARAM_IS_ENUM);
  }

  //virtual double getLastAutomatedValue() { return MLastAutomatedValue; }
  //virtual double getLastModulatedValue() { return MLastModulatedValue; }
  //virtual bool isGuiAutomationDirty() { return MGuiAutomationDirty; }
  //virtual bool isGuiModulationDirty() { return MGuiModulationDirty; }

//------------------------------
public:
//------------------------------

  virtual const char* valueToText(sat_param_t AValue) {
    sprintf(MValueText,"%.2f",AValue);
    return MValueText;
  }

  //----------

  virtual sat_param_t textToValue(const char* AText) {
    sat_param_t value = atof(AText);
    return value;
  }

  //----------

  // virtual sat_param_t getRange() {
  //   sat_param_t value = (MInfo.max_value - MInfo.min_value);
  //   return value;
  // }

  //----------

  virtual sat_param_t normalize(sat_param_t AValue) {
    //return AValue;
    double range = (MInfo.max_value - MInfo.min_value);
    if (range > 0.0) {
      return (AValue - MInfo.min_value) / range;
    }
    return 0.0;
  }

  //----------

  virtual sat_param_t denormalize(sat_param_t AValue) {
    //return AValue;
    double range = (MInfo.max_value - MInfo.min_value);
    if (range > 0.0) {
      return MInfo.min_value + (AValue * range);
    }
    return 0.0;
  }

};

//----------------------------------------------------------------------
#endif
