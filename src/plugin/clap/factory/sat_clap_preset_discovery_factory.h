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

  ---

  TODO: registry
        preset provider in registry? (pprovider data, preset indexer)
*/

#include "plugin/clap/sat_clap.h"


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


  /*
  struct clap_preset_discovery_indexer {
    clap_version_t clap_version;
    const char *name;
    const char *vendor;
    const char *url;
    const char *version;
    void       *indexer_data;
    bool declare_filetype     (const struct clap_preset_discovery_indexer *indexer, const clap_preset_discovery_filetype_t *filetype);
    bool declare_location     (const struct clap_preset_discovery_indexer *indexer, const clap_preset_discovery_location_t *location);
    bool declare_soundpack    (const struct clap_preset_discovery_indexer *indexer, const clap_preset_discovery_soundpack_t *soundpack);
    const void *get_extension (const struct clap_preset_discovery_indexer *indexer,
  };
  */


// !!! todo: use SAT_REGISTRY
const clap_preset_discovery_indexer_t* SAT_GLOBAL_CLAP_PRESET_INDEXER = nullptr;

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
    const clap_preset_discovery_filetype_t filetype = {
      "skei.audio preset",  //const char *name;
      "(description)",      //"const char *description; // optional
      "SA_Preset"           // file extension
    };
    const clap_preset_discovery_location_t location = {
      CLAP_PRESET_DISCOVERY_IS_USER_CONTENT,    //uint32_t flags;
      "skei.audio Presets",                     //const char *name;
      CLAP_PRESET_DISCOVERY_LOCATION_FILE,      //uint32_t kind;
      "/home/skei/Code/SA_Toolkit/bin/presets"  //const char *location;
    };
    SAT_GLOBAL_CLAP_PRESET_INDEXER->declare_location(SAT_GLOBAL_CLAP_PRESET_INDEXER,&location);
    SAT_GLOBAL_CLAP_PRESET_INDEXER->declare_filetype(SAT_GLOBAL_CLAP_PRESET_INDEXER,&filetype);
    //SAT_GLOBAL_CLAP_PRESET_INDEXER->declare_soundpack(SAT_GLOBAL_CLAP_PRESET_INDEXER,&soundpack);
    //void* ptr = SAT_GLOBAL_CLAP_PRESET_INDEXER->get_extension(SAT_GLOBAL_CLAP_PRESET_INDEXER,"");
    return true;
  }

  //----------

  // Destroys the preset provider

  void clap_preset_discovery_provider_destroy_callback(const struct clap_preset_discovery_provider *provider) {
    SAT_PRINT;
  }

  //----------

  // reads metadata from the given file and passes them to the metadata receiver

  /*
  struct clap_preset_discovery_metadata_receiver {
     void *receiver_data;
     void on_error         (const struct clap_preset_discovery_metadata_receiver *receiver, int32_t os_error, const char *error_message);
     bool begin_preset     (const struct clap_preset_discovery_metadata_receiver *receiver, const char *name, const char *load_key);
     void add_plugin_id    (const struct clap_preset_discovery_metadata_receiver *receiver, const clap_plugin_id_t *plugin_id);
     void set_soundpack_id (const struct clap_preset_discovery_metadata_receiver *receiver, const char *soundpack_id);
     void set_flags        (const struct clap_preset_discovery_metadata_receiver *receiver, uint32_t flags);
     void add_creator      (const struct clap_preset_discovery_metadata_receiver *receiver, const char *creator);
     void set_description  (const struct clap_preset_discovery_metadata_receiver *receiver, const char *description);
     void set_timestamps   (const struct clap_preset_discovery_metadata_receiver *receiver, clap_timestamp_t creation_time, clap_timestamp_t modification_time);
     void add_feature      (const struct clap_preset_discovery_metadata_receiver *receiver, const char *feature);
     void add_extra_info   (const struct clap_preset_discovery_metadata_receiver *receiver, const char *key, const char *value);
  };
  */
  
  bool clap_preset_discovery_provider_get_metadata_callback(const struct clap_preset_discovery_provider *provider, uint32_t location_kind, const char *location, const clap_preset_discovery_metadata_receiver_t *metadata_receiver) {
    SAT_PRINT;
    switch (location_kind) {
      
      // The preset are located in a file on the OS filesystem.
      // The location is then a path which works with the OS file system functions (open, stat, ...)
      // So both '/' and '\' shall work on Windows as a separator.

      case CLAP_PRESET_DISCOVERY_LOCATION_FILE: {
        SAT_Print("location: %s\n",location);
        if (metadata_receiver->begin_preset(metadata_receiver,"test1","testkey")) {
          SAT_PRINT;
          clap_plugin_id_t id = { "clap", SAT_VENDOR "/test1_synth (debug)/" SAT_VERSION };
          metadata_receiver->add_plugin_id(metadata_receiver,&id);
          metadata_receiver->set_flags(metadata_receiver,CLAP_PRESET_DISCOVERY_IS_USER_CONTENT);
          metadata_receiver->add_creator(metadata_receiver,"testcreator");
          metadata_receiver->set_description(metadata_receiver,"A really nice sound!");
          metadata_receiver->add_feature(metadata_receiver,"poly");
          metadata_receiver->add_extra_info(metadata_receiver,"key","value");
        }
        return true;
      }
      
      // The preset is bundled within the plugin DSO itself.
      // The location must then be null, as the preset are within the plugin itsel and then the plugin
      // will act as a preset container.
      
      case CLAP_PRESET_DISCOVERY_LOCATION_PLUGIN: {
        SAT_PRINT;
        break;
      }
      
    }
    // read from file (location)
    // call metadata_provider-> ...
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

  const clap_preset_discovery_provider_t SAT_CLAP_PRESET_DISCOVERY_PROVIDER = {
    .desc           = &SAT_CLAP_PRESET_DISCOVERY_PROVIDER_DESCRIPTOR,//nullptr,
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
    if (index == 0) { 
      return &SAT_CLAP_PRESET_DISCOVERY_PROVIDER_DESCRIPTOR;
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
    SAT_Print("provider_id %s\n",provider_id);
    SAT_Print("* indexer.name:    %s\n",indexer->name);
    SAT_Print("* indexer.vendor:  %s\n",indexer->vendor);
    SAT_Print("* indexer.url:     %s\n",indexer->url);
    SAT_Print("* indexer.version: %s\n",indexer->version);
    
    // !!!!!
    SAT_GLOBAL_CLAP_PRESET_INDEXER = indexer;
    
    if (strcmp(provider_id,SAT_CLAP_PRESET_DISCOVERY_PROVIDER_DESCRIPTOR.id) == 0) {
      SAT_Print("yes\n");
      return  &SAT_CLAP_PRESET_DISCOVERY_PROVIDER;
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

