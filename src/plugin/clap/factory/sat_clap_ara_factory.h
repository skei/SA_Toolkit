#ifndef sat_clap_ara_factory_included
#define sat_clap_ara_factory_included
//----------------------------------------------------------------------

#include "extern/ara/ARACLAP.h"

#if 0

ARA_DRAFT typedef struct clap_ara_factory {
  //! Get the number of ARA factories (i.e. ARA-capable plug.-ins) available.
  //! Note that the regular clap_plugin_factory can contain more plug-ins if these do not support
  //! ARA - make no assumption about items returned here being related to the items returned there
  //! in terms of count or order.
  
  uint32_t(CLAP_ABI *get_factory_count)(const struct clap_ara_factory *factory);

  //! Get the ARA factory for the plug-in at the given index.
  //! The returned pointer must remain valid until clap_plugin_entry_t.deinit() is called.
  //! The returned ARAFactory must be equal to the ARAFactory returned from instances of the
  //! associated CLAP plug-in through their clap_ara_plugin_extension_t.get_factory().
  
  const ARA_NAMESPACE ARAFactory *(CLAP_ABI *get_ara_factory)(const struct clap_ara_factory *factory, uint32_t index);

  //! Get the ID of the CLAP plug-in associated with the ARA factory for the given index.
  //! The plug-in must be in the same binary.
  //! The returned pointer must remain valid until clap_plugin_entry_t.deinit is called.
  
  const char *(CLAP_ABI *get_plugin_id)(const struct clap_ara_factory *factory, uint32_t index);
    
} clap_ara_factory_t;

#endif

// todo: clap_ara_plugin_extension (sat_clap_plugin)


//----------------------------------------------------------------------
#endif