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
  void*             MConnection                     = nullptr;
  
  sat_param_t       MValue                          = 0.0;
  sat_param_t       MModulation                     = 0.0;
  
  sat_param_t       MSmoothValue                    = 0.0;
  sat_param_t       MSmoothFactor                   = 1.0;

  int32_t           MLastPainted                    = -1;
  int32_t           MLastUpdated                    = 0;
  int32_t           MLastModulated                  = 0;
  double            MLastUpdatedValue               = 0.0;
  double            MLastModulatedValue             = 0.0;

  uint32_t          MNumDigits                      = 2;
//char              MValueText[SAT_MAX_NAME_LENGTH] = {0};
  

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
  void*                 getConnection()         { return MConnection; }

  sat_param_t           getValue()              { return MValue; }
  sat_param_t           getModulation()         { return MModulation; }

  int32_t               getLastPainted()        { return MLastPainted; }
  int32_t               getLastUpdated()        { return MLastUpdated; }
  int32_t               getLastModulated()      { return MLastModulated; }
  sat_param_t           getLastUpdatedValue()   { return MLastUpdated; }
  sat_param_t           getLastModulatedValue() { return MLastModulated; }


//------------------------------
public:
//------------------------------

  void setId(clap_id AId)                         { MInfo.id = AId; }
  void setFlags(clap_param_info_flags AFlags)     { MInfo.flags = AFlags; }
  void setCookie(void* ACookie)                   { MInfo.cookie = ACookie; }
  void setName(char* AName)                       { strcpy(MInfo.name,AName); }
  void setModule(char* AModule)                   { strcpy(MInfo.module,AModule); }
  void setMinValue(sat_param_t AValue)            { MInfo.min_value = AValue; }
  void setMaxValue(sat_param_t AValue)            { MInfo.max_value = AValue; }
  void setDefaultValue(sat_param_t AValue)        { MInfo.default_value = AValue; }
  void setFlag(clap_param_info_flags AFlag)       { MInfo.flags |= AFlag; }
  void clearFlag(clap_param_info_flags AFlag)     { MInfo.flags &= ~AFlag; }
  void setIndex(int32_t AIndex)                   { MIndex = AIndex; MInfo.id = AIndex; }
  void connect(void* AWidget)                     { MConnection = AWidget; }

  void setValue(double AValue)                    { MValue = AValue; }
  void setModulation(double AValue)               { MModulation = AValue; }

  void setLastPainted(uint32_t ALast)             { MLastPainted = ALast; }
  void setLastUpdated(uint32_t ALast)             { MLastUpdated = ALast; }
  void setLastModulated(uint32_t ALast)           { MLastModulated = ALast; }
  void setLastUpdatedValue(sat_param_t AValue)    { MLastUpdatedValue = AValue; }
  void setLastModulatedValue(sat_param_t AValue)  { MLastModulatedValue = AValue; }

  void setSmoothValue(sat_param_t AValue)         { MSmoothValue = AValue; }
  void setSmoothFactor(sat_param_t AFactor)       { MSmoothFactor = AFactor; }

//------------------------------
public:
//------------------------------

  virtual sat_param_t normalizeValue(sat_param_t AValue) {
    double range = MInfo.max_value - MInfo.min_value;
    if (range <= 0.0) return AValue;
    return (AValue - MInfo.min_value) / range;
  }

  //----------

  virtual sat_param_t denormalizeValue(sat_param_t AValue) {
    double range = MInfo.max_value - MInfo.min_value;
    return MInfo.min_value + (AValue * range);
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
    double value = atof(AText);
    *AValue = value;
    return true;
  }

  //----------

  //virtual void updateSmoothing() {
  //  sat_param_t target = MValue + MModulation;
  //  MSmoothValue += (target - MSmoothValue) * MSmoothFactor;
  //}

  //----------
  
  // smoothing should be done in modulationmatrix (moduationmanager?)
  // and include internal modulation, not expressions, ..

  // http://www.kvraudio.com/forum/viewtopic.php?p=6515525#p6515525

  virtual void updateSmoothing(double ATolerance=0.001, uint32_t ASteps=1) {
    double target = MValue + MModulation;
    double diff = (target - MSmoothValue);
    if ( abs(diff) < ATolerance ) MSmoothValue = target;
    else {
      if (ASteps == 1) {
        //updateSmoothing();
        MSmoothValue += diff * MSmoothFactor;
      }
      else {
        MSmoothValue += diff * (1.0 - pow( (1.0 - MSmoothFactor), ASteps) );
      }
    }
  }

  //----------

  virtual sat_param_t getSmoothedValue() {
    return MSmoothValue;
  }

};

//----------------------------------------------------------------------
#endif
