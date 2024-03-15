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

/*
  CLAP_PARAM_IS_STEPPED                 // Is this param stepped? (integer values only) if so the double value is converted to integer using a cast (equivalent to trunc).   
  CLAP_PARAM_IS_PERIODIC                // Useful for periodic parameters like a phase
  CLAP_PARAM_IS_HIDDEN                  // The parameter should not be shown to the user, because it is currently not used. It is not necessary to process automation for this parameter.
  CLAP_PARAM_IS_READONLY                // The parameter can't be changed by the host.
  CLAP_PARAM_IS_BYPASS                  // This parameter is used to merge the plugin and host bypass button. It implies that the parameter is stepped. min: 0 -> bypass off, max: 1 -> bypass on
  CLAP_PARAM_IS_AUTOMATABLE             // When set: automation can be recorded, automation can be played back. The host can send live user changes for this parameter regardless of this flag.
  CLAP_PARAM_IS_AUTOMATABLE_PER_NOTE_ID // Does this parameter support per note automations?
  CLAP_PARAM_IS_AUTOMATABLE_PER_KEY     // Does this parameter support per key automations?
  CLAP_PARAM_IS_AUTOMATABLE_PER_CHANNEL // Does this parameter support per channel automations?
  CLAP_PARAM_IS_AUTOMATABLE_PER_PORT    // Does this parameter support per port automations?
  CLAP_PARAM_IS_MODULATABLE             // Does this parameter support the modulation signal?
  CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID // Does this parameter support per note modulations?
  CLAP_PARAM_IS_MODULATABLE_PER_KEY     // Does this parameter support per key modulations?
  CLAP_PARAM_IS_MODULATABLE_PER_CHANNEL // Does this parameter support per channel modulations?
  CLAP_PARAM_IS_MODULATABLE_PER_PORT    // Does this parameter support per port modulations?
  CLAP_PARAM_REQUIRES_PROCESS           // Any change to this parameter will affect the plugin output and requires to be done via process() if the plugin is active. A simple example would be a DC Offset, changing it will change the output signal and must be processed.
  CLAP_PARAM_IS_ENUM                    // This parameter represents an enumerated value. If you set this flag, then you must set CLAP_PARAM_IS_STEPPED too. All values from min to max must not have a blank value_to_text().
*/

//----------------------------------------------------------------------

class SAT_Parameter {

//------------------------------
protected:
//------------------------------

  clap_param_info_t MInfo                           = {};
  int32_t           MIndex                          = -1;
  sat_param_t       MValue                          = 0.0;
  sat_param_t       MModulation                     = 0.0;

  uint32_t          MLastPainted                    = 0;
  char              MValueText[SAT_MAX_NAME_LENGTH] = {0};
  const char*       MValueFormat                    = "%.2f";

  uint32_t          MAutomationState                = 0;
  SAT_Color         MAutomationColor                = SAT_White;
  bool              MIsMapped                       = false;
  SAT_Color         MMappedColor                    = SAT_Black;

  void*             MWidget                         = nullptr;
  uint32_t          MWidgetIndex                    = 0;  // connect..

  bool              MGuiAutomationDirty             = false;
  sat_param_t       MLastAutomatedValue             = 0.0;

  bool              MGuiModulationDirty             = false;
  sat_param_t       MLastModulatedValue             = 0.0;

//------------------------------
public:
//------------------------------

  SAT_Parameter(const char* AName, sat_param_t AValue=0.0, sat_param_t AMinValue=0.0, sat_param_t AMaxValue=1.0, uint32_t AFlags=CLAP_PARAM_IS_AUTOMATABLE) {
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
    MInfo.cookie = this;
    setValue(AInfo->default_value);
  }

  //----------

  virtual ~SAT_Parameter() {
  }

//------------------------------
public:
//------------------------------

