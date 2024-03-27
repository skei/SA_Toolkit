#ifndef sa_botage_parameters_included
#define sa_botage_parameters_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/sat_parameter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_botage_ScalePercentParameter
: public SAT_Parameter {

public:

  sa_botage_ScalePercentParameter(const char* AName="", double AValue=0.0, double AMin=-1.0, double AMax=1.0, uint32_t AFlags=0)
  : SAT_Parameter(AName,AValue,AMin,AMax,AFlags) {
  }

  sa_botage_ScalePercentParameter(const clap_param_info_t* AInfo)
  : SAT_Parameter(AInfo) {
    //setValueFormat("%.f");
    //setValueFormat("%i");
  }

public:

  //bool valueToText(double value, char *display, uint32_t size) override {
  const char* valueToText(sat_param_t AValue) override {
    double v = powf(0.5,-AValue);
    v *= 100.0;
    uint32_t iv = SAT_Trunc(v);
    //sprintf(MValueText,MValueFormat,AValue);
    sprintf(MValueText,"%i",iv);
    return MValueText;
  }

};

//----------------------------------------------------------------------

class sa_botage_PercentParameter
: public SAT_Parameter {

public:

  sa_botage_PercentParameter(const char* AName="", double AValue=0.0, double AMin=-1.0, double AMax=1.0, uint32_t AFlags=0)
  : SAT_Parameter(AName,AValue,AMin,AMax,AFlags) {
  }

  sa_botage_PercentParameter(const clap_param_info_t* AInfo)
  : SAT_Parameter(AInfo) {
    //setValueFormat("%.f");
    //setValueFormat("%i");
  }

public:

  //bool valueToText(double value, char *display, uint32_t size) override {
  const char* valueToText(sat_param_t AValue) override {
    double v = AValue;//powf(0.5,-value);
    v *= 100.0;
    uint32_t iv = SAT_Trunc(v);
    //sprintf(MValueText,MValueFormat,AValue);
    sprintf(MValueText,"%i",iv);
    return MValueText;
  }

};

//----------------------------------------------------------------------

class sa_botage_OffsetParameter
: public SAT_Parameter {

public:

  sa_botage_OffsetParameter(const char* AName="", double AValue=0.0, double AMin=-1.0, double AMax=1.0, uint32_t AFlags=0)
  : SAT_Parameter(AName,AValue,AMin,AMax,AFlags) {
  }

  sa_botage_OffsetParameter(const clap_param_info_t* AInfo)
  : SAT_Parameter(AInfo) {
    //setValueFormat("%.f");
    //setValueFormat("%i");
  }

public:

  //bool valueToText(double value, char *display, uint32_t size) override {
  const char* valueToText(sat_param_t AValue) override {

    double v = AValue;//powf(0.5,-value);
    v *= 8.0;
    uint32_t iv = SAT_Trunc(v);
    //sprintf(MValueText,MValueFormat,AValue);
    sprintf(MValueText,"%i",iv);
    return MValueText;
  }

};


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

enum sa_botage_parameters_e {
  
  // Global

  SA_BOTAGE_PARAM_GAIN = 0,

  // Envelopes
  
  SA_BOTAGE_PARAM_ENV_LOOP_ATTACK,
  SA_BOTAGE_PARAM_ENV_LOOP_DECAY,
  SA_BOTAGE_PARAM_ENV_SLICE_ATTACK,
  SA_BOTAGE_PARAM_ENV_SLICE_DECAY,

  // Buffer
  
  SA_BOTAGE_PARAM_BUFFER_BEATS,
  SA_BOTAGE_PARAM_BUFFER_SLICES,
  
  // Trigger

  SA_BOTAGE_PARAM_PROB_TRIGGER,
  SA_BOTAGE_PARAM_PROB_TRIGGER_SLICES,
  SA_BOTAGE_PARAM_PROB_TRIGGER_LOOP,

