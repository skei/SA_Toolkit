#ifndef sat_lv2_entry_included
#define sat_lv2_entry_included
//----------------------------------------------------------------------

#include "base/sat.h"
//#include "plugin/sat_registry.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/lv2/sat_lv2.h"
#include "plugin/lv2/sat_lv2_manifest.h"
#include "plugin/lv2/sat_lv2_plugin.h"
#include "plugin/lv2/sat_lv2_utils.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Lv2Entry {

//------------------------------
private:
//------------------------------

//DESCRIPTOR        MDescriptor                       = {};
  LV2_Descriptor    MLv2Descriptor                    = {0};
  LV2UI_Descriptor  MLv2UIDescriptor                  = {0};
  char              MLv2Uri[SAT_LV2_MAX_URI_LENGTH]   = {0};
  char              MLv2UIUri[SAT_LV2_MAX_URI_LENGTH] = {0};
  
  //static LV2UI_Idle_Interface _lv2_idle_interface;
  //static LV2UI_Resize         _lv2_resize;

//------------------------------
public:
//------------------------------

  SAT_Lv2Entry() {
    SAT_PRINT;
    setup_lv2_uri();
    setup_lv2_descriptor();

    #ifndef SAT_NO_GUI
      setup_lv2ui_uri();
      setup_lv2ui_descriptor();
      //_lv2_idle_interface.idle = lv2_ext_idle;
      //_lv2_resize.handle = nullptr;
      //_lv2_resize.ui_resize = lv2_ext_resize;
    #endif
  }

  //----------

  virtual ~SAT_Lv2Entry() {
    SAT_PRINT;
  }

//------------------------------
public:
//------------------------------

  const LV2_Descriptor* lv2_entrypoint(unsigned long Index) {
    SAT_Print("Index %i\n",Index);
    if (Index > 0) return nullptr;
    return &MLv2Descriptor;
  }

  //----------

  const LV2UI_Descriptor* lv2ui_entrypoint(uint32_t index) {
    SAT_Print("index %i\n",index);
    if (index > 0) return nullptr;
    return &MLv2UIDescriptor;
  }

  //----------

//  SAT_Descriptor* getDescriptor() {
//    return &MDescriptor;
//  }

//------------------------------
public:
//------------------------------

  void export_ttl(void) {
    SAT_PRINT;
    #ifdef SAT_LV2_EXPORT_TTL
      SAT_Lv2Manifest manifest = {};
      //manifest.create_manifest_ttl();
      //manifest.create_plugin_ttl();
      //manifest.create_gui_ttl();
      manifest.export_ttl();
    #endif
  }

//------------------------------
private:
//------------------------------

  void setup_lv2_uri() {
    SAT_PRINT;
    memset(MLv2Uri,0,SAT_LV2_MAX_URI_LENGTH);
    strcpy(MLv2Uri,"urn:");
//    strcat(MLv2Uri,MDescriptor.getAuthor());
    strcat(MLv2Uri,"/");
//    strcat(MLv2Uri,MDescriptor.getName());
    SAT_Print("MLv2Uri: %s\n",MLv2Uri);
  }

  //----------

  void setup_lv2ui_uri() {
    SAT_PRINT;
    memset(MLv2UIUri,0,SAT_LV2_MAX_URI_LENGTH);
    strcpy(MLv2UIUri,"urn:");
//    strcat(MLv2UIUri,MDescriptor.getAuthor());
    strcat(MLv2UIUri,"/");
//    strcat(MLv2UIUri,MDescriptor.getName());
    strcat(MLv2UIUri,"_ui");
    SAT_Print("MLv2UIUri: %s\n",MLv2UIUri);
  }

  //----------

  void setup_lv2_descriptor() {
    SAT_PRINT;
    memset(&MLv2Descriptor,0,sizeof(LV2_Descriptor));
    MLv2Descriptor.URI             = MLv2Uri;
    MLv2Descriptor.instantiate     = lv2_instantiate_callback;
    MLv2Descriptor.connect_port    = lv2_connect_port_callback;
    MLv2Descriptor.activate        = lv2_activate_callback;
    MLv2Descriptor.run             = lv2_run_callback;
    MLv2Descriptor.deactivate      = lv2_deactivate_callback;
    MLv2Descriptor.cleanup         = lv2_cleanup_callback;
    MLv2Descriptor.extension_data  = lv2_extension_data_callback;
  }

  //----------

  void setup_lv2ui_descriptor() {
    SAT_PRINT;
    memset(&MLv2UIDescriptor,0,sizeof(LV2UI_Descriptor));
    MLv2UIDescriptor.URI            = MLv2UIUri;
    MLv2UIDescriptor.instantiate    = lv2ui_instantiate_callback;
    MLv2UIDescriptor.cleanup        = lv2ui_cleanup_callback;
    MLv2UIDescriptor.port_event     = lv2ui_port_event_callback;
    MLv2UIDescriptor.extension_data = lv2ui_extension_data_callback;
  }

  //------------------------------
  // extensions
  //------------------------------

  static
  int lv2_ext_idle(LV2UI_Handle handle) {
    //StuckUI* self = (StuckUI*)handle;
    //self->idle();
    return 0;
  }

  static
  int lv2_ext_resize(LV2UI_Feature_Handle handle, int w, int h) {
    //StuckUI* self = (StuckUI*)handle;
    //self->ui->size(w,h);
    return 0;
  }

