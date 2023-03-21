#ifndef sat_vst3_entry_included
#define sat_vst3_entry_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/utils/sat_strutils.h"
#include "plugin/sat_plugin.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/vst3/sat_vst3.h"
#include "plugin/vst3/sat_vst3_host_implementation.h"
//#include "plugin/vst3/sat_vst3_host.h"
#include "plugin/vst3/sat_vst3_plugin.h"
#include "plugin/vst3/sat_vst3_utils.h"

//#ifndef SAT_NO_GUI
//#include "plugin/sat_editor.h"
//#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Vst3Entry
: public IPluginFactory3 {

//------------------------------
private:
//------------------------------

  uint32_t      MRefCount     = 1;
  FUnknown*     MHostContext  = nullptr;
  char          MPluginId[16] = {0};
  char          MEditorId[16] = {0};

//------------------------------
public:
//------------------------------

  SAT_Vst3Entry() {
    SAT_Print("SAT_Vst3Entry\n");
    MRefCount = 1;
  }

  //----------

  virtual ~SAT_Vst3Entry() {
    SAT_Print("~SAT_Vst3Entry\n");
  }

//------------------------------
private:
//------------------------------


  //#define SAT_MAGIC_M_PL   0x4d5f504c    // M_PL   // plugin
  //#define SAT_MAGIC_M_ED   0x4d5f4544    // M_ED   // editor

  //----------

  const char* getLongId(const clap_plugin_descriptor_t* descriptor) {
    uint32_t* id = (uint32_t*)MPluginId;
    id[0] = SAT_MAGIC_PLUGIN;
    id[1] = SAT_HashString(descriptor->name);
    id[2] = SAT_HashString(descriptor->vendor);
    id[3] = SAT_HashString(descriptor->version);
    return (const char*)id;
  }

  //----------

  int32_t findPluginIndex(const char* cid) {
    uint32_t num = SAT_GLOBAL.REGISTRY.getNumDescriptors();
    for (uint32_t i=0; i<num; i++) {
      const clap_plugin_descriptor_t* desc = SAT_GLOBAL.REGISTRY.getDescriptor(i);
      const char* id = getLongId(desc);
      if (VST3_iidEqual(cid,id)) { return i; }
      
      
    }
    return -1;
  }

  //----------

  bool isInstrument(const clap_plugin_descriptor_t* descriptor) {
    const char* const* feature = descriptor->features;
    uint32_t index = 0;
    while (feature[index]) {
      if (strcmp(feature[index], "instrument") == 0) return true;
      index++;
    }
    return false;
  }


//------------------------------
public:
//------------------------------

  //--------------------
  // FUnknown
  //--------------------

  uint32 PLUGIN_API addRef() override {
    SAT_Print("SAT_Vst3Entry.addRef\n");
    MRefCount++;
    return MRefCount;
  }

  //----------

  uint32 PLUGIN_API release() override {
    SAT_Print("SAT_Vst3Entry.release\n");
    const uint32_t r = --MRefCount;
    if (r == 0) delete this;
    return r;
  }

  //----------

  tresult PLUGIN_API queryInterface(const TUID _iid, void** obj) override {
    SAT_Print("SAT_Vst3Entry.queryInterface\n");
    if (VST3_iidEqual(_iid,IPluginFactory2_iid)) {
      *obj = (IPluginFactory2*)this;
      addRef();
      return kResultOk;
    }
    if (VST3_iidEqual(_iid,IPluginFactory3_iid)) {
      *obj = (IPluginFactory3*)this;
      addRef();
      return kResultOk;
    }
    *obj = nullptr;
    return kNoInterface;
  }

  //--------------------
  // IPluginFactory
  //--------------------

  tresult PLUGIN_API getFactoryInfo(PFactoryInfo* info) override {
    SAT_Print("SAT_Vst3Entry.getFactoryInfo\n");
    strcpy(info->vendor,"<factory author>");
    strcpy(info->url,"<factory url>");
    strcpy(info->email,"<factory email>");
    info->flags = PFactoryInfo::kNoFlags;
    return kResultOk;
  }

  //----------


  int32   PLUGIN_API countClasses() override {
    SAT_Print("SAT_Vst3Entry.countClasses\n");
    return SAT_GLOBAL.REGISTRY.getNumDescriptors();
  }

  //----------

  tresult PLUGIN_API getClassInfo(int32 index, PClassInfo* info) override {
    SAT_Print("SAT_Vst3Entry.getClassInfo\n");
    const clap_plugin_descriptor_t* descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(index);
    const char* long_id = getLongId(descriptor);
    memcpy(info->cid,long_id,16);
    info->cardinality = PClassInfo::kManyInstances;
    strncpy(info->category,kVstAudioEffectClass,PClassInfo::kCategorySize);
    strncpy(info->name,descriptor->name,PClassInfo::kNameSize);
    return kResultOk;
  }

  //----------

  tresult PLUGIN_API createInstance(FIDString cid, FIDString _iid, void** obj) override {
    
    int32_t index = findPluginIndex(cid);
    if (index < 0) return kNotImplemented;
    
    const clap_plugin_descriptor_t* descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(index);

    SAT_Vst3HostImplementation* vst3_host = new SAT_Vst3HostImplementation();
    
    //SAT_Vst3Host* vst3_host = new SAT_Vst3Host();
    
    const clap_plugin_t* clapplugin = SAT_CreatePlugin(index,descriptor,vst3_host->getHost());
    SAT_Plugin* plugin = (SAT_Plugin*)clapplugin->plugin_data;
    
SAT_PRINT;

    plugin->init();

SAT_PRINT;
    
    SAT_Vst3Plugin* vst3plugin = new SAT_Vst3Plugin(plugin);

    /*
    Plugin* plug = (Plugin*)plugin;
    uint32_t num_par = plug->getParameterCount();
    for (uint32_t i=0; i<num_par; i++) {
      //clap_param_info_t info;
      //MPlugin->params_get_info(id,&info);
      Parameter* param = plug->getParameter(i);
      double value = param->getDefaultValue();
      //value = param->normalize(value);
      param->setValue(value);
    }
    */

    /*
    ParameterArray* pa = pl->getParameters();
    vst3plugin->setParameters(pa);
    plugin->setListener(vst3_instance);
    plugin->on_plugin_open();
    plugin->setDefaultParameterValues();
    plugin->updateAllParameters();
    */

    *obj = (Vst::IComponent*)vst3plugin;
    return kResultOk;

    //return kNotImplemented;
  }

  //--------------------
  // IPluginFactory2
  //--------------------

  tresult PLUGIN_API getClassInfo2(int32 index, PClassInfo2* info) override {
    SAT_Print("Vst3Entry.getClassInfo2\n");
    const clap_plugin_descriptor_t* descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(index);
    const char* long_id = getLongId(descriptor);
    memcpy(info->cid,long_id,16);
    info->cardinality = PClassInfo::kManyInstances;
    strcpy(info->category,kVstAudioEffectClass);
    strcpy(info->name,descriptor->name);
    info->classFlags = 0;
    if (isInstrument(descriptor)) strcpy(info->subCategories,Vst::PlugType::kInstrument);
    else strcpy(info->subCategories,Vst::PlugType::kFx);
    strcpy(info->vendor,descriptor->vendor);
    strcpy(info->version,descriptor->version);
    strcpy(info->sdkVersion,kVstVersionString);
    return kResultOk;
  }

  //--------------------
  // IPluginFactory3
  //--------------------

  tresult PLUGIN_API getClassInfoUnicode(int32 index, PClassInfoW* info) override {
    SAT_Print("Vst3Entry.getClassInfoUnicode\n");
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API setHostContext(FUnknown* context) override {
    SAT_Print("Vst3Entry.setHostContext\n");
    MHostContext = context;
    return kResultOk;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#define VST3_ENTRY_SYMBOL asm ("GetPluginFactory");
//IPluginFactory* PLUGIN_API sat_vst3_entry() VST3_ENTRY_SYMBOL

IPluginFactory* PLUGIN_API sat_vst3_entry() asm ("GetPluginFactory");
bool vst3_module_entry(void* sharedLibraryHandle) asm ("ModuleEntry");
bool vst3_module_exit(void) asm ("ModuleExit");

//----------

//__EXPORT
__attribute__ ((visibility ("default")))
IPluginFactory* PLUGIN_API sat_vst3_entry() {
  SAT_Print("GetPluginFactory\n");
  printf("GetPluginFactory\n");
  return new SAT_Vst3Entry();
}

//----------

//void* moduleHandle = nullptr;
//static int counter {0};

//__EXPORT
__attribute__ ((visibility ("default")))
bool vst3_module_entry(void* sharedLibraryHandle)  {
  SAT_Print("ModuleEntry\n");
  printf("ModuleEntry\n");
  //if (++counter == 1) {
  //  moduleHandle = sharedLibraryHandle;
  //  return true;
  //}
  return true;
}

//__EXPORT
__attribute__ ((visibility ("default")))
bool vst3_module_exit(void) {
  SAT_Print("ModuleExit\n");
  printf("ModuleExit\n");
  //if (--counter == 0) {
  //  moduleHandle = nullptr;
  //  return true;
  //}
  return true;
}


//----------------------------------------------------------------------
#endif
