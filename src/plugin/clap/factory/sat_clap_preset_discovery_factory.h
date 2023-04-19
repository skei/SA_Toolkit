#ifndef sat_clap_preset_discovery_factory_incuded
#define sat_clap_preset_discovery_factory_incuded
//----------------------------------------------------------------------

/*
  Preset Discovery enables a plug-in host to identify where presets are found, what
  extensions they have, which plug-ins they apply to, and other metadata associated with the
  presets so that they can be indexed and searched for quickly within the plug-in host's browser.

  This has a number of advantages for the user:
  - it allows them to browse for presets from one central location in a consistent way
  - the user can browse for presets without having to commit to a particular plug-in first

  The API works as follow to index presets and presets metadata:
  1. clap_plugin_entry.get_factory(CLAP_PRESET_DISCOVERY_FACTORY_ID)
  2. clap_preset_discovery_factory_t.create(...)
  3. clap_preset_discovery_provider.init() (only necessary the first time, declarations can be cached)
    `-> clap_preset_discovery_indexer.declare_filetype()
    `-> clap_preset_discovery_indexer.declare_location()
    `-> clap_preset_discovery_indexer.declare_soundpack() (optional)
    `-> clap_preset_discovery_indexer.set_invalidation_watch_file() (optional)
  4. crawl the given locations and monitor file system changes
    `-> clap_preset_discovery_indexer.get_metadata() for each presets files

  Then to load a preset, use ext/draft/preset-load.h.

*/

#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------
//
// provider
//
//----------------------------------------------------------------------

  const clap_preset_discovery_provider_descriptor_t SAT_CLAP_PRESET_DISCOVERY_PROVIDER_DESCRIPTOR = {
    .clap_version = CLAP_VERSION,
    .id           = "skei.audio/SAPresetProvider/0",
    .name         = "SAPresetProvider",
    .vendor       = "skei.audio"
  };

  //----------------------------------------------------------------------

  // Initialize the preset provider.
  // It should declare all its locations, filetypes and sound packs.
  // Returns false if initialization failed.

  bool clap_preset_discovery_provider_init_callback(const struct clap_preset_discovery_provider *provider) {
    SAT_PRINT;
    const clap_preset_discovery_indexer_t* indexer = (const clap_preset_discovery_indexer_t*)provider->provider_data;
    const clap_preset_discovery_location_t location = {
      CLAP_PRESET_DISCOVERY_IS_USER_CONTENT,    // uint32_t flags;
      "test_synth presets",                     // const char *name;
      CLAP_PRESET_DISCOVERY_LOCATION_FILE,      // uint32_t kind;
      "/home/skei/Code/SA_Toolkit/bin/presets"  // const char *location;
    };
    if (!indexer->declare_location(indexer,&location)) {
      SAT_Print("OUCH! indexer->declare_location returned false\n");
    }
    const clap_preset_discovery_filetype_t filetype = {
      "test_synth preset",              // const char *name;
      "preset for test_synth (debug)",  // "const char *description; // optional
      "sap"                             // file extension
    };
    if (!indexer->declare_filetype(indexer,&filetype)) {
      SAT_Print("OUCH! indexer->declare_filetype returned false\n");
    }
    //indexer->declare_soundpack(indexer,&soundpack);
    //void* ptr = indexer->get_extension(indexer,"");
    return true;
  }

  //----------

  // Destroys the preset provider

  void clap_preset_discovery_provider_destroy_callback(const struct clap_preset_discovery_provider *provider) {
    SAT_PRINT;
  }

  //----------
  
  // reads metadata from the given file and passes them to the metadata receiver

  bool clap_preset_discovery_provider_get_metadata_callback(const struct clap_preset_discovery_provider *provider, uint32_t location_kind, const char *location, const clap_preset_discovery_metadata_receiver_t *metadata_receiver) {
    SAT_PRINT;
    //const clap_preset_discovery_indexer_t* indexer = (const clap_preset_discovery_indexer_t*)provider->provider_data;
    
    switch (location_kind) {
      
      // The preset are located in a file on the OS filesystem.
      // The location is then a path which works with the OS file system functions (open, stat, ...)
      // So both '/' and '\' shall work on Windows as a separator.

      case CLAP_PRESET_DISCOVERY_LOCATION_FILE: {
        SAT_Print("CLAP_PRESET_DISCOVERY_LOCATION_FILE location: '%s'\n",location);
        //metadata_receiver->on_error(metadata_receiver,0,"error");
        if (metadata_receiver->begin_preset(metadata_receiver,"name","key")) {
          clap_plugin_id_t id = {
            .abi = "clap",                                        // The plugin ABI name, in lowercase. eg: "clap"
            .id = SAT_VENDOR "/test_synth (debug)/" SAT_VERSION   // The plugin ID, for example "com.u-he.Diva". If the ABI rely upon binary plugin ids, then they shall be hex encoded (lower case).            
          };
          metadata_receiver->add_plugin_id(metadata_receiver,&id);
          //metadata_receiver->set_soundpack_id(metadata_receiver,"");
          metadata_receiver->set_flags(metadata_receiver,CLAP_PRESET_DISCOVERY_IS_FACTORY_CONTENT);
          metadata_receiver->add_creator(metadata_receiver,"test_synth_creator");
          metadata_receiver->set_description(metadata_receiver,"A nice sound!");
          //metadata_receiver->add_feature(metadata_receiver,"poly");
          metadata_receiver->set_timestamps(metadata_receiver,CLAP_TIMESTAMP_UNKNOWN,CLAP_TIMESTAMP_UNKNOWN);
          //metadata_receiver->add_extra_info(metadata_receiver,"key","value");
        }
        return true;
      }
      
      // The preset is bundled within the plugin DSO itself.
      // The location must then be null, as the preset are within the plugin itsel and then the plugin
      // will act as a preset container.
      
      case CLAP_PRESET_DISCOVERY_LOCATION_PLUGIN: {
        SAT_Print("CLAP_PRESET_DISCOVERY_LOCATION_PLUGIN\n");
        return false;
      }
      
    }
    
    return false;
  }

  //----------

  // Query an extension.
  // The returned pointer is owned by the provider.
  // It is forbidden to call it before provider->init().
  // You can call it within provider->init() call, and after.

  const void* clap_preset_discovery_provider_get_extension_callback(const struct clap_preset_discovery_provider *provider, const char *extension_id) {
    SAT_PRINT;
    return "SA_Preset";
  }

  //----------------------------------------------------------------------

  //const
  clap_preset_discovery_provider_t SAT_CLAP_PRESET_DISCOVERY_PROVIDER = {
    .desc           = &SAT_CLAP_PRESET_DISCOVERY_PROVIDER_DESCRIPTOR,
    .provider_data  = nullptr,
    .init           = clap_preset_discovery_provider_init_callback,
    .destroy        = clap_preset_discovery_provider_destroy_callback,
    .get_metadata   = clap_preset_discovery_provider_get_metadata_callback,
    .get_extension  = clap_preset_discovery_provider_get_extension_callback
  };

