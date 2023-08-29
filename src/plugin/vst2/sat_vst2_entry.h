#ifndef sat_vst2_entry_included
#define sat_vst2_entry_included
//----------------------------------------------------------------------

// work in progress..

#include "base/sat.h"
#include "base/system/sat_paths.h"
#include "plugin/sat_registry.h"
#include "plugin/clap/sat_clap.h"
//#include "plugin/clap/sat_clap_host.h"
#include "plugin/vst2/sat_vst2.h"
#include "plugin/vst2/sat_vst2_host_implementation.h"
#include "plugin/vst2/sat_vst2_plugin.h"
#include "plugin/vst2/sat_vst2_utils.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Vst2Entry {

//------------------------------
private:
//------------------------------

  //SAT_ClapHost* MHost = nullptr;

//------------------------------
public:
//------------------------------

  SAT_Vst2Entry() {
    SAT_Print("\n");
  }

  //----------

  ~SAT_Vst2Entry() {
    SAT_Print("\n");
  }

//------------------------------
public:
//------------------------------

// host cando "shellCategory"
// ask the host about shellCategorycurid;


  AEffect* entry(audioMasterCallback audioMaster) {
    SAT_Print("\n");
    
    uint32_t index = 0;

    // shell
    #ifndef SAT_PLUGIN_VST2_NO_SHELL
      uint32_t current_id = 0;
      if ( audioMaster(nullptr,audioMasterCanDo,0,0,(void*)"shellCategory",0) == 1) {
        //SAT_Print("host supports shellCategory\n");
        current_id = audioMaster(nullptr,audioMasterCurrentId,0,0,nullptr,0);
        SAT_Print("shellCategory.. current_id: %i\n",current_id);
        for (int32_t i=0; i<SAT_REGISTRY.getNumDescriptors(); i++) {
          const clap_plugin_descriptor_t* desc = SAT_REGISTRY.getDescriptor(i);
          if (current_id == sat_vst2_create_unique_id(desc)) {
            SAT_Print("found plugin %i = '%s'\n",i,desc->name);
            index = i;
            break;
          }
        }
      }
      //if ( audioMaster(effect,audioMasterCanDo,0,0,(void*)"shellCategorycurID",0) == 1) {
      //  SAT_Print("host supports shellCategorycurID\n");
      //}
    #endif

    char path[1024] = {};
    const char* plugin_path = SAT_GetLibFilename(path);
    SAT_Print("plugin_path '%s'\n",plugin_path);
    //SAT_GLOBAL.REGISTRY.setPath(plugin_path);
    
//    SAT_Vst2HostImplementation* vst2_host = new SAT_Vst2HostImplementation(audioMaster);      // deleted in SAT_Vst2Plugin destructor
//    const clap_plugin_descriptor_t* descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(index);
//    const clap_host_t* host = vst2_host->getHost();
//    const clap_plugin_t* plugin = SAT_CreatePlugin(index,descriptor,host);                    // deleted in SAT_Vst2Plugin destructor (call setHost!)
//    plugin->init(plugin); // destroy() called in effClose
//    SAT_Vst2Plugin* vst2_plugin  = new SAT_Vst2Plugin(host,plugin,audioMaster);               // deleted in vst2_dispatcher_callback(effClose)

    const clap_plugin_descriptor_t* descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(index);
    SAT_Vst2HostImplementation* vst2_host = new SAT_Vst2HostImplementation(audioMaster);        // deleted in SAT_Vst2Plugin destructor
    const clap_host_t* claphost = vst2_host->getClapHost();
    const clap_plugin_t* clapplugin = SAT_CreatePlugin(index,descriptor,claphost);              // destroy() called in SAT_Vst2Plugin destructor
    SAT_Plugin* plugin = (SAT_Plugin*)clapplugin->plugin_data;
    plugin->setPluginFormat("VST2");
    plugin->init();
    //SAT_Vst2Plugin* vst2plugin = new SAT_Vst2Plugin(plugin);
    SAT_Vst2Plugin* vst2plugin = new SAT_Vst2Plugin(claphost,clapplugin,audioMaster);           // deleted in vst2_dispatcher_callback/effClose

    // hmmm..
    vst2plugin->setHost(vst2_host);
    
    #ifndef SAT_PLUGIN_VST2_NO_SHELL
      vst2plugin->MShellPluginCurrentId = current_id;
    #endif

    /*
      assumes stereo in & out
      TODO: check clap.audio-ports
      use number of channels in port with 'is_main'
    */

    uint32_t  num_inputs  = 2;
    uint32_t  num_outputs = 2;
    uint32_t  num_params  = 0;
    int32_t   flags       = effFlagsCanReplacing;

    /*
    if (strstr(descriptor->features,"instrument")) {
      flags |= effFlagsIsSynth;
      num_inputs = 0;
    }
    */

    const clap_plugin_gui_t* gui = (const clap_plugin_gui_t*)plugin->get_extension(CLAP_EXT_GUI);
    if (gui) flags |= effFlagsHasEditor;

    uint32_t i = 0;
    while (descriptor->features[i]) {
      const char* str = descriptor->features[i++];
      SAT_Print("feature %i: '%s'\n",i,str);
      if (strcmp(str,CLAP_PLUGIN_FEATURE_INSTRUMENT) == 0) flags |= effFlagsIsSynth;
    }
    
    // flags |= effFlagsProgramChunks;
    // flags |= effFlagsNoSoundInStop;
    // flags |= effFlagsCanDoubleReplacing;

    const clap_plugin_params_t* params = (const clap_plugin_params_t*)plugin->get_extension(CLAP_EXT_PARAMS);
    if (params) {
      num_params = params->count(clapplugin);
    }

    // move all of the below to vst2plugin ?

    AEffect* effect = vst2plugin->getAEffect();
    vst2_host->setAEffect(effect);
    memset(effect,0,sizeof(AEffect));

    //
    
    uint32_t unique_id = sat_vst2_create_unique_id(descriptor);

    effect->magic                   = kEffectMagic;
    effect->uniqueID                = unique_id;
    effect->flags                   = flags;
    effect->numInputs               = num_inputs;
    effect->numOutputs              = num_outputs;
    effect->numParams               = num_params;
    effect->numPrograms             = 0;
    effect->version                 = 0x00000000; // TODO
    effect->initialDelay            = 0;
    effect->object                  = vst2plugin;
    effect->user                    = nullptr;//this;
    effect->dispatcher              = vst2_dispatcher_callback;
    effect->setParameter            = vst2_setParameter_callback;
    effect->getParameter            = vst2_getParameter_callback;
    effect->processReplacing        = vst2_process_callback;
    effect->processDoubleReplacing  = vst2_processDouble_callback;

    return effect;
  }

