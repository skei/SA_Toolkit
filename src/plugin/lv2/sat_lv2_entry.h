#ifndef sat_lv2_entry_included
#define sat_lv2_entry_included
//----------------------------------------------------------------------

#include "base/sat.h"
//#include "plugin/sat_registry.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/lv2/sat_lv2.h"
#include "plugin/lv2/sat_lv2_manifest.h"
#include "plugin/lv2/sat_lv2_plugin.h"
#include "plugin/lv2/sat_lv2_ui.h"
#include "plugin/lv2/sat_lv2_utils.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Lv2Entry {

//------------------------------
public:
//------------------------------

  SAT_Lv2Entry() {
    SAT_PRINT;
  }

  //----------

  virtual ~SAT_Lv2Entry() {
    SAT_PRINT;
  }

//------------------------------
public:
//------------------------------

  const LV2_Descriptor* lv2_plugin_entrypoint(unsigned long Index) {
    SAT_Print("Index %i\n",Index);
    if (Index >= SAT_GLOBAL.REGISTRY.getNumDescriptors()) return nullptr;
    const clap_plugin_descriptor_t* clap_descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(Index);
    
    char* uri = (char*)malloc(SAT_LV2_MAX_URI_LENGTH);
    memset(uri,0,SAT_LV2_MAX_URI_LENGTH);
    strcpy(uri,"urn:");
    strcat(uri,clap_descriptor->id);

    LV2_Descriptor* descriptor = (LV2_Descriptor*)malloc(sizeof(LV2_Descriptor));
    memset(descriptor,0,sizeof(LV2_Descriptor));
    descriptor->URI             = uri;
    descriptor->instantiate     = lv2_plugin_instantiate_callback;
    descriptor->connect_port    = lv2_plugin_connect_port_callback;
    descriptor->activate        = lv2_plugin_activate_callback;
    descriptor->run             = lv2_plugin_run_callback;
    descriptor->deactivate      = lv2_plugin_deactivate_callback;
    descriptor->cleanup         = lv2_plugin_cleanup_callback;
    descriptor->extension_data  = lv2_plugin_extension_data_callback;
    
    return descriptor;
    
  }
  
  //----------
  
  const LV2UI_Descriptor* lv2_ui_entrypoint(unsigned long Index) {
    SAT_Print("Index %i\n",Index);
    if (Index >= SAT_GLOBAL.REGISTRY.getNumDescriptors()) return nullptr;
    const clap_plugin_descriptor_t* clap_descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(Index);
    
    char* uri = (char*)malloc(SAT_LV2_MAX_URI_LENGTH);
    memset(uri,0,SAT_LV2_MAX_URI_LENGTH);
    strcpy(uri,"urn:");
    strcat(uri,clap_descriptor->id);
    strcat(uri,":ui");

    LV2UI_Descriptor* descriptor = (LV2UI_Descriptor*)malloc(sizeof(LV2UI_Descriptor));
    memset(descriptor,0,sizeof(LV2UI_Descriptor));
    descriptor->URI             = uri;
    descriptor->instantiate     = lv2_ui_instantiate_callback;
    descriptor->cleanup         = lv2_ui_cleanup_callback;
    descriptor->port_event      = lv2_ui_port_event_callback;
    descriptor->extension_data  = lv2_ui_extension_data_callback;
    
    return descriptor;
  }

//------------------------------
public:
//------------------------------

  void export_ttl(void) {
    SAT_PRINT;
    #ifdef SAT_LV2_EXPORT_TTL
      SAT_Lv2Manifest manifest = {};
      manifest.export_ttl();
    #endif
  }