//----------------------------------------------------------------------
private: // lv2 callbacks
//----------------------------------------------------------------------

  /*
     Instantiate the plugin.

     Note that instance initialisation should generally occur in activate()
     rather than here. If a host calls instantiate(), it MUST call cleanup()
     at some point in the future.

     @param descriptor Descriptor of the plugin to instantiate.

     @param sample_rate Sample rate, in Hz, for the new plugin instance.

     @param bundle_path Path to the LV2 bundle which contains this plugin
     binary. It MUST include the trailing directory separator so that simply
     appending a filename will yield the path to that file in the bundle.

     @param features A NULL terminated array of LV2_Feature structs which
     represent the features the host supports. Plugins may refuse to
     instantiate if required features are not found here. However, hosts MUST
     NOT use this as a discovery mechanism: instead, use the RDF data to
     determine which features are required and do not attempt to instantiate
     unsupported plugins at all. This parameter MUST NOT be NULL, i.e. a host
     that supports no features MUST pass a single element array containing
     NULL.

     @return A handle for the new plugin instance, or NULL if instantiation
     has failed.
  */

  //-----

  static
  LV2_Handle lv2_instantiate_callback(const LV2_Descriptor* descriptor, double sample_rate, const char* bundle_path, const LV2_Feature* const* features) {
    SAT_Print("sample_rate %.2f bundle_path '%s' features %p\n",sample_rate,bundle_path,features);
    SAT_Lv2PrintFeatures(features);
//    SAT_Descriptor* desc = sat_lv2_get_descriptor();
//    //SAT_Lv2Plugin* lv2_plugin = new SAT_Lv2Plugin(desc);
//    // !!!
//    SAT_Instance* inst = new INSTANCE(desc);                             // who deletes this ???
    SAT_Lv2Plugin* lv2_plugin = new SAT_Lv2Plugin();
//    lv2_plugin->lv2_setup(sample_rate,bundle_path,features);
//    return (LV2_Handle)lv2_plugin;
    return lv2_plugin->getHandle();
  }

  //----------

  static
  void lv2_connect_port_callback(LV2_Handle instance, uint32_t port, void* data_location) {
    SAT_Print("port %i data_location %p\n",port,data_location);
    SAT_Lv2Plugin* lv2_plugin = (SAT_Lv2Plugin*)instance;
    if (lv2_plugin) lv2_plugin->lv2_connect_port(port,data_location);
  }

  //----------

  static
  void lv2_activate_callback(LV2_Handle instance) {
    SAT_PRINT;
    SAT_Lv2Plugin* lv2_plugin = (SAT_Lv2Plugin*)instance;
    if (lv2_plugin) lv2_plugin->lv2_activate();
  }

  //----------

  static
  void lv2_run_callback(LV2_Handle instance, uint32_t sample_count) {
    SAT_Print("sample_count %i\n",sample_count);
    SAT_Lv2Plugin* lv2_plugin = (SAT_Lv2Plugin*)instance;
    if (lv2_plugin) lv2_plugin->lv2_run(sample_count);
  }

  //----------

  static
  void lv2_deactivate_callback(LV2_Handle instance) {
    SAT_PRINT;
    SAT_Lv2Plugin* lv2_plugin = (SAT_Lv2Plugin*)instance;
    if (lv2_plugin) lv2_plugin->lv2_deactivate();
  }

  //----------

  static
  void lv2_cleanup_callback(LV2_Handle instance) {
    SAT_PRINT;
    SAT_Lv2Plugin* lv2_plugin = (SAT_Lv2Plugin*)instance;
    if (lv2_plugin) lv2_plugin->lv2_cleanup();
    delete lv2_plugin;
  }

  //----------

  /*
     Return additional plugin data defined by some extension.

     A typical use of this facility is to return a struct containing function
     pointers to extend the LV2_Descriptor API.

     The actual type and meaning of the returned object MUST be specified
     precisely by the extension. This function MUST return NULL for any
     unsupported URI. If a plugin does not support any extension data, this
     field may be NULL.

     The host is never responsible for freeing the returned value.
  */

  static
  const void* lv2_extension_data_callback(const char* uri) {
    SAT_Print("uri %s\n",uri);
    return nullptr;
  }

