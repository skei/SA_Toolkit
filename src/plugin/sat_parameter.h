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

  virtual void*                 getWidget()               { return MWidget; }
  virtual uint32_t              getWidgetIndex()          { return MWidgetIndex; }
  virtual int32_t               getIndex()                { return MIndex; }
  virtual clap_param_info_t*    getInfo()                 { return &MInfo; }
  virtual sat_param_t           getValue()                { return MValue; }
  virtual sat_param_t           getModulation()           { return MModulation; }
  virtual sat_param_t           getNormalizedValue()      { return normalize(MValue); }
  virtual sat_param_t           getNormalizedModulation() { return normalize(MModulation); }
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

  virtual const char* valueToText(sat_param_t AValue) {
    sprintf(MValueText,MValueFormat,getValue());
    return MValueText;
  }

  //----------

  virtual sat_param_t textToValue(const char* AText) {
    return atof(AText);
  }

};

//----------------------------------------------------------------------
#endif