//----------------------------------------------------------------------
private: // lv2 plugin callbacks
//----------------------------------------------------------------------

  static
  LV2_Handle lv2_plugin_instantiate_callback(const LV2_Descriptor* descriptor, double sample_rate, const char* bundle_path, const LV2_Feature* const* features) {
    //SAT_Print("sample_rate %.2f bundle_path '%s' features %p\n",sample_rate,bundle_path,features);
    SAT_Lv2PrintFeatures(features);
    SAT_Lv2Plugin* lv2_plugin = new SAT_Lv2Plugin();
    return lv2_plugin->lv2_plugin_instantiate(descriptor,sample_rate,bundle_path,features);
  }

  //----------

  static
  void lv2_plugin_connect_port_callback(LV2_Handle instance, uint32_t port, void* data_location) {
    //SAT_Print("port %i data_location %p\n",port,data_location);
    SAT_Lv2Plugin* lv2_plugin = (SAT_Lv2Plugin*)instance;
    if (lv2_plugin) lv2_plugin->lv2_plugin_connect_port(port,data_location);
  }

  //----------

  static
  void lv2_plugin_activate_callback(LV2_Handle instance) {
    //SAT_PRINT;
    SAT_Lv2Plugin* lv2_plugin = (SAT_Lv2Plugin*)instance;
    if (lv2_plugin) lv2_plugin->lv2_plugin_activate();
  }

  //----------

  static
  void lv2_plugin_run_callback(LV2_Handle instance, uint32_t sample_count) {
    //SAT_Print("sample_count %i\n",sample_count);
    SAT_Lv2Plugin* lv2_plugin = (SAT_Lv2Plugin*)instance;
    if (lv2_plugin) lv2_plugin->lv2_plugin_run(sample_count);
  }

  //----------

  static
  void lv2_plugin_deactivate_callback(LV2_Handle instance) {
    //SAT_PRINT;
    SAT_Lv2Plugin* lv2_plugin = (SAT_Lv2Plugin*)instance;
    if (lv2_plugin) lv2_plugin->lv2_plugin_deactivate();
  }

  //----------

  static
  void lv2_plugin_cleanup_callback(LV2_Handle instance) {
    SAT_PRINT;
    SAT_Lv2Plugin* lv2_plugin = (SAT_Lv2Plugin*)instance;
    if (lv2_plugin) {
      lv2_plugin->lv2_plugin_cleanup();
      const LV2_Descriptor* descriptor = lv2_plugin->getDescriptor();
      const char* uri = descriptor->URI;
      delete lv2_plugin;
      free((void*)uri);
      free((void*)descriptor);
    }
  }

  //----------
  
  static
  const void* lv2_plugin_extension_data_callback(const char* uri) {
    SAT_Print("uri %s\n",uri);
    return nullptr;
  }

//----------------------------------------------------------------------
private: // lv2 ui callbacks
//----------------------------------------------------------------------

  static
  LV2UI_Handle lv2_ui_instantiate_callback(const struct LV2UI_Descriptor *descriptor, const char *plugin_uri, const char *bundle_path, LV2UI_Write_Function write_function, LV2UI_Controller controller, LV2UI_Widget *widget, const LV2_Feature *const *features) {
    //SAT_Print("sample_rate %.2f bundle_path '%s' features %p\n",sample_rate,bundle_path,features);
    //SAT_Lv2PrintFeatures(features);
    SAT_Lv2UI* lv2_ui = new SAT_Lv2UI();
    return lv2_ui->lv2_ui_instantiate(descriptor,plugin_uri,bundle_path,write_function,controller,widget,features);
  }
  
  //----------
  
  static
  void lv2_ui_cleanup_callback(LV2UI_Handle ui) {
    SAT_PRINT;
    SAT_Lv2UI* lv2_ui = (SAT_Lv2UI*)ui;
    lv2_ui->lv2_ui_cleanup();
    const LV2UI_Descriptor* descriptor = lv2_ui->getUIDescriptor();
    const char* uri = descriptor->URI;
    delete lv2_ui;
    free((void*)uri);
    free((void*)descriptor);
  }

  //----------
  
  static
  void lv2_ui_port_event_callback(LV2UI_Handle ui, uint32_t port_index, uint32_t buffer_size, uint32_t format, const void *buffer) {
    SAT_Lv2UI* lv2_ui = (SAT_Lv2UI*)ui;
    lv2_ui->lv2_ui_port_event(port_index,buffer_size,format,buffer);
  }
  
  //----------
  
  static
  const void* lv2_ui_extension_data_callback(const char *uri) {
    return nullptr;
  }
  
};