//------------------------------
private: // vst2 callbacks
//------------------------------

  static
  VstIntPtr vst2_dispatcher_callback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt) {
    VstIntPtr result = 0;
    SAT_Vst2Plugin* vst2_plugin = (SAT_Vst2Plugin*)effect->object;
    result = vst2_plugin->vst2_dispatcher(opcode,index,value,ptr,opt);
    if (opcode == effClose) {
      delete (SAT_Vst2Plugin*)vst2_plugin;
    }
    return result;
  }

  //----------

  static
  void vst2_setParameter_callback(AEffect* effect, VstInt32 index, float parameter) {
    SAT_Vst2Plugin* vst2_plugin = (SAT_Vst2Plugin*)effect->object;
    vst2_plugin->vst2_setParameter(index,parameter);
  }

  //----------

  static
  float vst2_getParameter_callback(AEffect* effect, VstInt32 index) {
    SAT_Vst2Plugin* vst2_plugin = (SAT_Vst2Plugin*)effect->object;
    return vst2_plugin->vst2_getParameter(index);
  }

  //----------

  static
  void vst2_process_callback(AEffect* effect, float** inputs, float** outputs, VstInt32 sampleFrames) {
    SAT_Vst2Plugin* vst2_plugin = (SAT_Vst2Plugin*)effect->object;
    vst2_plugin->vst2_process(inputs,outputs,sampleFrames);
  }

  //----------

  static
  void vst2_processDouble_callback(AEffect* effect, double** inputs, double** outputs, VstInt32 sampleFrames) {
    SAT_Vst2Plugin* vst2_plugin = (SAT_Vst2Plugin*)effect->object;
    vst2_plugin->vst2_processDouble(inputs,outputs,sampleFrames);
  }

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

#define SAT_VST2_ENTRY_SYMBOL asm ("VSTPluginMain");
AEffect* sat_vst2_entry(audioMasterCallback audioMaster) SAT_VST2_ENTRY_SYMBOL

//----------

// ugh...
//TODO: move into SAT_GLOBAL.VST2 ??

SAT_Vst2Entry GLOBAL_VST2_PLUGIN_ENTRY;

//----------

//__SAT_EXPORT
__attribute__ ((visibility ("default")))
AEffect* sat_vst2_entry(audioMasterCallback audioMaster) {
  SAT_Print("\n");
  if (!audioMaster(0,audioMasterVersion,0,0,0,0)) return 0;
  return GLOBAL_VST2_PLUGIN_ENTRY.entry(audioMaster);
}

//

#undef SAT_VST2_ENTRY_SYMBOL

//----------------------------------------------------------------------
#endif

