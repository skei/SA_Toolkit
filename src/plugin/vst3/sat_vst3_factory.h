#ifndef sat_vst3_factory_included
#define sat_vst3_factory_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/util/sat_strutils.h"
#include "plugin/sat_host_implementation.h"
#include "plugin/sat_plugin.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/vst3/sat_vst3.h"
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

class SAT_Vst3Factory
: public IPluginFactory3 {

//------------------------------
private:
//------------------------------

  uint32_t    MRefCount       = 1;
  FUnknown*   MHostContext    = nullptr;
  
  char        MPluginId[16]   = {0};
  char        MEditorId[16]   = {0};

//------------------------------
public:
//------------------------------

  SAT_Vst3Factory() {
    SAT_TRACE;
    MRefCount = 1;
  }

  //----------

  virtual ~SAT_Vst3Factory() {
    SAT_TRACE;
  }

//------------------------------
private:
//------------------------------

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

//  bool isInstrument(const clap_plugin_descriptor_t* descriptor) {
//    const char* const* feature = descriptor->features;
//    uint32_t index = 0;
//    while (feature[index]) {
//      if (strcmp(feature[index], CLAP_PLUGIN_FEATURE_INSTRUMENT) == 0) return true;
//      index++;
//    }
//    return false;
//  }

//------------------------------
public: // FUnknown
//------------------------------

  uint32 PLUGIN_API addRef() override {
    SAT_TRACE;
    MRefCount++;
    return MRefCount;
  }

  //----------

  uint32 PLUGIN_API release() override {
    SAT_TRACE;
    uint32_t r = --MRefCount;
    if (r == 0) {
      SAT_PRINT("deleting\n");
      delete this;
    }
    return r;
  }

  //----------

  tresult PLUGIN_API queryInterface(const TUID _iid, void** obj) override {
    SAT_PRINT("_iid: ");
    VST3_PrintIID(_iid);
    SAT_DPRINT("\n");
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

//------------------------------
public: // IPluginFactory
//------------------------------

  tresult PLUGIN_API getFactoryInfo(PFactoryInfo* info) override {
    SAT_TRACE;
    strcpy(info->vendor,"<factory author>");
    strcpy(info->url,   "<factory url>"   );
    strcpy(info->email, "<factory email>" );
    info->flags = PFactoryInfo::kNoFlags;
    return kResultOk;
  }

  //----------

  int32 PLUGIN_API countClasses() override {
    SAT_TRACE;
    return SAT_GLOBAL.REGISTRY.getNumDescriptors();
  }

  //----------
  
  tresult PLUGIN_API getClassInfo(int32 index, PClassInfo* info) override {
    SAT_PRINT("index %i\n",index);
    const clap_plugin_descriptor_t* descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(index);
    const char* long_id = getLongId(descriptor);
    memcpy(info->cid,long_id,16);
    info->cardinality = PClassInfo::kManyInstances;

//    if (SAT_ClapIsInstrument(descriptor)) strcpy(info->subCategories,Vst::PlugType::kInstrument);
//    else strcpy(info->subCategories,Vst::PlugType::kFx);

    strncpy(info->category,kVstAudioEffectClass,PClassInfo::kCategorySize);
    
    // ugly..
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstringop-truncation"
      strncpy(info->name,descriptor->name,PClassInfo::kNameSize);
    #pragma GCC diagnostic pop

    return kResultOk;
  }

  //----------

  tresult PLUGIN_API createInstance(FIDString cid, FIDString _iid, void** obj) override {
    
    SAT_PRINT("cid ");    VST3_PrintIID(cid);
    SAT_DPRINT(" _iid "); VST3_PrintIID(_iid);
    SAT_DPRINT("\n");
    
    int32_t index = findPluginIndex(cid);
    if (index < 0) return kNotImplemented;

    const clap_plugin_descriptor_t* descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(index);

//    SAT_Vst3HostImplementation* vst3_host = new SAT_Vst3HostImplementation();
    SAT_HostImplementation* vst3_host = new SAT_HostImplementation();

    SAT_ClapPlugin* plugin = SAT_CreatePlugin(index,descriptor,vst3_host->getClapHost());
    const clap_plugin_t* clapplugin = plugin->getClapPlugin();
    SAT_Plugin* satplugin = (SAT_Plugin*)clapplugin->plugin_data;
    satplugin->setPluginFormat("VST3");
    plugin->init();

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

//------------------------------
public: // IPluginFactory2
//------------------------------

  tresult PLUGIN_API getClassInfo2(int32 index, PClassInfo2* info) override {
    SAT_PRINT("index %i\n",index);
    const clap_plugin_descriptor_t* descriptor = SAT_GLOBAL.REGISTRY.getDescriptor(index);
    const char* long_id = getLongId(descriptor);
    memcpy(info->cid,long_id,16);
    info->cardinality = PClassInfo::kManyInstances;
    strcpy(info->category,kVstAudioEffectClass);
    strcpy(info->name,descriptor->name);
    info->classFlags = 0;
    
//    if (SAT_ClapIsInstrument(descriptor)) strcpy(info->subCategories,Vst::PlugType::kInstrument);
//    else
    strcpy(info->subCategories,Vst::PlugType::kFx);

    strcpy(info->vendor,descriptor->vendor);
    strcpy(info->version,descriptor->version);
    strcpy(info->sdkVersion,kVstVersionString);
    return kResultOk;
  }

  //--------------------
  // IPluginFactory3
  //--------------------

  tresult PLUGIN_API getClassInfoUnicode(int32 index, PClassInfoW* info) override {
    SAT_PRINT("index %i\n",index);
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API setHostContext(FUnknown* context) override {
    SAT_TRACE;
    MHostContext = context;
    return kResultOk;
  }

};

//----------------------------------------------------------------------
#endif