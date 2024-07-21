#ifndef sa_demo_parameters_included
#define sa_demo_parameters_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------

enum sa_demo_parameter_e {
  SA_DEMO_PARAM_P1  = 0,
  SA_DEMO_PARAM_P2,
  SA_DEMO_PARAM_P3,
  SA_DEMO_PARAM_P4,
  SA_DEMO_PARAM_P5,
  SA_DEMO_PARAM_P6,
  SA_DEMO_PARAM_P7,
  SA_DEMO_PARAM_P8,
  SA_DEMO_PARAM_COUNT
};

//----------------------------------------------------------------------

#define A   CLAP_PARAM_IS_AUTOMATABLE
#define M   CLAP_PARAM_IS_MODULATABLE
#define MN (CLAP_PARAM_IS_MODULATABLE | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID)
#define S   CLAP_PARAM_IS_STEPPED
#define SE (CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_ENUM )
#define H   CLAP_PARAM_IS_HIDDEN

//const
clap_param_info_t sa_demo_parameters[SA_DEMO_PARAM_COUNT] = {
  { SA_DEMO_PARAM_P1, A|M, nullptr, "P1", "", 0,1, 0 },
  { SA_DEMO_PARAM_P2, A|M, nullptr, "P2", "", 0,1, 0 },
  { SA_DEMO_PARAM_P3, A|M, nullptr, "P3", "", 0,1, 0 },
  { SA_DEMO_PARAM_P4, A|M, nullptr, "P4", "", 0,1, 0 },
  { SA_DEMO_PARAM_P5, A|M, nullptr, "P5", "", 0,1, 0 },
  { SA_DEMO_PARAM_P6, A|M, nullptr, "P6", "", 0,1, 0 },
  { SA_DEMO_PARAM_P7, A|M, nullptr, "P7", "", 0,1, 0 },
  { SA_DEMO_PARAM_P8, A|M, nullptr, "P8", "", 0,1, 0 }
};

#undef A
#undef M
#undef MN
#undef S
#undef SE
#undef H


//----------

bool sa_demo_SetupParameters(SAT_Plugin* APlugin) {
  // for (uint32_t i=0; i<SA_DEMO_PARAM_COUNT; i++) {
  //   APlugin->appendParameter( new SAT_Parameter( &sa_demo_parameters[i] ));
  // }
  APlugin->appendParameter( new SAT_Parameter( &sa_demo_parameters[SA_DEMO_PARAM_P1] ));
  APlugin->appendParameter( new SAT_Parameter( &sa_demo_parameters[SA_DEMO_PARAM_P2] ));
  APlugin->appendParameter( new SAT_Parameter( &sa_demo_parameters[SA_DEMO_PARAM_P3] ));
  APlugin->appendParameter( new SAT_Parameter( &sa_demo_parameters[SA_DEMO_PARAM_P4] ));
  APlugin->appendParameter( new SAT_Parameter( &sa_demo_parameters[SA_DEMO_PARAM_P5] ));
  APlugin->appendParameter( new SAT_Parameter( &sa_demo_parameters[SA_DEMO_PARAM_P6] ));
  APlugin->appendParameter( new SAT_Parameter( &sa_demo_parameters[SA_DEMO_PARAM_P7] ));
  APlugin->appendParameter( new SAT_Parameter( &sa_demo_parameters[SA_DEMO_PARAM_P8] ));
  return true;
}

//----------------------------------------------------------------------
#endif