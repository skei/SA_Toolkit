#ifndef sat_clap_ara_factory_included
#define sat_clap_ara_factory_included
//----------------------------------------------------------------------

#include "extern/ara/ARACLAP.h"

#if 0

ARA_DRAFT typedef struct clap_ara_factory {
  uint32_t(CLAP_ABI *get_factory_count)(const struct clap_ara_factory *factory);
  const ARA_NAMESPACE ARAFactory *(CLAP_ABI *get_ara_factory)(const struct clap_ara_factory *factory, uint32_t index);
  const char *(CLAP_ABI *get_plugin_id)(const struct clap_ara_factory *factory, uint32_t index);
} clap_ara_factory_t;

#endif

// todo: clap_ara_plugin_extension (sat_clap_plugin)

//----------------------------------------------------------------------
#endif