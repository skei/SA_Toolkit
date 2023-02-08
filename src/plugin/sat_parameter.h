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
public:
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

  clap_param_info_t MParamInfo                      = {0};
  int32_t           MIndex                          = -1;

  sat_param_t       MValue                          = 0.0;
  char              MValueText[SAT_MAX_NAME_LENGTH] = {0};
  uint32_t          MNumDigits                      = 2;

  sat_param_t       MModulation                     = 0.0;
  bool              MIsModulated                    = false;
  uint32_t          MLastModulated                  = 0;

//------------------------------
public:
//------------------------------

  SAT_Parameter(const char* AName, sat_param_t AValue=0.0, sat_param_t AMinValue=0.0, sat_param_t AMaxValue=1.0) {
    MParamInfo.id             = 0;
    MParamInfo.flags          = CLAP_PARAM_IS_AUTOMATABLE
                              | CLAP_PARAM_IS_MODULATABLE
                              | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID;
    MParamInfo.cookie         = this;
    strcpy(MParamInfo.name,AName);
    //strcpy(MParamInfo.module,AModule);
    MParamInfo.default_value  = AValue;
    MParamInfo.min_value      = AMinValue;
    MParamInfo.max_value      = AMaxValue;
    MValue                    = AValue;
  }

  //----------

  virtual ~SAT_Parameter() {
  }

//------------------------------
public:
//------------------------------

  clap_id               getId()               { return MParamInfo.id; }
  clap_param_info_flags getFlags()            { return MParamInfo.flags; }
  void *                getCookie()           { return MParamInfo.cookie; }
  char*                 getName()             { return MParamInfo.name; }
  char*                 getModule()           { return MParamInfo.module; }
  sat_param_t           getMinValue()         { return MParamInfo.min_value; }
  sat_param_t           getMaxValue()         { return MParamInfo.max_value; }
  sat_param_t           getDefaultValue()     { return MParamInfo.default_value; }

  //----------

  int32_t               getIndex()            { return MIndex; }
  clap_param_info_t*    getParamInfo()        { return &MParamInfo; }
  sat_param_t           getValue()            { return MValue; }
  sat_param_t           getModulatione()      { return MModulation; }

  bool                  getIsModulated()      { return MIsModulated; }
  uint32_t              getLastModulated()    { return MLastModulated; }

//------------------------------
public:
//------------------------------

  void setId(clap_id AId)                     { MParamInfo.id = AId; }
  void setFlags(clap_param_info_flags AFlags) { MParamInfo.flags = AFlags; }
  void setCookie(void* ACookie)               { MParamInfo.cookie = ACookie; }
  void setName(char* AName)                   { strcpy(MParamInfo.name,AName); }
  void setModule(char* AModule)               { strcpy(MParamInfo.module,AModule); }
  void setMinValue(sat_param_t AValue)        { MParamInfo.min_value = AValue; }
  void setMaxValue(sat_param_t AValue)        { MParamInfo.max_value = AValue; }
  void setDefaultValue(sat_param_t AValue)    { MParamInfo.default_value = AValue; }

  //----------

  void setIndex(int32_t AIndex)               { MIndex = AIndex; }
  void setModulation(sat_param_t AValue)      { MModulation = AValue; }
  void setValue(sat_param_t AValue)           { MValue = AValue; }

  void setIsModulated(bool AState)            { MIsModulated = AState; }
  void setLastModulated(uint32_t ALast)       { MLastModulated = ALast; }

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
