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

  /*
    https://lv2plug.in/book/
    LV2 makes use of URIs as globally-unique identifiers for resources.
    For example, the ID of the plugin described here is <http://lv2plug.in/plugins/eg-amp>.
    Note that URIs are only used as identifiers and don’t necessarily imply that something
    can be accessed at that address on the web (though that may be the case).
  */
  
  /*
    https://www.ietf.org/rfc/rfc2396.txt  
    A URN differs from a URL in that it's primary purpose is persistent
    labeling of a resource with an identifier.  That identifier is drawn
    from one of a set of defined namespaces, each of which has its own
    set name structure and assignment procedures.  The "urn" scheme has
    been reserved to establish the requirements for a standardized URN
    namespace, as defined in "URN Syntax" [RFC2141] and its related
    specifications.
    Most of the examples in this specification demonstrate URL, since
    they allow the most varied use of the syntax and often have a
    hierarchical namespace.  A parser of the URI syntax is capable of
    parsing both URL and URN references as a generic URI; once the scheme
    is determined, the scheme-specific parsing can be performed on the
    generic URI components.  In other words, the URI syntax is a superset
    of the syntax of all URI schemes.  
  */

  // where is all of this free'd?
  // (uri, descriptor)

  const LV2_Descriptor* lv2_entrypoint(unsigned long Index) {
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
    descriptor->instantiate     = lv2_instantiate_callback;
    descriptor->connect_port    = lv2_connect_port_callback;
    descriptor->activate        = lv2_activate_callback;
    descriptor->run             = lv2_run_callback;
    descriptor->deactivate      = lv2_deactivate_callback;
    descriptor->cleanup         = lv2_cleanup_callback;
    descriptor->extension_data  = lv2_extension_data_callback;
    
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
    //SAT_Print("sample_rate %.2f bundle_path '%s' features %p\n",sample_rate,bundle_path,features);
    SAT_Lv2PrintFeatures(features);
    SAT_Lv2Plugin* lv2_plugin = new SAT_Lv2Plugin();
    return lv2_plugin->lv2_instantiate(descriptor,sample_rate,bundle_path,features);
  }

  //----------

  static
  void lv2_connect_port_callback(LV2_Handle instance, uint32_t port, void* data_location) {
    //SAT_Print("port %i data_location %p\n",port,data_location);
    SAT_Lv2Plugin* lv2_plugin = (SAT_Lv2Plugin*)instance;
    if (lv2_plugin) lv2_plugin->lv2_connect_port(port,data_location);
  }

  //----------

  static
  void lv2_activate_callback(LV2_Handle instance) {
    //SAT_PRINT;
    SAT_Lv2Plugin* lv2_plugin = (SAT_Lv2Plugin*)instance;
    if (lv2_plugin) lv2_plugin->lv2_activate();
  }

  //----------

  static
  void lv2_run_callback(LV2_Handle instance, uint32_t sample_count) {
    //SAT_Print("sample_count %i\n",sample_count);
    SAT_Lv2Plugin* lv2_plugin = (SAT_Lv2Plugin*)instance;
    if (lv2_plugin) lv2_plugin->lv2_run(sample_count);
  }

  //----------

  static
  void lv2_deactivate_callback(LV2_Handle instance) {
    //SAT_PRINT;
    SAT_Lv2Plugin* lv2_plugin = (SAT_Lv2Plugin*)instance;
    if (lv2_plugin) lv2_plugin->lv2_deactivate();
  }

  //----------

  static
  void lv2_cleanup_callback(LV2_Handle instance) {
    SAT_PRINT;
    SAT_Lv2Plugin* lv2_plugin = (SAT_Lv2Plugin*)instance;
    if (lv2_plugin) {
      lv2_plugin->lv2_cleanup();
      const LV2_Descriptor* descriptor = lv2_plugin->getDescriptor();
      const char* uri = descriptor->URI;
      delete lv2_plugin;
      free((void*)uri);
      free((void*)descriptor);
    }
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
  
  //----------
  
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

const LV2_Descriptor*   sat_lv2_entrypoint(uint32_t Index) asm ("lv2_descriptor");
//const LV2UI_Descriptor* sat_lv2ui_entrypoint(uint32_t index) asm ("lv2ui_descriptor");
void                    sat_lv2_export_ttl(void) asm             ("lv2_export_ttl");

//----------

__SAT_EXPORT
//__attribute__((visibility("default")))
const LV2_Descriptor* sat_lv2_entrypoint(uint32_t Index) {
  SAT_Print("Index %i\n",Index);
  return GLOBAL_LV2_PLUGIN_ENTRY.lv2_entrypoint(Index);
}

//__SAT_EXPORT
////__attribute__((visibility("default")))
//const LV2UI_Descriptor* sat_lv2ui_entrypoint(uint32_t index) {
//  SAT_Print("index %i\n",index);
//  return GLOBAL_LV2_PLUGIN_ENTRY.lv2ui_entrypoint(index);
//}

__SAT_EXPORT
//__attribute__((visibility("default")))
void sat_lv2_export_ttl(void) {
  SAT_PRINT;
  GLOBAL_LV2_PLUGIN_ENTRY.export_ttl();
}

//----------------------------------------------------------------------
#endif

