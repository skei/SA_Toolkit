#ifndef sat_vst3_entry_included
#define sat_vst3_entry_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/vst3/sat_vst3.h"
#include "plugin/vst3/sat_vst3_factory.h"

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
  return new SAT_Vst3Factory();
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