//----------------------------------------------------------------------
private: // ui callbacks
//----------------------------------------------------------------------

  static
  LV2UI_Handle lv2ui_instantiate_callback(
    const LV2UI_Descriptor*     descriptor,
    const char*                 plugin_uri,
    const char*                 bundle_path,
    LV2UI_Write_Function        write_function,
    LV2UI_Controller            controller,
    LV2UI_Widget*               widget,
    const LV2_Feature* const*   features) {
    /* --- */
    //SAT_Print("\n");

//    if (strcmp(plugin_uri, MLv2UIUri) != 0) {
//      return nullptr;
//    }
//    StuckUI* self = new StuckUI();
//    if(!self) return 0;
//    LV2UI_Resize* resize = NULL;
//    /* --- */
//    self->controller = controller;
//    self->write_function = write_function;
//    /* --- */
//    void* parentXwindow = 0;
//    for (int i = 0; features[i]; ++i) {
//      if (!strcmp(features[i]->URI, LV2_UI__parent)) {
//        parentXwindow = features[i]->data;
//      }
//      else if (!strcmp(features[i]->URI, LV2_UI__resize)) {
//        resize = (LV2UI_Resize*)features[i]->data;
//      }
//    }
//    /* --- */
//    self->ui = self->show();
//    fl_open_display();
//    // set host to change size of the window
//    if (resize) {
//       resize->ui_resize(resize->handle, self->ui->w(), self->ui->h());
//    }
//    fl_embed( self->ui,(Window)parentXwindow);
//    *widget = (LV2UI_Widget)fl_xid(self->ui);
//    return (LV2UI_Handle)self;

    return nullptr;
  }

  //----------

  static
  void lv2ui_cleanup_callback(LV2UI_Handle ui) {
    //SAT_Print("\n");
//    StuckUI *self = (StuckUI*)ui;
//    delete self;
  }

  //----------

  static
  void lv2ui_port_event_callback(LV2UI_Handle ui, uint32_t port_index, uint32_t buffer_size, uint32_t format, const void * buffer) {
    //SAT_Print("\n");
//    StuckUI *self = (StuckUI*)ui;
//    if (!format) {
//      float val = *(float*)buffer;
//      switch(port_index) {
//        case STICKIT:
//          self->stickit->value((int)val);
//          self->led->value((int)val);
//          break;
//        case DRONEGAIN:
//          self->volume->value(val);
//          break;
//        case RELEASE:
//          self->time->value(val);
//          break;
//      }
//    }
  }

  //----------

  static
  const void* lv2ui_extension_data_callback(const char* uri) {
    //SAT_Print("uri %s\n",uri);
    //if (!strcmp(uri, LV2_UI__idleInterface)) return &_lv2_idle_interface;
    //if (!strcmp(uri, LV2_UI__resize)) return &_lv2_resize;
    return nullptr;
  }

  //----------
  
};

//----------------------------------------------------------------------
//
// global LV2 entry
//
//----------------------------------------------------------------------

// ugh..
//TODO: move into SAT_GLOBAL.VST2 ??

SAT_Lv2Entry GLOBAL_LV2_PLUGIN_ENTRY;

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

/*
   Prototype for plugin accessor function.

   Plugins are discovered by hosts using RDF data (not by loading libraries).
   See http://lv2plug.in for details on the discovery process, though most
   hosts should use an existing library to implement this functionality.

   This is the simple plugin discovery API, suitable for most statically
   defined plugins.  Advanced plugins that need access to their bundle during
   discovery can use lv2_lib_descriptor() instead.  Plugin libraries MUST
   include a function called "lv2_descriptor" or "lv2_lib_descriptor" with
   C-style linkage, but SHOULD provide "lv2_descriptor" wherever possible.

   When it is time to load a plugin (designated by its URI), the host loads the
   plugin's library, gets the lv2_descriptor() function from it, and uses this
   function to find the LV2_Descriptor for the desired plugin.  Plugins are
   accessed by index using values from 0 upwards.  This function MUST return
   NULL for out of range indices, so the host can enumerate plugins by
   increasing `index` until NULL is returned.

   Note that `index` has no meaning, hosts MUST NOT depend on it remaining
   consistent between loads of the plugin library.
*/

//----------------------------------------------------------------------

const LV2_Descriptor*   sat_lv2_entrypoint(  uint32_t Index) asm ("lv2_descriptor");
const LV2UI_Descriptor* sat_lv2ui_entrypoint(uint32_t index) asm ("lv2ui_descriptor");
void                    sat_lv2_export_ttl(void) asm             ("lv2_export_ttl");

//----------

//__attribute__((visibility("default")))
__SAT_EXPORT
const LV2_Descriptor* sat_lv2_entrypoint(unsigned long Index) {
  SAT_Print("Index %i\n",Index);
  return GLOBAL_LV2_PLUGIN_ENTRY.lv2_entrypoint(Index);
}

//__attribute__((visibility("default")))
__SAT_EXPORT
const LV2UI_Descriptor* sat_lv2ui_entrypoint(uint32_t index) {
  SAT_Print("index %i\n",index);
  return GLOBAL_LV2_PLUGIN_ENTRY.lv2ui_entrypoint(index);
}

__attribute__((visibility("default")))
void sat_lv2_export_ttl(void) {
  SAT_PRINT;
  GLOBAL_LV2_PLUGIN_ENTRY.export_ttl();
}

// called from lv2_instantiate_callback()

//SAT_Descriptor* sat_lv2_get_descriptor() {
//  return GLOBAL_LV2_PLUGIN_ENTRY.getDescriptor();
//}
  
//----------

/*
  - ?? (check this)
    carla calls lv2_descriptor a second time before deleting the first
*/


//----------------------------------------------------------------------
#endif