  virtual void*                 getWidget()               { return MWidget; }
  virtual uint32_t              getWidgetIndex()          { return MWidgetIndex; }
  virtual int32_t               getIndex()                { return MIndex; }
  virtual clap_param_info_t*    getInfo()                 { return &MInfo; }
  virtual sat_param_t           getValue()                { return MValue; }
  virtual sat_param_t           getModulation()           { return MModulation; }
  //virtual sat_param_t           getNormalizedValue()      { return normalize(MValue); }
  //virtual sat_param_t           getNormalizedModulation() { return normalize(MModulation); }
  virtual uint32_t              getAutomationState()      { return MAutomationState; }
  virtual SAT_Color             getAutomationColor()      { return MAutomationColor; }
  virtual bool                  getIsMapped()             { return MIsMapped; }
  virtual SAT_Color             getMappedColor()          { return MMappedColor; }
  virtual clap_id               getId()                   { return MInfo.id; }
  virtual const char*           getName()                 { return MInfo.name; }
  virtual const char*           getModule()               { return MInfo.name; }
  virtual clap_param_info_flags getFlags()                { return MInfo.flags; }
  virtual void*                 getCookie()               { return MInfo.cookie; }
  virtual sat_param_t           getMinValue()             { return MInfo.min_value; }
  virtual sat_param_t           getMaxValue()             { return MInfo.max_value; }
  virtual sat_param_t           getDefaultValue()         { return MInfo.default_value; }
  virtual sat_param_t           getLastAutomatedValue()   { return MLastAutomatedValue; }
  virtual sat_param_t           getLastModulatedValue()   { return MLastModulatedValue; }
  virtual bool                  isGuiAutomationDirty()    { return MGuiAutomationDirty; }
  virtual bool                  isGuiModulationDirty()    { return MGuiModulationDirty; }
//------------------------------
public:
//------------------------------

  virtual void        setWidget(void* AWidget)                  { MWidget = AWidget; }
  virtual void        setIndex(int32_t AIndex)                  { MIndex = AIndex; MInfo.id  = AIndex; }  // !!!!!
  virtual void        setModulation(sat_param_t AValue)         { MModulation = AValue; }
  virtual void        setValue(sat_param_t AValue)              { MValue = AValue; }
  virtual void        setValueFormat(const char* AFormat)       { MValueFormat = AFormat; }
  virtual void        setAutomationState(uint32_t AState)       { MAutomationState = AState; }
  virtual void        setAutomationColor(SAT_Color AColor)      { MAutomationColor = AColor; }
  virtual void        setIsMapped(bool AMapped)                 { MIsMapped = AMapped; }
  virtual void        setMappedColor(SAT_Color AColor)          { MMappedColor = AColor; }
  virtual void        setGuiAutomationDirty(bool ADirty=true)   { MGuiAutomationDirty = ADirty; }
  virtual void        setLastAutomatedValue(sat_param_t AValue) { MLastAutomatedValue = AValue; }
  virtual void        setGuiModulationDirty(bool ADirty=true)   { MGuiModulationDirty = ADirty; }
  virtual void        setLastModulatedValue(sat_param_t AValue) { MLastModulatedValue = AValue; }
  virtual void        setFlag(clap_param_info_flags AFlag)      { MInfo.flags |= AFlag; }
  virtual void        clearFlag(clap_param_info_flags AFlag)    { MInfo.flags &= ~AFlag; }

  // virtual void        setSmoothFactor(sat_param_t AFactor)      { MSmoothFactor = AFactor; }
  // virtual void        setSmoothValue(sat_param_t AValue)        { MSmoothValue = AValue; }
  // virtual void        setSmoothTarget(sat_param_t ATarget)      { MSmoothValue = AValue; }
  // virtual sat_param_t getSmoothValue() { return MSmoothValue; }
  // virtual sat_param_t updateSmoothValue() {
  //   MSmoothValue += (MSmoothTarget - MSmoothValue) * MSmoothFactor;
  //   return MSmoothValue;
  // }

//------------------------------
public:
//------------------------------

  virtual void connect(void* AWidget, uint32_t AIndex=0) {
    MWidget = AWidget;
    MWidgetIndex = AIndex;
  }

  //----------

  virtual sat_param_t normalize(sat_param_t AValue) {
    sat_param_t range = MInfo.max_value - MInfo.min_value;
    if (range > 0.0) {
      return (AValue - MInfo.min_value) / range;
    }
    return 0.0;
  }

  //----------

  virtual sat_param_t denormalize(sat_param_t AValue) {
    sat_param_t range = MInfo.max_value - MInfo.min_value;
    return MInfo.min_value + (AValue * range);
  }

  //----------

  virtual sat_param_t getNormalizedValue() {
    sat_param_t value = getValue();
    return normalize(value);
  }
  
  //----------
  
  virtual sat_param_t getNormalizedModulation() {
    sat_param_t modulation = getModulation();
    sat_param_t range = MInfo.max_value - MInfo.min_value;
    if (range <= 0.0) return modulation;
    return modulation / range;
  }
  
  //----------  

  virtual const char* valueToText(sat_param_t AValue) {
    sprintf(MValueText,MValueFormat,AValue);//getValue());
    return MValueText;
  }

  //----------

  virtual sat_param_t textToValue(const char* AText) {
    return atof(AText);
  }

};

//----------------------------------------------------------------------
#endif