  // Probabilities - Range

  SA_BOTAGE_PARAM_PROB_RANGE_LENGTH,
  SA_BOTAGE_PARAM_PROB_RANGE_LENGTH_MIN,
  SA_BOTAGE_PARAM_PROB_RANGE_LENGTH_MAX,
  SA_BOTAGE_PARAM_PROB_RANGE_SPEED,
  SA_BOTAGE_PARAM_PROB_RANGE_SPEED_MIN,
  SA_BOTAGE_PARAM_PROB_RANGE_SPEED_MAX,
  SA_BOTAGE_PARAM_PROB_RANGE_OFFSET,
  SA_BOTAGE_PARAM_PROB_RANGE_OFFSET_MIN,
  SA_BOTAGE_PARAM_PROB_RANGE_OFFSET_MAX,
  SA_BOTAGE_PARAM_PROB_RANGE_REVERSE,
  SA_BOTAGE_PARAM_PROB_RANGE_FX,
  SA_BOTAGE_PARAM_PROB_RANGE_FX_MIN,
  SA_BOTAGE_PARAM_PROB_RANGE_FX_MAX,

  // Probabilities - Loop

  SA_BOTAGE_PARAM_PROB_LOOP_LENGTH,
  SA_BOTAGE_PARAM_PROB_LOOP_LENGTH_MIN,
  SA_BOTAGE_PARAM_PROB_LOOP_LENGTH_MAX,
  SA_BOTAGE_PARAM_PROB_LOOP_SPEED,
  SA_BOTAGE_PARAM_PROB_LOOP_SPEED_MIN,
  SA_BOTAGE_PARAM_PROB_LOOP_SPEED_MAX,
  SA_BOTAGE_PARAM_PROB_LOOP_OFFSET,
  SA_BOTAGE_PARAM_PROB_LOOP_OFFSET_MIN,
  SA_BOTAGE_PARAM_PROB_LOOP_OFFSET_MAX,
  SA_BOTAGE_PARAM_PROB_LOOP_REVERSE,
  SA_BOTAGE_PARAM_PROB_LOOP_FX,
  SA_BOTAGE_PARAM_PROB_LOOP_FX_MIN,
  SA_BOTAGE_PARAM_PROB_LOOP_FX_MAX,

  // Probabilities - FX

  SA_BOTAGE_PARAM_PROB_FX1,
  SA_BOTAGE_PARAM_PROB_FX1_TYPE,
  SA_BOTAGE_PARAM_PROB_FX1_ARG1,
  SA_BOTAGE_PARAM_PROB_FX1_ARG2,
  SA_BOTAGE_PARAM_PROB_FX1_ARG3,

  SA_BOTAGE_PARAM_PROB_FX2,
  SA_BOTAGE_PARAM_PROB_FX2_TYPE,
  SA_BOTAGE_PARAM_PROB_FX2_ARG1,
  SA_BOTAGE_PARAM_PROB_FX2_ARG2,
  SA_BOTAGE_PARAM_PROB_FX2_ARG3,

  SA_BOTAGE_PARAM_PROB_FX3,
  SA_BOTAGE_PARAM_PROB_FX3_TYPE,
  SA_BOTAGE_PARAM_PROB_FX3_ARG1,
  SA_BOTAGE_PARAM_PROB_FX3_ARG2,
  SA_BOTAGE_PARAM_PROB_FX3_ARG3,

  SA_BOTAGE_PARAM_PROB_FX4,
  SA_BOTAGE_PARAM_PROB_FX4_TYPE,
  SA_BOTAGE_PARAM_PROB_FX4_ARG1,
  SA_BOTAGE_PARAM_PROB_FX4_ARG2,
  SA_BOTAGE_PARAM_PROB_FX4_ARG3,

