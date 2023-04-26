#ifndef sa_botage_parameters_included
#define sa_botage_parameters_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_plugin.h"

//----------------------------------------------------------------------

#define SA_BOTAGE_FX_TYPE_COUNT 8
const char* fx_type_text[SA_BOTAGE_FX_TYPE_COUNT] = {
  "Off",
  "Lowpass",
  "Highpass",
  "Bandpass",
  "Distortion",
  "fx6",
  "fx7",
  "fx8"
  //"Delay",
  //"Distortion",
  //"Bitcrusher",
  //"Comb Filter"
};

enum fx_type_enums {
  FX_OFF = 0,
  FX_LP,
  FX_HP,
  FX_BP,
  FX_DIST,
  FX_5,
  FX_6,
  FX_7,
  //FX_DELAY,
  //FX_DISTORTION,
  //FX_BITCRUSHER,
  //FX_COMBFILTER
};

enum fx_mode_enums {
  FX_MODE_SINGLE  = 0,
  FX_MODE_MULTI,
  FX_MODE_ALL,
  FX_MODE_COUNT
};


//#define SA_BOTAGE_FX_MODE_COUNT 3
const char* fx_mode_text[FX_MODE_COUNT] = {
  "Single",
  "Multi",
  "All"
};

//----------------------------------------------------------------------

enum sa_botage_params_e {

  PAR_NUM_BEATS = 0,
  PAR_NUM_SLICES,

  PAR_RANGE_PROB,
  PAR_RANGE_SLICE_COUNT,
  PAR_RANGE_LOOP_COUNT,

  PAR_LOOP_ENV_ATTACK,
  PAR_LOOP_ENV_DECAY,
  PAR_SLICE_ENV_ATTACK,
  PAR_SLICE_ENV_DECAY,

  PAR_PROB_SIZE_PROB_RANGE,
  PAR_PROB_SIZE_MIN_RANGE,
  PAR_PROB_SIZE_MAX_RANGE,
  PAR_PROB_SIZE_PROB_LOOP,
  PAR_PROB_SIZE_MIN_LOOP,
  PAR_PROB_SIZE_MAX_LOOP,

  PAR_PROB_SPEED_PROB_RANGE,
  PAR_PROB_SPEED_MIN_RANGE,
  PAR_PROB_SPEED_MAX_RANGE,
  PAR_PROB_SPEED_PROB_LOOP,
  PAR_PROB_SPEED_MIN_LOOP,
  PAR_PROB_SPEED_MAX_LOOP,

  PAR_PROB_OFFSET_PROB_RANGE,
  PAR_PROB_OFFSET_MIN_RANGE,
  PAR_PROB_OFFSET_MAX_RANGE,
  PAR_PROB_OFFSET_PROB_LOOP,
  PAR_PROB_OFFSET_MIN_LOOP,
  PAR_PROB_OFFSET_MAX_LOOP,

  PAR_PROB_REVERSE_PROB_RANGE,
  PAR_PROB_REVERSE_PROB_LOOP,

  PAR_PROB_FX_PROB_RANGE,
  PAR_PROB_FX_MIN_RANGE,
  PAR_PROB_FX_MAX_RANGE,
  PAR_PROB_FX_PROB_LOOP,
  PAR_PROB_FX_MIN_LOOP,
  PAR_PROB_FX_MAX_LOOP,

  PAR_FX_MODE,

  PAR_FX1_PROB,
  PAR_FX1_TYPE,
  PAR_FX1_PAR1,
  PAR_FX1_PAR2,
  PAR_FX1_PAR3,

  PAR_FX2_PROB,
  PAR_FX2_TYPE,
  PAR_FX2_PAR1,
  PAR_FX2_PAR2,
  PAR_FX2_PAR3,

  PAR_FX3_PROB,
  PAR_FX3_TYPE,
  PAR_FX3_PAR1,
  PAR_FX3_PAR2,
  PAR_FX3_PAR3,

  PAR_FX4_PROB,
  PAR_FX4_TYPE,
  PAR_FX4_PAR1,
  PAR_FX4_PAR2,
  PAR_FX4_PAR3,

  PAR_FX5_PROB,
  PAR_FX5_TYPE,
  PAR_FX5_PAR1,
  PAR_FX5_PAR2,
  PAR_FX5_PAR3,

  SA_BOTAGE_PARAM_COUNT
};


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ScalePercentParameter
: public MIP_Parameter {

public:

  MIP_ScalePercentParameter(const char* AName="", double AValue=0.0, double AMin=-1.0, double AMax=1.0)
  : MIP_Parameter(AName,AValue,AMin,AMax) {
  }

public:

  bool valueToText(double value, char *display, uint32_t size) override {
    double v = powf(0.5,-value);
    v *= 100.0;
    //sprintf(display,"%.2f",v);
    uint32_t iv = MIP_Trunc(v);
    sprintf(display,"%i",iv);
    return true;
  }

};

