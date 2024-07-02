#ifndef sa_demo_parameters_included
#define sa_demo_parameters_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/lib/sat_clap.h"

//----------------------------------------------------------------------

enum sa_demo_parameter_e {
  SA_DEMO_PARAM_COUNT
};

//----------------------------------------------------------------------

#define A   CLAP_PARAM_IS_AUTOMATABLE
#define M   CLAP_PARAM_IS_MODULATABLE
#define MN (CLAP_PARAM_IS_MODULATABLE | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID)
#define S   CLAP_PARAM_IS_STEPPED
#define SE (CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_ENUM )
#define H   CLAP_PARAM_IS_HIDDEN

//----------

// clap_param_info_t sa_mael_Parameters[SA_MAEL_PARAM_COUNT] = {
// }

//----------

bool sa_demo_SetupParameters(SAT_Plugin* APlugin) {
  // APlugin->appendParameter( new SAT_Parameter( &sa_demo_Parameters[SA_DEMO_PARAM_GAIN] ));
  return true;
}

//----------------------------------------------------------------------
#endif