  SA_BOTAGE_PARAM_PROB_FX5,
  SA_BOTAGE_PARAM_PROB_FX5_TYPE,
  SA_BOTAGE_PARAM_PROB_FX5_ARG1,
  SA_BOTAGE_PARAM_PROB_FX5_ARG2,
  SA_BOTAGE_PARAM_PROB_FX5_ARG3,

  SA_BOTAGE_PARAM_COUNT
};

//----------------------------------------------------------------------

//enum {
//   CLAP_PARAM_IS_STEPPED
//   CLAP_PARAM_IS_PERIODIC
//   CLAP_PARAM_IS_HIDDEN
//   CLAP_PARAM_IS_READONLY
//   CLAP_PARAM_IS_BYPASS
//   CLAP_PARAM_IS_AUTOMATABLE
//   CLAP_PARAM_IS_AUTOMATABLE_PER_NOTE_ID
//   CLAP_PARAM_IS_AUTOMATABLE_PER_KEY
//   CLAP_PARAM_IS_AUTOMATABLE_PER_CHANNEL
//   CLAP_PARAM_IS_AUTOMATABLE_PER_PORT
//   CLAP_PARAM_IS_MODULATABLE
//   CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID
//   CLAP_PARAM_IS_MODULATABLE_PER_KEY
//   CLAP_PARAM_IS_MODULATABLE_PER_CHANNEL
//   CLAP_PARAM_IS_MODULATABLE_PER_PORT
//   CLAP_PARAM_REQUIRES_PROCESS
//};
//typedef uint32_t clap_param_info_flags;

//struct clap_param_info {
//   clap_id id;
//   clap_param_info_flags flags;
//   void *cookie;
//   char name[CLAP_NAME_SIZE];
//   char module[CLAP_PATH_SIZE];
//   double min_value;
//   double max_value;
//   double default_value;
//};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define A   CLAP_PARAM_IS_AUTOMATABLE
#define M   CLAP_PARAM_IS_MODULATABLE
#define MN (CLAP_PARAM_IS_MODULATABLE | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID)
#define S   CLAP_PARAM_IS_STEPPED
#define SE (CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_ENUM )
#define H   CLAP_PARAM_IS_HIDDEN

//----------

