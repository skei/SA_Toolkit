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
*/

#include "plugin/clap/sat_clap.h"


//----------------------------------------------------------------------
//
// provider
//
//----------------------------------------------------------------------

const clap_preset_discovery_provider_descriptor_t SAT_CLAP_PRESET_DISCOVERY_PROVIDER_DESCRIPTOR = {
  .clap_version = CLAP_VERSION,
  .id           = "me/myPresetDiscoveryProvider/0",
  .name         = "myPresetDiscoveryProvider",
  .vendor       = "me"
};

//----------------------------------------------------------------------

// Initialize the preset provider.
// It should declare all its locations, filetypes and sound packs.
// Returns false if initialization failed.

bool clap_preset_discovery_provider_init_callback(const struct clap_preset_discovery_provider *provider) {
  return false;
}

//----------

// Destroys the preset provider

void clap_preset_discovery_provider_destroy_callback(const struct clap_preset_discovery_provider *provider) {
}

//----------

// reads metadata from the given file and passes them to the metadata receiver

bool clap_preset_discovery_provider_get_metadata_callback(const struct clap_preset_discovery_provider *provider, const char *uri, const clap_preset_discovery_metadata_receiver_t *metadata_receiver) {
  return false;
}

//----------

// Query an extension.
// The returned pointer is owned by the provider.
// It is forbidden to call it before provider->init().
// You can call it within provider->init() call, and after.

const void* clap_preset_discovery_provider_get_extension_callback(const struct clap_preset_discovery_provider *provider, const char *extension_id) {
  return nullptr;
}

//----------------------------------------------------------------------

const clap_preset_discovery_provider_t SAT_CLAP_PRESET_DISCOVERY_PROVIDER = {
  .desc           = nullptr,
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
  return 1;
}

//----------

// Retrieves a preset provider descriptor by its index.
// Returns null in case of error.
// The descriptor must not be freed.
// [thread-safe]

const clap_preset_discovery_provider_descriptor_t* clap_preset_discovery_factory_get_descriptor_callback( const struct clap_preset_discovery_factory *factory, uint32_t index) {
  if (index == 0) return &SAT_CLAP_PRESET_DISCOVERY_PROVIDER_DESCRIPTOR;
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
  if (strcmp(provider_id,SAT_CLAP_PRESET_DISCOVERY_PROVIDER_DESCRIPTOR.id) == 0) {
    return  &SAT_CLAP_PRESET_DISCOVERY_PROVIDER;
  }
  return nullptr;
}


//----------------------------------------------------------------------

const clap_preset_discovery_factory_t SAT_CLAP_PRESET_FACTORY {
  .count          = clap_preset_discovery_factory_count_callback,
  .get_descriptor = clap_preset_discovery_factory_get_descriptor_callback,
  .create         = clap_preset_discovery_factory_create_callback
};

//----------------------------------------------------------------------
#endif

