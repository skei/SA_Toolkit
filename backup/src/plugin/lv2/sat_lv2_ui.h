#ifndef sat_lv2_ui_included
#define sat_lv2_ui_included
//----------------------------------------------------------------------

// clap-as-lv2

#include "sat.h"
//#include "plugin/sat_editor.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/lv2/sat_lv2.h"
#include "plugin/lv2/sat_lv2_utils.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Lv2UI {
  
//------------------------------
private:
//------------------------------

  const LV2UI_Descriptor*   MUIDescriptor     = nullptr;
  const char*               MPluginUri        = "";
  const char*               MBundlePath       = "";
  LV2UI_Write_Function      MWriteFunction    = nullptr;
  LV2UI_Controller          MController       = nullptr;
  LV2UI_Widget*             MWidget           = nullptr;
  const LV2_Feature* const* MFeatures         = nullptr;
  
//  LV2_Handle                      MHandle         = nullptr;
//  const LV2_Descriptor*           MDescriptor     = nullptr;
//  const char*                     MBundlePath     = nullptr;
//  const LV2_Feature* const*       MFeatures       = nullptr;
//  double                          MSampleRate     = 0.0;
//  
//  const clap_host_t*              MClapHost       = nullptr;
//  const clap_plugin_t*            MClapPlugin     = nullptr;
//  clap_process_t                  MProcess        = {0};
//
//  SAT_HostImplementation*         MHost           = nullptr;
//  SAT_Plugin*                     MPlugin         = nullptr;
//  
//  uint32_t                        MNumEvents      = 0;
//  char                            MEvents[SAT_PLUGIN_MAX_PARAM_EVENTS_PER_BLOCK * SAT_PLUGIN_MAX_EVENT_SIZE]  = {0};
//
//  uint32_t                        MNumInputs      = 0;
//  uint32_t                        MNumOutputs     = 0;
//  uint32_t                        MNumParameters  = 0;
//
//  float**                         MInputPtrs      = nullptr;
//  float**                         MOutputPtrs     = nullptr;
//  float**                         MParameterPtrs  = nullptr;
//  
//  LV2_URID                        MMidiInputUrid  = 0;
//  const LV2_Atom_Sequence*        MAtomSequence   = nullptr;
  
//------------------------------
public:
//------------------------------

  SAT_Lv2UI() {
    SAT_PRINT;
    
  }
  
  //----------
  
  ~SAT_Lv2UI() {
    SAT_PRINT;
  }

//------------------------------
public:
//------------------------------

//  LV2_Handle getHandle() {
//    return MHandle;
//  }

  //----------
  
  const LV2UI_Descriptor* getUIDescriptor() {
    return MUIDescriptor;
  }