//----------------------------------------------------------------------
//
// factory
//
//----------------------------------------------------------------------

  // Get the number of preset providers available.
  // [thread-safe]

  uint32_t clap_preset_discovery_factory_count_callback(const struct clap_preset_discovery_factory *factory) {
    SAT_PRINT;
    return 1;
  }

  //----------

  // Retrieves a preset provider descriptor by its index.
  // Returns null in case of error.
  // The descriptor must not be freed.
  // [thread-safe]

  const clap_preset_discovery_provider_descriptor_t* clap_preset_discovery_factory_get_descriptor_callback(const struct clap_preset_discovery_factory *factory, uint32_t index) {
    SAT_Print("index %i\n",index);
    switch (index) {
      case 0: return &SAT_CLAP_PRESET_DISCOVERY_PROVIDER_DESCRIPTOR;
    }
    return nullptr;
  }

  //----------

  // Create a preset provider by its id.
  // The returned pointer must be freed by calling preset_provider->destroy(preset_provider);
  // The preset provider is not allowed to use the indexer callbacks in the create method.
  // It is forbidden to call back into the indexer before the indexer calls provider->init().
  // Returns null in case of error.
  // [thread-safe]

  const clap_preset_discovery_provider_t* clap_preset_discovery_factory_create_callback( const struct clap_preset_discovery_factory *factory, const clap_preset_discovery_indexer_t *indexer, const char *provider_id) {
    SAT_Print( "provider_id '%s'\n",provider_id);
    SAT_DPrint("  SAT_CLAP_PRESET_DISCOVERY_PROVIDER_DESCRIPTOR.id: '%s')\n",SAT_CLAP_PRESET_DISCOVERY_PROVIDER_DESCRIPTOR.id);
    SAT_DPrint("  indexer.name:    '%s'\n",indexer->name);
    SAT_DPrint("  indexer.vendor:  '%s'\n",indexer->vendor);
    SAT_DPrint("  indexer.url:     '%s'\n",indexer->url);
    SAT_DPrint("  indexer.version: '%s'\n",indexer->version);
    if (strcmp(provider_id,SAT_CLAP_PRESET_DISCOVERY_PROVIDER_DESCRIPTOR.id) == 0) {
      SAT_CLAP_PRESET_DISCOVERY_PROVIDER.provider_data = (void*)indexer;
      return &SAT_CLAP_PRESET_DISCOVERY_PROVIDER;
    }
    SAT_Print("no\n");
    return nullptr;
  }

  //----------------------------------------------------------------------

  const clap_preset_discovery_factory_t SAT_CLAP_PRESET_DISCOVERY_FACTORY {
    .count          = clap_preset_discovery_factory_count_callback,
    .get_descriptor = clap_preset_discovery_factory_get_descriptor_callback,
    .create         = clap_preset_discovery_factory_create_callback
  };

//----------------------------------------------------------------------
#endif

