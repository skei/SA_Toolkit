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

  clap_param_info_t MParamInfo  = {0};

  int32_t           MIndex      = -1;
  double            MValue      = 0.0;
  uint32_t          MNumDigits  = 3;

  char              MValueText[SAT_MAX_NAME_LENGTH] = {0};
//------------------------------
public:
//------------------------------

  SAT_Parameter(const char* AName, double AValue=0.0, double AMinValue=0.0, double AMaxValue=1.0) {
    MParamInfo.id             = 0;
    MParamInfo.flags          = CLAP_PARAM_IS_AUTOMATABLE;
    MParamInfo.cookie         = this;
    strcpy(MParamInfo.name,AName);
    //strcpy(MParamInfo.module,AModule);
    MParamInfo.default_value  = AValue;
    MParamInfo.min_value      = AMinValue;
    MParamInfo.max_value      = AMaxValue;
  }

  //----------

  virtual ~SAT_Parameter() {
  }

//------------------------------
public:
//------------------------------

  clap_id               getId()           { return MParamInfo.id; }
  clap_param_info_flags getFlags()        { return MParamInfo.flags; }
  void *                getCookie()       { return MParamInfo.cookie; }
  char*                 getName()         { return MParamInfo.name; }
  char*                 getModule()       { return MParamInfo.module; }
  double                getMinValue()     { return MParamInfo.min_value; }
  double                getMaxValue()     { return MParamInfo.max_value; }
  double                getDefaultValue() { return MParamInfo.default_value; }

  int32_t               getIndex()        { return MIndex; }
  clap_param_info_t*    getParamInfo()    { return &MParamInfo; }
  double                getValue()        { return MValue; }

  //----------

  void setId(clap_id AId)                     { MParamInfo.id = AId; }
  void setFlags(clap_param_info_flags AFlags) { MParamInfo.flags = AFlags; }
  void setCookie(void* ACookie)               { MParamInfo.cookie = ACookie; }
  void setName(char* AName)                   { strcpy(MParamInfo.name,AName); }
  void setModule(char* AModule)               { strcpy(MParamInfo.module,AModule); }
  void setMinValue(double AValue)             { MParamInfo.min_value = AValue; }
  void setMaxValue(double AValue)             { MParamInfo.max_value = AValue; }
  void setDefaultValue(double AValue)         { MParamInfo.default_value = AValue; }

  void setIndex(int32_t AIndex)               { MIndex = AIndex; }
  void setNumDigits(uint32_t ANum)            { MNumDigits = ANum; }
  void setValue(double AValue)                { MValue = AValue; }

//------------------------------
public:
//------------------------------

  virtual double normalizeValue(double AValue) {
    return AValue;
  }

  //----------

  virtual double denormalizeValue(double AValue) {
    return AValue;
  }

  //----------

  virtual bool valueToText(double AValue, char* ABuffer, uint32_t ABufferSize) {
    char format[16] = {0};
    format[0] = '%';
    format[1] = '.';
    format[2] = '0' + MNumDigits;
    format[2] = 'f';
    format[3] = 0;
    sprintf(ABuffer,format,AValue);
    return true;;
  }

  //----------

  virtual bool textToValue(const char* AText, double* AValue) {
    //*AValue = 0.0;
    return false;
  }

};

//----------------------------------------------------------------------
#endif
