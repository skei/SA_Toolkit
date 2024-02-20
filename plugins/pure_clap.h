
#include "extern/clap/clap.h"

#include <stdio.h>      // printf
#include <stdlib.h>     // malloc
#include <string.h>     // strcmp

//----------------------------------------------------------------------
// descriptor
//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/myPlugin",
  .name         = "my plugin",
  .vendor       = "me",
  .url          = "https://website.com/myPlugin",
  .manual_url   = "https://website.com/myPlugin/manual.pdf",
  .support_url  = "https://website.com/support",
  .version      = "0.0.0",
  .description  = "my precious plugin",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

bool plugin_init_callback(const struct clap_plugin *plugin) {
  printf("plugin_init_callback\n");
  return true;
}

void plugin_destroy_callback(const struct clap_plugin *plugin) {
  printf("plugin_destroy_callback\n");
  free((void*)plugin); // allocated in create_plugin_callback()
}

bool plugin_activate_callback(const struct clap_plugin *plugin, double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
  printf("plugin_activate_callback : sample_rate: %.2f min_frames: %i max_frames: %i\n",sample_rate,min_frames_count,max_frames_count);
  return true;
}

void plugin_deactivate_callback(const struct clap_plugin *plugin) {
  printf("plugin_deactivate_callback\n");
}

bool plugin_start_processing_callback(const struct clap_plugin *plugin) {
  printf("plugin_start_processing_callback\n");
  return true;
}

void plugin_stop_processing_callback(const struct clap_plugin *plugin) {
  printf("plugin_stop_processing_callback\n");
}

void plugin_reset_callback(const struct clap_plugin *plugin) {
  printf("plugin_reset_callback\n");
}

clap_process_status plugin_process_callback(const struct clap_plugin *plugin, const clap_process_t *process) {
  //printf("plugin_process_callback\n");
  return CLAP_PROCESS_CONTINUE;
}

const void* plugin_get_extension_callback(const struct clap_plugin *plugin, const char *id) {
  printf("plugin_get_extension_callback : id: %s\n",id);
  return nullptr;
}

void plugin_on_main_thread_callback(const struct clap_plugin *plugin) {
  printf("plugin_on_main_thread_callback\n");
}

//----------------------------------------------------------------------
// plugin factory
//----------------------------------------------------------------------

uint32_t factory_get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  printf("factory_get_plugin_count_callback\n");
  return 1;
}

const clap_plugin_descriptor_t* factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  printf("factory_get_plugin_descriptor_callback : index: %i\n",index);
  if (index == 0) return &myDescriptor;
  return nullptr;
}

const clap_plugin_t* factory_create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  printf("factory_create_plugin_callback : plugin_id: %s\n",plugin_id);
  if (strcmp(plugin_id,myDescriptor.id) == 0) {
    clap_plugin_t* plugin = (clap_plugin_t*)malloc(sizeof(clap_plugin_t)); // freed in destroy_callback()
    plugin->desc             = &myDescriptor;
    plugin->plugin_data      = nullptr; // ptr to instance-specific data
    plugin->init             = plugin_init_callback;
    plugin->destroy          = plugin_destroy_callback;
    plugin->activate         = plugin_activate_callback;
    plugin->deactivate       = plugin_deactivate_callback;
    plugin->start_processing = plugin_start_processing_callback;
    plugin->stop_processing  = plugin_stop_processing_callback;
    plugin->reset            = plugin_reset_callback;
    plugin->process          = plugin_process_callback;
    plugin->get_extension    = plugin_get_extension_callback;
    plugin->on_main_thread   = plugin_on_main_thread_callback;
    return plugin;
  }
  return nullptr;
}

const clap_plugin_factory_t plugin_factory {
  .get_plugin_count      = factory_get_plugin_count_callback,
  .get_plugin_descriptor = factory_get_plugin_descriptor_callback,
  .create_plugin         = factory_create_plugin_callback
};

//----------------------------------------------------------------------
// clap entry
//----------------------------------------------------------------------

bool entry_init_callback(const char *plugin_path) {
  printf("entry_init_callback : plugin_path: %s\n",plugin_path);
  return true;
}

void entry_deinit_callback(void) {
  printf("entry_deinit_callback\n");
}

const void* entry_get_factory_callback(const char* factory_id) {
  printf("entry_get_factory_callback : factory_id: %s\n",factory_id);
  if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) return &plugin_factory;
  return nullptr;
}

CLAP_EXPORT extern const clap_plugin_entry_t clap_entry {
  .clap_version = CLAP_VERSION,
  .init         = entry_init_callback,
  .deinit       = entry_deinit_callback,
  .get_factory  = entry_get_factory_callback
};