//const
clap_param_info_t sa_botage_parameter_infos[SA_BOTAGE_PARAM_COUNT] = {
  
  { SA_BOTAGE_PARAM_GAIN,                   A,    nullptr,  "Gain",             "Global",     0.0, 1.0,   0.5    },
  { SA_BOTAGE_PARAM_ENV_LOOP_ATTACK,        A,    nullptr,  "Loop Attack",      "Global",     1.0, 25.0,   1.0   },
  { SA_BOTAGE_PARAM_ENV_LOOP_DECAY,         A,    nullptr,  "Loop Decay",       "Global",     1.0, 25.0,   1.0   },
  { SA_BOTAGE_PARAM_ENV_SLICE_ATTACK,       A,    nullptr,  "Slice Attack",     "Global",     0.0, 1.0,   0.0    },
  { SA_BOTAGE_PARAM_ENV_SLICE_DECAY,        A,    nullptr,  "Slice Decay",      "Global",     0.0, 1.0,   0.0    },
  { SA_BOTAGE_PARAM_BUFFER_BEATS,           A|S,  nullptr,  "Beats",            "Global",     1,   8,     4      },
  { SA_BOTAGE_PARAM_BUFFER_SLICES,          A|S,  nullptr,  "Slices",           "Global",     1,   8,     2      },

  { SA_BOTAGE_PARAM_PROB_TRIGGER,           A,    nullptr,  "Trigger",          "Trigger",    0.0, 1.0,   0.25   },
  { SA_BOTAGE_PARAM_PROB_TRIGGER_SLICES,    A|S,  nullptr,  "Trigger Slices",   "Trigger",    0,   255,   15     },
  { SA_BOTAGE_PARAM_PROB_TRIGGER_LOOP,      A|S,  nullptr,  "Trigger Loop",     "Trigger",    0,   255,   15     },

  // Probabilities - Range

  { SA_BOTAGE_PARAM_PROB_RANGE_LENGTH,      A,    nullptr,  "Range Length",     "ProbRange",  0.0, 1.0,   0.25   },
  { SA_BOTAGE_PARAM_PROB_RANGE_LENGTH_MIN,  A,    nullptr,  "Range Length Min", "ProbRange", -1.0, 1.0,  -0.5    },
  { SA_BOTAGE_PARAM_PROB_RANGE_LENGTH_MAX,  A,    nullptr,  "Range Length Max", "ProbRange", -1.0, 1.0,   0.5    },
  { SA_BOTAGE_PARAM_PROB_RANGE_SPEED,       A,    nullptr,  "Range Speed",      "ProbRange",  0.0, 1.0,   0.25   },
  { SA_BOTAGE_PARAM_PROB_RANGE_SPEED_MIN,   A,    nullptr,  "Range Speed Min",  "ProbRange", -1.0, 1.0,  -0.5    },
  { SA_BOTAGE_PARAM_PROB_RANGE_SPEED_MAX,   A,    nullptr,  "Range Speed Max",  "ProbRange", -1.0, 1.0,   0.5    },
  { SA_BOTAGE_PARAM_PROB_RANGE_OFFSET,      A,    nullptr,  "Range Offset",     "ProbRange",  0.0, 1.0,   0.25   },
  { SA_BOTAGE_PARAM_PROB_RANGE_OFFSET_MIN,  A,    nullptr,  "Range Offset Min", "ProbRange", -1.0, 1.0,  -0.5    },
  { SA_BOTAGE_PARAM_PROB_RANGE_OFFSET_MAX,  A,    nullptr,  "Range Offset Max", "ProbRange", -1.0, 1.0,   0.5    },
  { SA_BOTAGE_PARAM_PROB_RANGE_REVERSE,     A,    nullptr,  "Range Reverse",    "ProbRange",  0.0, 1.0,   0.25   },
  { SA_BOTAGE_PARAM_PROB_RANGE_FX,          A,    nullptr,  "Range FX",         "ProbRange",  0.0, 1.0,   0.25   },
  { SA_BOTAGE_PARAM_PROB_RANGE_FX_MIN,      A,    nullptr,  "Range FX Min",     "ProbRange", -1.0, 1.0,  -0.5    },
  { SA_BOTAGE_PARAM_PROB_RANGE_FX_MAX,      A,    nullptr,  "Range FX Max",     "ProbRange", -1.0, 1.0,   0.5    },

  // Probabilities - Loop

  { SA_BOTAGE_PARAM_PROB_LOOP_LENGTH,       A,    nullptr,  "Loop Length",      "ProbLoop",   0.0, 1.0,   0.25   },
  { SA_BOTAGE_PARAM_PROB_LOOP_LENGTH_MIN,   A,    nullptr,  "Loop Length Min",  "ProbLoop",  -1.0, 1.0,  -0.5    },
  { SA_BOTAGE_PARAM_PROB_LOOP_LENGTH_MAX,   A,    nullptr,  "Loop Length Max",  "ProbLoop",  -1.0, 1.0,   0.5    },
  { SA_BOTAGE_PARAM_PROB_LOOP_SPEED,        A,    nullptr,  "Loop Speed",       "ProbLoop",   0.0, 1.0,   0.25   },
  { SA_BOTAGE_PARAM_PROB_LOOP_SPEED_MIN,    A,    nullptr,  "Loop Speed Min",   "ProbLoop",  -1.0, 1.0,  -0.5    },
  { SA_BOTAGE_PARAM_PROB_LOOP_SPEED_MAX,    A,    nullptr,  "Loop Speed Max",   "ProbLoop",  -1.0, 1.0,   0.5    },
  { SA_BOTAGE_PARAM_PROB_LOOP_OFFSET,       A,    nullptr,  "Loop Offset",      "ProbLoop",   0.0, 1.0,   0.25   },
  { SA_BOTAGE_PARAM_PROB_LOOP_OFFSET_MIN,   A,    nullptr,  "Loop Offset Min",  "ProbLoop",  -1.0, 1.0,  -0.5    },
  { SA_BOTAGE_PARAM_PROB_LOOP_OFFSET_MAX,   A,    nullptr,  "Loop Offset Max",  "ProbLoop",  -1.0, 1.0,   0.5    },
  { SA_BOTAGE_PARAM_PROB_LOOP_REVERSE,      A,    nullptr,  "Loop Reverse",     "ProbLoop",   0.0, 1.0,   0.25   },
  { SA_BOTAGE_PARAM_PROB_LOOP_FX,           A,    nullptr,  "Loop FX",          "ProbLoop",   0.0, 1.0,   0.25   },
  { SA_BOTAGE_PARAM_PROB_LOOP_FX_MIN,       A,    nullptr,  "Loop FX Min",      "ProbLoop",  -1.0, 1.0,  -0.5    },
  { SA_BOTAGE_PARAM_PROB_LOOP_FX_MAX,       A,    nullptr,  "Loop FX Max",      "ProbLoop",  -1.0, 1.0,   0.5    },

  // Probabilities - FX

  { SA_BOTAGE_PARAM_PROB_FX1,               A,    nullptr,  "FX1",              "ProbFX",     0.0, 1.0,   0.25   },
  { SA_BOTAGE_PARAM_PROB_FX1_TYPE,          A|SE, nullptr,  "FX1 Type",         "ProbFX",     0,   2,     0      },
  { SA_BOTAGE_PARAM_PROB_FX1_ARG1,          A,    nullptr,  "FX1 Arg1",         "ProbFX",     0.0, 1.0,   0.0    },
  { SA_BOTAGE_PARAM_PROB_FX1_ARG2,          A,    nullptr,  "FX1 Arg1",         "ProbFX",     0.0, 1.0,   0.0    },
  { SA_BOTAGE_PARAM_PROB_FX1_ARG3,          A,    nullptr,  "FX1Arg1",          "ProbFX",     0.0, 1.0,   0.0    },

  { SA_BOTAGE_PARAM_PROB_FX2,               A,    nullptr,  "FX2",              "ProbFX",     0.0, 1.0,   0.25   },
  { SA_BOTAGE_PARAM_PROB_FX2_TYPE,          A|SE, nullptr,  "FX2 Type",         "ProbFX",     0,   2,     0      },
  { SA_BOTAGE_PARAM_PROB_FX2_ARG1,          A,    nullptr,  "FX2 Arg1",         "ProbFX",     0.0, 1.0,   0.0    },
  { SA_BOTAGE_PARAM_PROB_FX2_ARG2,          A,    nullptr,  "FX2 Arg1",         "ProbFX",     0.0, 1.0,   0.0    },
  { SA_BOTAGE_PARAM_PROB_FX2_ARG3,          A,    nullptr,  "FX2 Arg1",         "ProbFX",     0.0, 1.0,   0.0    },

  { SA_BOTAGE_PARAM_PROB_FX3,               A,    nullptr,  "FX3",              "ProbFX",     0.0, 1.0,   0.25   },
  { SA_BOTAGE_PARAM_PROB_FX3_TYPE,          A|SE, nullptr,  "FX3 Type",         "ProbFX",     0,   2,     0      },
  { SA_BOTAGE_PARAM_PROB_FX3_ARG1,          A,    nullptr,  "FX3 Arg1",         "ProbFX",     0.0, 1.0,   0.0    },
  { SA_BOTAGE_PARAM_PROB_FX3_ARG2,          A,    nullptr,  "FX3 Arg1",         "ProbFX",     0.0, 1.0,   0.0    },
  { SA_BOTAGE_PARAM_PROB_FX3_ARG3,          A,    nullptr,  "FX3 Arg1",         "ProbFX",     0.0, 1.0,   0.0    },

  { SA_BOTAGE_PARAM_PROB_FX4,               A,    nullptr,  "FX4",              "ProbFX",     0.0, 1.0,   0.25   },
  { SA_BOTAGE_PARAM_PROB_FX4_TYPE,          A|SE, nullptr,  "FX4 Type",         "ProbFX",     0,   2,     0      },
  { SA_BOTAGE_PARAM_PROB_FX4_ARG1,          A,    nullptr,  "FX4 Arg1",         "ProbFX",     0.0, 1.0,   0.0    },
  { SA_BOTAGE_PARAM_PROB_FX4_ARG2,          A,    nullptr,  "FX4 Arg1",         "ProbFX",     0.0, 1.0,   0.0    },
  { SA_BOTAGE_PARAM_PROB_FX4_ARG3,          A,    nullptr,  "FX4 Arg1",         "ProbFX",     0.0, 1.0,   0.0    },

  { SA_BOTAGE_PARAM_PROB_FX5,               A,    nullptr,  "FX5",              "ProbFX",     0.0, 1.0,   0.25   },
  { SA_BOTAGE_PARAM_PROB_FX5_TYPE,          A|SE, nullptr,  "FX5 Type",         "ProbFX",     0,   2,     0      },
  { SA_BOTAGE_PARAM_PROB_FX5_ARG1,          A,    nullptr,  "FX5 Arg1",         "ProbFX",     0.0, 1.0,   0.0    },
  { SA_BOTAGE_PARAM_PROB_FX5_ARG2,          A,    nullptr,  "FX5 Arg1",         "ProbFX",     0.0, 1.0,   0.0    },
  { SA_BOTAGE_PARAM_PROB_FX5_ARG3,          A,    nullptr,  "FX5 Arg1",         "ProbFX",     0.0, 1.0,   0.0    }

};

