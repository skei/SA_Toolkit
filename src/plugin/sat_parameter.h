#ifndef sat_parameter_included
#define sat_parameter_included
//----------------------------------------------------------------------

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

  /*
    clap_id id;
    clap_param_info_flags flags;
    void *cookie;
    char name[CLAP_NAME_SIZE];
    char module[CLAP_PATH_SIZE];
    double min_value;
    double max_value;
    double default_value;
  */

  clap_param_info_t MInfo                           = {0};
  int32_t           MIndex                          = -1;
  void*             MWidget                         = nullptr;
  void*             MConnection                     = nullptr;
  char              MValueText[SAT_MAX_NAME_LENGTH] = {0};
  uint32_t          MNumDigits                      = 3;

  sat_param_t       MValue                          = 0.0;
  sat_param_t       MModulation                     = 0.0;

  int32_t           MLastPainted                    = -1;
  int32_t           MLastUpdated                    = 0;
  int32_t           MLastModulated                  = 0;
  double            MLastUpdatedValue               = 0.0;
  double            MLastModulatedValue             = 0.0;

//------------------------------
public:
//------------------------------

  SAT_Parameter(const char* AName, sat_param_t AValue=0.0, sat_param_t AMinValue=0.0, sat_param_t AMaxValue=1.0) {
    MInfo.id             = 0;
    MInfo.flags          = CLAP_PARAM_IS_AUTOMATABLE;// | CLAP_PARAM_IS_MODULATABLE | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID;
    MInfo.cookie         = this;
    strcpy(MInfo.name,AName);
    strcpy(MInfo.module,"");
    MInfo.default_value  = AValue;
    MInfo.min_value      = AMinValue;
    MInfo.max_value      = AMaxValue;
  }

  //----------

  SAT_Parameter(clap_param_info_t* info) {
    MInfo.id             = info->id;
    MInfo.flags          = info->flags;
    MInfo.cookie         = this;
    strcpy(MInfo.name,info->name);
    strcpy(MInfo.module,info->module);
    MInfo.default_value  = info->default_value;
    MInfo.min_value      = info->min_value;
    MInfo.max_value      = info->max_value;
  }

  //----------

  virtual ~SAT_Parameter() {
  }

//------------------------------
public:
//------------------------------

  clap_id               getId()                 { return MInfo.id; }
  clap_param_info_flags getFlags()              { return MInfo.flags; }
  void *                getCookie()             { return MInfo.cookie; }
  char*                 getName()               { return MInfo.name; }
  char*                 getModule()             { return MInfo.module; }
  sat_param_t           getMinValue()           { return MInfo.min_value; }
  sat_param_t           getMaxValue()           { return MInfo.max_value; }
  sat_param_t           getDefaultValue()       { return MInfo.default_value; }

  int32_t               getIndex()              { return MIndex; }
  clap_param_info_t*    getParamInfo()          { return &MInfo; }
  void*                 getWidget()             { return MWidget; }
  void*                 getConnection()         { return MConnection; }

  sat_param_t           getValue()              { return MValue; }
  sat_param_t           getModulation()         { return MModulation; }

  int32_t               getLastPainted()        { return MLastPainted; }
  int32_t               getLastUpdated()        { return MLastUpdated; }
  int32_t               getLastModulated()      { return MLastModulated; }
  double                getLastUpdatedValue()   { return MLastUpdated; }
  double                getLastModulatedValue() { return MLastModulated; }


//------------------------------
public:
//------------------------------

  void setId(clap_id AId)                     { MInfo.id = AId; }
  void setFlags(clap_param_info_flags AFlags) { MInfo.flags = AFlags; }
  void setCookie(void* ACookie)               { MInfo.cookie = ACookie; }
  void setName(char* AName)                   { strcpy(MInfo.name,AName); }
  void setModule(char* AModule)               { strcpy(MInfo.module,AModule); }
  void setMinValue(sat_param_t AValue)        { MInfo.min_value = AValue; }
  void setMaxValue(sat_param_t AValue)        { MInfo.max_value = AValue; }
  void setDefaultValue(sat_param_t AValue)    { MInfo.default_value = AValue; }
  void setWidget(void* AWidget)               { MWidget = AWidget; }
  void setFlag(clap_param_info_flags AFlag)   { MInfo.flags |= AFlag; }
  void clearFlag(clap_param_info_flags AFlag) { MInfo.flags &= ~AFlag; }

  void setIndex(int32_t AIndex) {
    MIndex    = AIndex;
    MInfo.id  = AIndex;
  }

  void connect(void* AWidget) {
    MConnection = AWidget;
  }

  void setValue(double AValue)      { MValue = AValue; }
  void setModulation(double AValue) { MModulation = AValue; }

  void setLastPainted(uint32_t ALast)         { MLastPainted = ALast; }
  void setLastUpdated(uint32_t ALast)         { MLastUpdated = ALast; }
  void setLastModulated(uint32_t ALast)       { MLastModulated = ALast; }
  void setLastUpdatedValue(double AValue)     { MLastUpdatedValue = AValue; }
  void setLastModulatedValue(double AValue)   { MLastModulatedValue = AValue; }



//------------------------------
public:
//------------------------------

  virtual sat_param_t normalizeValue(sat_param_t AValue) {
    return AValue;
  }

  //----------

  virtual sat_param_t denormalizeValue(sat_param_t AValue) {
    return AValue;
  }

  //----------

  virtual bool valueToText(sat_param_t AValue, char* ABuffer, uint32_t ABufferSize) {
    char format[16] = {0};
    format[0] = '%';
    format[1] = '.';
    format[2] = '0' + MNumDigits;
    format[3] = 'f';
    format[4] = 0;
    sprintf(ABuffer,format,AValue);
    return true;;
  }

  //----------

  virtual bool textToValue(const char* AText, sat_param_t* AValue) {
    //*AValue = 0.0;
    return false;
  }

};

//----------------------------------------------------------------------
#endif