//------------------------------
public:
//------------------------------

  /*  
    controller      The opaque controller pointer passed to LV2UI_Descriptor::instantiate().
    port_index      Index of the port to update.
    buffer          Buffer containing buffer_size bytes of data.
    buffer_size     Size of buffer in bytes.
    port_protocol   Either 0 or the URID for a ui:PortProtocol. If 0, the protocol is implicitly ui:floatProtocol, the port MUST be an lv2:ControlPort input, buffer MUST point to a single float value, and buffer_size MUST be sizeof(float). The UI SHOULD NOT use a protocol not supported by the host, but the host MUST gracefully ignore any protocol it does not understand.
  */

  //void LV2UI_Write_Function(LV2UI_Controller controller, uint32_t port_index, uint32_t buffer_size, uint32_t port_protocol, const void *buffer)
  
  //----------
  
  /*
    Create a new UI and return a handle to it.
    This function works similarly to LV2_Descriptor::instantiate()  
  
    descriptor      The descriptor for the UI to instantiate.
    plugin_uri      The URI of the plugin that this UI will control.
    bundle_path     The path to the bundle containing this UI, including the
                    trailing directory separator.
    write_function  A function that the UI can use to send data to the plugin's
                    input ports.
    controller      A handle for the UI instance to be passed as the first
                    parameter of UI methods.
    widget(output)  widget pointer. The UI points this at its main widget, which
                    has the type defined by the UI type in the data file.
    features        An array of LV2_Feature pointers. The host must pass all
                    feature URIs that it and the UI supports and any additional
                    data, as in LV2_Descriptor::instantiate(). Note that UI
                    features and plugin features are not necessarily the same.  
  */

  LV2UI_Handle lv2_ui_instantiate(const struct LV2UI_Descriptor *descriptor, const char *plugin_uri, const char *bundle_path, LV2UI_Write_Function write_function, LV2UI_Controller controller, LV2UI_Widget *widget, const LV2_Feature *const *features) {
    
    SAT_Print("plugin_uri %s bundle_path %s\n",plugin_uri);
    SAT_Lv2PrintFeatures(features);
    
    MUIDescriptor   = descriptor;
    MPluginUri      = plugin_uri;
    MBundlePath     = bundle_path;
    MWriteFunction  = write_function;
    MController     = controller;
    MWidget         = widget;
    MFeatures       = features;
    
    // instance access
    
    /*
      To support this feature the host must pass an LV2_Feature struct to the UI
      instantiate method with URI LV2_INSTANCE_ACCESS_URI and data pointed
      directly to the LV2_Handle of the plugin instance.    
    */
    
    LV2_Handle handle = SAT_Lv2FindFeature(LV2_INSTANCE_ACCESS_URI,features);
    SAT_Print("handle: %p\n",handle);
    SAT_Lv2Plugin* lv2_plugin = (SAT_Lv2Plugin*)handle;
    const clap_plugin_t* clap_plugin = lv2_plugin->getClapPlugin();
    //SAT_ClapPlugin* plugin = (SAT_ClapPlugin*)clap_plugin->plugin_data;
    SAT_Plugin* plugin = (SAT_Plugin*)clap_plugin->plugin_data;
    SAT_Print("SAT_Plugin* %p\n",plugin);
    
    // todo: host window, etc,
    // ala vst2/3 wrappers..
    // init, set_parent, open, ..
    
    return this;
  }
  
  //----------
  
  /*
    Destroy the UI.
    The host must not try to access the widget after calling this function.  
  */

  void lv2_ui_cleanup() {
  }
  
  //----------
  
  /*
    Tell the UI that something interesting has happened at a plugin port.
    What is "interesting" and how it is written to buffer is defined by format,
    which has the same meaning as in LV2UI_Write_Function(). Format 0 is a
    special case for lv2:ControlPort, where this function should be called when
    the port value changes (but not necessarily for every change), buffer_size
    must be sizeof(float), and buffer points to a single IEEE-754 float.

    By default, the host should only call this function for lv2:ControlPort
    inputs. However, the UI can request updates for other ports statically with
    ui:portNotification or dynamically with ui:portSubscribe.

    The UI MUST NOT retain any reference to buffer after this function returns,
    it is only valid for the duration of the call.

    This member may be NULL if the UI is not interested in any port events.
  */

  void lv2_ui_port_event(uint32_t port_index, uint32_t buffer_size, uint32_t format, const void *buffer) {
    SAT_Print("port_index %i buffer_size %i format %i\n",port_index,buffer_size,format);
  }
    
  //----------
  
  /*
    Return a data structure associated with an extension URI, typically an
    interface struct with additional function pointers.
    This member may be set to NULL if the UI is not interested in supporting
    any extensions. This is similar to LV2_Descriptor::extension_data().  
    
      LV2UI_Resize
      LV2UI_Port_Map
      LV2UI_Port_Subscribe
      LV2UI_Touch
      LV2UI_Request_Value
      LV2UI_Idle_Interface
      LV2UI_Show_Interface
      LV2UI_Peak_Data    
    
  */

  //const void* lv2_ui_extension_data(const char *uri) {
  //  return nullptr;
  //}

//------------------------------
private:
//------------------------------


};

//----------------------------------------------------------------------
#endif