//----------------------------------------------------------------------

class MIP_PercentParameter
: public MIP_Parameter {

public:

  MIP_PercentParameter(const char* AName="", double AValue=0.0, double AMin=-1.0, double AMax=1.0)
  : MIP_Parameter(AName,AValue,AMin,AMax) {
  }

public:

  bool valueToText(double value, char *display, uint32_t size) override {
    double v = value;//powf(0.5,-value);
    v *= 100.0;
    //sprintf(display,"%.2f",v);
    uint32_t iv = MIP_Trunc(v);
    sprintf(display,"%i",iv);
    return true;
  }

};

//----------------------------------------------------------------------

class MIP_OffsetParameter
: public MIP_Parameter {

public:

  MIP_OffsetParameter(const char* AName="", double AValue=0.0, double AMin=-1.0, double AMax=1.0)
  : MIP_Parameter(AName,AValue,AMin,AMax) {
  }

public:

  bool valueToText(double value, char *display, uint32_t size) override {
    double v = value;//powf(0.5,-value);
    v *= 8.0;
    //sprintf(display,"%.2f",v);
    uint32_t iv = MIP_Trunc(v);
    sprintf(display,"%i",iv);
    return true;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void sa_botage_init_parameters(MIP_Plugin* APlugin) {

  APlugin->appendParameter(new MIP_IntParameter(          "Beats",                4,          1, 8 ));
  APlugin->appendParameter(new MIP_IntParameter(          "Slices",               2,          1, 8 ));

  // ----- slice

  APlugin->appendParameter(new MIP_PercentParameter(      "Range Prob",           0.25,       0, 1   ));
  APlugin->appendParameter(new MIP_IntParameter(          "Range Slice Count",    0b00001111, 1, 255 ));
  APlugin->appendParameter(new MIP_IntParameter(          "Range Loop Count",     0b00001111, 1, 255 ));

  // ----- env

  APlugin->appendParameter(new MIP_Parameter(             "Loop Env Attack",      1,          0, 25 ));
  APlugin->appendParameter(new MIP_Parameter(             "Loop Env Decay",       1,          0, 25 ));   // !!!!!
  APlugin->appendParameter(new MIP_Parameter(             "Slice Env Attack",     0,          0, 1 ));
  APlugin->appendParameter(new MIP_Parameter(             "Slice Env Decay",      0,          0, 1 ));    // !!!!!

//  // ----- fx filter

  // ----- loop size

  APlugin->appendParameter(new MIP_PercentParameter(      "Size Prob Range",      0.25,       0, 1 ));
  APlugin->appendParameter(new MIP_ScalePercentParameter( "Size Min Range",      -0.5,       -1, 1 ));
  APlugin->appendParameter(new MIP_ScalePercentParameter( "Size Max Range",       0.5,       -1, 1 ));

  APlugin->appendParameter(new MIP_PercentParameter(      "Size Prob Loop",       0.25,       0, 1 ));
  APlugin->appendParameter(new MIP_ScalePercentParameter( "Size Min Loop",       -0.5,       -1, 1 ));
  APlugin->appendParameter(new MIP_ScalePercentParameter( "Size Max Loop",        0.5,       -1, 1 ));

  // ----- loop speed

  APlugin->appendParameter(new MIP_PercentParameter(      "Speed Prob Range",     0.25,       0, 1 ));
  APlugin->appendParameter(new MIP_ScalePercentParameter( "Speed Min Range",     -0.5,       -1, 1 ));
  APlugin->appendParameter(new MIP_ScalePercentParameter( "Speed Max Range",      0.5,       -1, 1 ));

  APlugin->appendParameter(new MIP_PercentParameter(      "Speed Prob Loop",      0.25,       0, 1 ));
  APlugin->appendParameter(new MIP_ScalePercentParameter( "Speed Min Loop",      -0.5,       -1, 1 ));
  APlugin->appendParameter(new MIP_ScalePercentParameter( "Speed Max Loop",       0.5,       -1, 1 ));

  // ----- loop offset

  APlugin->appendParameter(new MIP_PercentParameter(      "Offset Prob Range",    0.25,       0, 1 ));
  APlugin->appendParameter(new MIP_OffsetParameter(       "Offset Min Range",    -0.5,       -1, 1 ));
  APlugin->appendParameter(new MIP_OffsetParameter(       "Offset Max Range",     0.5,       -1, 1 ));

  APlugin->appendParameter(new MIP_PercentParameter(      "Offset Prob Loop",     0.25,       0, 1 ));
  APlugin->appendParameter(new MIP_OffsetParameter(       "Offset Min Loop",     -0.5,       -1, 1 ));
  APlugin->appendParameter(new MIP_OffsetParameter(       "Offset Max Loop",      0.5,       -1, 1 ));

  // ----- loop reverse

  APlugin->appendParameter(new MIP_PercentParameter(      "Reverse Prob Range",   0.25,       0, 1 ));
  APlugin->appendParameter(new MIP_PercentParameter(      "Reverse Prob Loop",    0.25,       0, 1 ));

  // ----- fx prob

  APlugin->appendParameter(new MIP_PercentParameter(      "FX Prob Range",        0.25,       0, 1 ));
  APlugin->appendParameter(new MIP_ScalePercentParameter( "FX Min Range",        -0.5,       -1, 1 ));
  APlugin->appendParameter(new MIP_ScalePercentParameter( "FX Max Range",         0.5,       -1, 1 ));

  APlugin->appendParameter(new MIP_PercentParameter(      "FX Prob Loop",         0.25,       0, 1 ));
  APlugin->appendParameter(new MIP_ScalePercentParameter( "FX Min Loop",         -0.5,       -1, 1 ));
  APlugin->appendParameter(new MIP_ScalePercentParameter( "FX Max Loop",          0.5,       -1, 1 ));

  // ----- effects

  APlugin->appendParameter(new MIP_TextParameter(         "FX Mode",              0,          0, FX_MODE_COUNT-1, fx_mode_text ));

  APlugin->appendParameter(new MIP_PercentParameter(      "FX1 Prob",             0.5,        0, 1 ));
  APlugin->appendParameter(new MIP_TextParameter(         "FX1 Type",             1,          0, SA_BOTAGE_FX_TYPE_COUNT-1, fx_type_text ));
  APlugin->appendParameter(new MIP_Parameter(             "FX1 Param1",           0.6,        0, 1 ));
  APlugin->appendParameter(new MIP_Parameter(             "FX1 Param2",           0.4,        0, 1 ));
  APlugin->appendParameter(new MIP_Parameter(             "FX1 Param3",           0.0,        0, 1 ));

  APlugin->appendParameter(new MIP_PercentParameter(      "FX2 Prob",             0.5,        0, 1 ));
  APlugin->appendParameter(new MIP_TextParameter(         "FX2 Type",             2,          0, SA_BOTAGE_FX_TYPE_COUNT-1, fx_type_text ));
  APlugin->appendParameter(new MIP_Parameter(             "FX2 Param1",           0.6,        0, 1 ));
  APlugin->appendParameter(new MIP_Parameter(             "FX2 Param2",           0.4,        0, 1 ));
  APlugin->appendParameter(new MIP_Parameter(             "FX2 Param3",           0.2,        0, 1 ));

  APlugin->appendParameter(new MIP_PercentParameter(      "FX3 Prob",             0.5,        0, 1 ));
  APlugin->appendParameter(new MIP_TextParameter(         "FX3 Type",             3,          0, SA_BOTAGE_FX_TYPE_COUNT-1, fx_type_text ));
  APlugin->appendParameter(new MIP_Parameter(             "FX3 Param1",           0.6,        0, 1 ));
  APlugin->appendParameter(new MIP_Parameter(             "FX3 Param2",           0.4,        0, 1 ));
  APlugin->appendParameter(new MIP_Parameter(             "FX3 Param3",           0.8,        0, 1 ));

  APlugin->appendParameter(new MIP_PercentParameter(      "FX4 Prob",             0,          0, 1 ));
  APlugin->appendParameter(new MIP_TextParameter(         "FX4 Type",             4,          0, SA_BOTAGE_FX_TYPE_COUNT-1, fx_type_text ));
  APlugin->appendParameter(new MIP_Parameter(             "FX4 Param1",           0.5,        0, 1 ));
  APlugin->appendParameter(new MIP_Parameter(             "FX4 Param2",           0.5,        0, 1 ));
  APlugin->appendParameter(new MIP_Parameter(             "FX4 Param3",           0.0,        0, 1 ));

  APlugin->appendParameter(new MIP_PercentParameter(      "FX5 Prob",             0,          0, 1 ));
  APlugin->appendParameter(new MIP_TextParameter(         "FX5 Type",             0,          0, SA_BOTAGE_FX_TYPE_COUNT-1, fx_type_text ));
  APlugin->appendParameter(new MIP_Parameter(             "FX5 Param1",           0,          0, 1 ));
  APlugin->appendParameter(new MIP_Parameter(             "FX5 Param2",           0,          0, 1 ));
  APlugin->appendParameter(new MIP_Parameter(             "FX5 Param3",           0.0,        0, 1 ));

  MIP_Assert( APlugin->getParameterCount() == SA_BOTAGE_PARAM_COUNT );

}

//----------------------------------------------------------------------
#endif
