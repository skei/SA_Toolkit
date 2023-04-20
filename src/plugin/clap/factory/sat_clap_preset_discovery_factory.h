#ifndef sat_clap_preset_discovery_factory_incuded
#define sat_clap_preset_discovery_factory_incuded
//----------------------------------------------------------------------

#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

  const clap_preset_discovery_location_t test_synth_preset_location = {
    .flags    = 0,//CLAP_PRESET_DISCOVERY_IS_USER_CONTENT,
    .name     = "test_synth_presets",
    .kind     = CLAP_PRESET_DISCOVERY_LOCATION_FILE,
    .location = "/home/skei/Code/SA_Toolkit/bin/presets"
  };
  
  //----------

  const clap_preset_discovery_filetype_t test_synth_preset_filetype = {
    .name           = "test_synth_preset",
    .description    = "preset for test_synth", // optional
    .file_extension = "sa_preset"
  };

  //----------
  
  //char test_synth_plugin_id_buffer[SAT_MAX_NAME_LENGTH] = {0};

  const clap_plugin_id_t test_synth_plugin_id = {
    .abi  = "clap",                   // The plugin ABI name, in lowercase. eg: "clap"
    .id   = test_synth_descriptor.id  // The plugin ID, for example "com.u-he.Diva". If the ABI rely upon binary plugin ids, then they shall be hex encoded (lower case).            
    //.id   = test_synth_plugin_id_buffer
  };