//----------------------------------------------------------------------
//
// global LV2 entry
//
//----------------------------------------------------------------------

//TODO: move into SAT_GLOBAL.LV2 ??
SAT_Lv2Entry GLOBAL_LV2_PLUGIN_ENTRY;

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

const LV2_Descriptor*   sat_lv2_plugin_entrypoint(uint32_t Index) asm ("lv2_descriptor");
const LV2UI_Descriptor* sat_lv2_ui_entrypoint(uint32_t Index)     asm ("lv2ui_descriptor");
void                    sat_lv2_export_ttl(void)                  asm ("lv2_export_ttl");

//const LV2_Descriptor*   sat_lv2_lib_entrypoint(uint32_t Index)  asm ("lv2_lib_descriptor");

//----------

__SAT_EXPORT
//__attribute__((visibility("default")))
const LV2_Descriptor* sat_lv2_plugin_entrypoint(uint32_t index) {
  SAT_Print("Index %i\n",index);
  return GLOBAL_LV2_PLUGIN_ENTRY.lv2_plugin_entrypoint(index);
}

__SAT_EXPORT
//__attribute__((visibility("default")))
const LV2UI_Descriptor* sat_lv2_ui_entrypoint(uint32_t index) {
  SAT_Print("index %i\n",index);
  return GLOBAL_LV2_PLUGIN_ENTRY.lv2_ui_entrypoint(index);
}

__SAT_EXPORT
//__attribute__((visibility("default")))
void sat_lv2_export_ttl(void) {
  SAT_PRINT;
  GLOBAL_LV2_PLUGIN_ENTRY.export_ttl();
}

//----------------------------------------------------------------------
#endif


//----------------------------------------------------------------------
// features
//----------------------------------------------------------------------

/*
  qtractor 0.9.34
  
	 0: http://lv2plug.in/ns/ext/urid#map
	 1: http://lv2plug.in/ns/ext/urid#unmap
	 2: http://lv2plug.in/ns/ext/state
	 3: http://lv2plug.in/ns/ext/buf-size#fixedBlockLength
	 4: http://lv2plug.in/ns/ext/buf-size#boundedBlockLength
	 5: http://lv2plug.in/ns/ext/state#loadDefaultState
	 6: http://lv2plug.in/ns/ext/state#mapPath
	 7: http://kxstudio.sf.net/ns/lv2ext/programs#Host
	 8: http://ardour.org/lv2/midnam#update
	 9: http://lv2plug.in/ns/ext/options#options
   
  reaper
  
	 0: http://lv2plug.in/ns/ext/urid#map
	 1: http://lv2plug.in/ns/ext/urid#unmap
	 2: http://kxstudio.sf.net/ns/lv2ext/external-ui#Host
	 3: http://lv2plug.in/ns/extensions/ui#parent
	 4: http://lv2plug.in/ns/ext/instance-access
	 5: http://lv2plug.in/ns/ext/data-access
	 6: http://lv2plug.in/ns/ext/worker#schedule
	 7: http://lv2plug.in/ns/extensions/ui#touch
	 8: http://lv2plug.in/ns/extensions/ui#resize
	 9: http://lv2plug.in/ns/ext/state#makePath
	 10: http://lv2plug.in/ns/ext/state#freePath
	 11: http://lv2plug.in/ns/ext/state#mapPath
	 12: http://ardour.org/lv2/midnam#update
	 13: http://lv2plug.in/ns/extensions/ui#portMap
	 14: http://harrisonconsoles.com/lv2/inlinedisplay#queue_draw
	 15: http://lv2plug.in/ns/ext/options#options
	 16: http://lv2plug.in/ns/ext/state#loadDefaultState
	 17: http://lv2plug.in/ns/ext/buf-size#boundedBlockLength
*/