//----------------------------------------------------------------------

bool sa_botage_SetupParameters(SAT_Plugin* APlugin) {

  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_GAIN] ));
  // Envelopes
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_ENV_LOOP_ATTACK] ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_ENV_LOOP_DECAY] ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_ENV_SLICE_ATTACK] ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_ENV_SLICE_DECAY] ));
  // Buffer
  APlugin->appendParameter( new SAT_IntParameter(                 &sa_botage_parameter_infos[SA_BOTAGE_PARAM_BUFFER_BEATS] ));
  APlugin->appendParameter( new SAT_IntParameter(                 &sa_botage_parameter_infos[SA_BOTAGE_PARAM_BUFFER_SLICES] ));
  // Trigger
  APlugin->appendParameter( new sa_botage_PercentParameter(       &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_TRIGGER] ));
  APlugin->appendParameter( new SAT_IntParameter(                 &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_TRIGGER_SLICES] ));
  APlugin->appendParameter( new SAT_IntParameter(                 &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_TRIGGER_LOOP] ));
  // Probabilities - Range
  APlugin->appendParameter( new sa_botage_PercentParameter(       &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_RANGE_LENGTH] ));
  APlugin->appendParameter( new sa_botage_ScalePercentParameter(  &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_RANGE_LENGTH_MIN] ));
  APlugin->appendParameter( new sa_botage_ScalePercentParameter(  &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_RANGE_LENGTH_MAX] ));
  APlugin->appendParameter( new sa_botage_PercentParameter(       &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_RANGE_SPEED] ));
  APlugin->appendParameter( new sa_botage_ScalePercentParameter(  &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_RANGE_SPEED_MIN] ));
  APlugin->appendParameter( new sa_botage_ScalePercentParameter(  &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_RANGE_SPEED_MAX] ));
  APlugin->appendParameter( new sa_botage_PercentParameter(       &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_RANGE_OFFSET] ));
  APlugin->appendParameter( new sa_botage_OffsetParameter(        &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_RANGE_OFFSET_MIN] ));
  APlugin->appendParameter( new sa_botage_OffsetParameter(        &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_RANGE_OFFSET_MAX] ));
  APlugin->appendParameter( new sa_botage_PercentParameter(       &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_RANGE_REVERSE] ));
  APlugin->appendParameter( new sa_botage_PercentParameter(       &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_RANGE_FX] ));
  APlugin->appendParameter( new sa_botage_ScalePercentParameter(  &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_RANGE_FX_MIN] ));
  APlugin->appendParameter( new sa_botage_ScalePercentParameter(  &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_RANGE_FX_MAX] ));
  // Probabilities - Loop
  APlugin->appendParameter( new sa_botage_PercentParameter(       &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_LOOP_LENGTH] ));
  APlugin->appendParameter( new sa_botage_ScalePercentParameter(  &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_LOOP_LENGTH_MIN] ));
  APlugin->appendParameter( new sa_botage_ScalePercentParameter(  &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_LOOP_LENGTH_MAX] ));
  APlugin->appendParameter( new sa_botage_PercentParameter(       &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_LOOP_SPEED] ));
  APlugin->appendParameter( new sa_botage_ScalePercentParameter(  &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_LOOP_SPEED_MIN] ));
  APlugin->appendParameter( new sa_botage_ScalePercentParameter(  &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_LOOP_SPEED_MAX] ));
  APlugin->appendParameter( new sa_botage_PercentParameter(       &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_LOOP_OFFSET] ));
  APlugin->appendParameter( new sa_botage_OffsetParameter(        &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_LOOP_OFFSET_MIN] ));
  APlugin->appendParameter( new sa_botage_OffsetParameter(        &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_LOOP_OFFSET_MAX] ));
  APlugin->appendParameter( new sa_botage_PercentParameter(       &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_LOOP_REVERSE] ));
  APlugin->appendParameter( new sa_botage_PercentParameter(       &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_LOOP_FX] ));
  APlugin->appendParameter( new sa_botage_ScalePercentParameter(  &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_LOOP_FX_MIN] ));
  APlugin->appendParameter( new sa_botage_ScalePercentParameter(  &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_LOOP_FX_MAX] ));
  // Probabilities - FX
  APlugin->appendParameter( new sa_botage_PercentParameter(       &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX1] ));
  APlugin->appendParameter( new SAT_TextParameter(                &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX1_TYPE], fx_type_text ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX1_ARG1] ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX1_ARG2] ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX1_ARG3] ));
  APlugin->appendParameter( new sa_botage_PercentParameter(       &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX2] ));
  APlugin->appendParameter( new SAT_TextParameter(                &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX2_TYPE], fx_type_text ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX2_ARG1] ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX2_ARG2] ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX2_ARG3] ));
  APlugin->appendParameter( new sa_botage_PercentParameter(       &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX3] ));
  APlugin->appendParameter( new SAT_TextParameter(                &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX3_TYPE], fx_type_text ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX3_ARG1] ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX3_ARG2] ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX3_ARG3] ));
  APlugin->appendParameter( new sa_botage_PercentParameter(       &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX4] ));
  APlugin->appendParameter( new SAT_TextParameter(                &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX4_TYPE], fx_type_text ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX4_ARG1] ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX4_ARG2] ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX4_ARG3] ));
  APlugin->appendParameter( new sa_botage_PercentParameter(       &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX5] ));
  APlugin->appendParameter( new SAT_TextParameter(                &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX5_TYPE], fx_type_text ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX5_ARG1] ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX5_ARG2] ));
  APlugin->appendParameter( new SAT_Parameter(                    &sa_botage_parameter_infos[SA_BOTAGE_PARAM_PROB_FX5_ARG3] ));
  return true;
}

//----------------------------------------------------------------------
#endif