//----------------------------------------------------------------------
//
// provider
//
//----------------------------------------------------------------------

  const clap_preset_discovery_provider_descriptor_t SAT_CLAP_PRESET_DISCOVERY_PROVIDER_DESCRIPTOR = {
    .clap_version = CLAP_VERSION,
    .id           = "test_synth_preset_provider/1",
    .name         = "test_synth preset provider",
    .vendor       = SAT_VENDOR
  };

  //----------------------------------------------------------------------

  /*
    Initialize the preset provider.
    It should declare all its locations, filetypes and sound packs.
    Returns false if initialization failed.
  */

  bool clap_preset_discovery_provider_init_callback(const struct clap_preset_discovery_provider *provider) {
    SAT_PRINT;
    const clap_preset_discovery_indexer_t* indexer = (const clap_preset_discovery_indexer_t*)provider->provider_data;
    if (!indexer) {
      SAT_Print("indexer == null\n");
      return false;
    }
    if (!indexer->declare_filetype(indexer,&test_synth_preset_filetype)) {
      SAT_Print("Oops! indexer->declare_filetype returned false\n");
      return false;
    }
    if (!indexer->declare_location(indexer,&test_synth_preset_location)) {
      SAT_Print("oops! indexer->declare_location returned false\n");
      return false;
    }
    //indexer->declare_soundpack(indexer,&soundpack);
    //void* ptr = indexer->get_extension(indexer,"");
    SAT_Print("ok\n");
    return true;
  }

  //----------

  /*
    Destroys the preset provider
  */

  void clap_preset_discovery_provider_destroy_callback(const struct clap_preset_discovery_provider *provider) {
    SAT_PRINT;
  }

  //----------
  
  /*
    reads metadata from the given file and passes them to the metadata receiver
  */

  bool clap_preset_discovery_provider_get_metadata_callback(const struct clap_preset_discovery_provider *provider, uint32_t location_kind, const char *location, const clap_preset_discovery_metadata_receiver_t *metadata_receiver) {
    
    SAT_Print("CLAP_PRESET_DISCOVERY_LOCATION_FILE location: '%s'\n",location);
    SAT_Print("(plugin_id: '%s')\n",test_synth_plugin_id.id);

    switch (location_kind) {
      
      /*
        The preset are located in a file on the OS filesystem.
        The location is then a path which works with the OS file system functions (open, stat, ...)
        So both '/' and '\' shall work on Windows as a separator.
      */
      
      case CLAP_PRESET_DISCOVERY_LOCATION_FILE: {

        const char* filename = SAT_GetFilenameFromPath(location);

        //char name[SAT_MAX_PATH_LENGTH] = {0};
        //strcpy(name,filename);
        //SAT_StripFileExt(name);
        //strcpy(test_synth_plugin_id_buffer,"plugin://");
        //strcat(test_synth_plugin_id_buffer,test_synth_descriptor.id);
        //SAT_Print("preset: '%s'\n",name);
        
        /*
          This must be called for every preset in the file and before any preset metadata is sent with the calls below.
          If the preset file is a preset container then name and load_key are mandatory, otherwise they are optional.
          The load_key is a machine friendly string used to load the preset inside the container via a
          the preset-load plug-in extension. The load_key can also just be the subpath if that's what
          the plugin wants but it could also be some other unique id like a database primary key or a
          binary offset. It's use is entirely up to the plug-in.
          If the function returns false, the the provider must stop calling back into the receiver.
        */

        if (metadata_receiver->begin_preset(metadata_receiver,filename,location)) {
        //if (metadata_receiver->begin_preset(metadata_receiver,nullptr,nullptr)) {
          
          /*
            Adds a plug-in id that this preset can be used with.
          */
          
          metadata_receiver->add_plugin_id(metadata_receiver,&test_synth_plugin_id);
          
          /*
            Sets the sound pack to which the preset belongs to.
          */
          
          //metadata_receiver->set_soundpack_id(metadata_receiver,"");

          /*
            Sets the flags, see clap_preset_discovery_flags.
            If unset, they are then inherited from the location.
          */
          
//          metadata_receiver->set_flags(metadata_receiver,CLAP_PRESET_DISCOVERY_IS_USER_CONTENT);
          
          /*
            Adds a creator name for the preset.
          */
          
          metadata_receiver->add_creator(metadata_receiver,"tor.helge.skei");
          
          /*
            Sets a description of the preset.          
          */
          
          metadata_receiver->set_description(metadata_receiver,"..description..");
          
          /*
            Sets the creation time and last modification time of the preset.
            If one of the times isn't known, set it to CLAP_TIMESTAMP_UNKNOWN.
            If this function is not called, then the indexer may look at the file's creation and
            modification time.
          */
          
          metadata_receiver->set_timestamps(metadata_receiver,CLAP_TIMESTAMP_UNKNOWN,CLAP_TIMESTAMP_UNKNOWN);
          
          /*
            Adds a feature to the preset.
            The feature string is arbitrary, it is the indexer's job to understand it and remap it to its
            internal categorization and tagging system.
            However, the strings from plugin-features.h should be understood by the indexer and one of the
            plugin category could be provided to determine if the preset will result into an audio-effect,
            instrument, ...
            Examples:
            kick, drum, tom, snare, clap, cymbal, bass, lead, metalic, hardsync, crossmod, acid,
            distorted, drone, pad, dirty, etc...
          */
          
          metadata_receiver->add_feature(metadata_receiver,"lead");
          
          /*
            Adds extra information to the metadata.
          */
          
          metadata_receiver->add_extra_info(metadata_receiver,"key","value");
          
          return true;
          
        }
        else {
          SAT_Print("metadata_receiver->begin_preset(%p,%s,%s) returned false\n",metadata_receiver,filename,location);
          return false;
        }
        
      }
      
      /*
        The preset is bundled within the plugin DSO itself.
        The location must then be null, as the preset are within the plugin itsel and then the plugin
        will act as a preset container.
      */
      
      case CLAP_PRESET_DISCOVERY_LOCATION_PLUGIN: {
        SAT_Print("CLAP_PRESET_DISCOVERY_LOCATION_PLUGIN\n");
        return false;
      }
      
      default: {
        SAT_Print("unknown location_kind: %i\n",location_kind);
        return false;
      }
      
      
    } // switch
    
    return false;
  }

  //----------

  /*
    Query an extension.
    The returned pointer is owned by the provider.
    It is forbidden to call it before provider->init().
    You can call it within provider->init() call, and after.
  */

  const void* clap_preset_discovery_provider_get_extension_callback(const struct clap_preset_discovery_provider *provider, const char *extension_id) {
    SAT_Print("%s\n",extension_id);
    return nullptr;
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

  /*
    Get the number of preset providers available.
    [thread-safe]
  */

  uint32_t clap_preset_discovery_factory_count_callback(const struct clap_preset_discovery_factory *factory) {
    SAT_PRINT;
    return 1;
  }

  //----------

  /*
    Retrieves a preset provider descriptor by its index.
    Returns null in case of error.
    The descriptor must not be freed.
    [thread-safe]
  */

  const clap_preset_discovery_provider_descriptor_t* clap_preset_discovery_factory_get_descriptor_callback(const struct clap_preset_discovery_factory *factory, uint32_t index) {
    SAT_Print("index %i\n",index);
    switch (index) {
      case 0: return &SAT_CLAP_PRESET_DISCOVERY_PROVIDER_DESCRIPTOR;
    }
    return nullptr;
  }

  //----------

  /*
    Create a preset provider by its id.
    The returned pointer must be freed by calling preset_provider->destroy(preset_provider);
    The preset provider is not allowed to use the indexer callbacks in the create method.
    It is forbidden to call back into the indexer before the indexer calls provider->init().
    Returns null in case of error.
    [thread-safe]
  */

  const clap_preset_discovery_provider_t* clap_preset_discovery_factory_create_callback( const struct clap_preset_discovery_factory *factory, const clap_preset_discovery_indexer_t *indexer, const char *provider_id) {
    SAT_Print( "provider_id '%s'\n",provider_id);
    SAT_Print("  SAT_CLAP_PRESET_DISCOVERY_PROVIDER_DESCRIPTOR.id: '%s')\n",SAT_CLAP_PRESET_DISCOVERY_PROVIDER_DESCRIPTOR.id);
    SAT_Print("  indexer.name:    '%s'\n",indexer->name);
    SAT_Print("  indexer.vendor:  '%s'\n",indexer->vendor);
    SAT_Print("  indexer.url:     '%s'\n",indexer->url);
    SAT_Print("  indexer.version: '%s'\n",indexer->version);
    if (strcmp(provider_id,SAT_CLAP_PRESET_DISCOVERY_PROVIDER_DESCRIPTOR.id) == 0) {
      SAT_CLAP_PRESET_DISCOVERY_PROVIDER.provider_data = (void*)indexer;
      return &SAT_CLAP_PRESET_DISCOVERY_PROVIDER;
    }
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

