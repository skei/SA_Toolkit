#ifndef sat_vst3_entry_included
#define sat_vst3_entry_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/vst3/sat_vst3.h"
#include "plugin/vst3/sat_vst3_factory.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

IPluginFactory* PLUGIN_API sat_vst3_entry() asm ("GetPluginFactory");
bool vst3_module_entry(void* sharedLibraryHandle) asm ("ModuleEntry");
bool vst3_module_exit(void) asm ("ModuleExit");

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// host deletes it? or else it leaks..
// should we have a global/static factory, and return a ptr to this?
// will the host try to delete it?

//__EXPORT
__attribute__ ((visibility ("default")))
IPluginFactory* PLUGIN_API sat_vst3_entry() {
  SAT_TRACE;
  return new SAT_Vst3Factory();
}

//----------

//void* moduleHandle = nullptr;
//static int counter {0};

//__EXPORT
__attribute__ ((visibility ("default")))
bool vst3_module_entry(void* sharedLibraryHandle)  {
  SAT_TRACE;
  //if (++counter == 1) {
  //  moduleHandle = sharedLibraryHandle;
  //  return true;
  //}
  return true;
}

//----------

//__EXPORT
__attribute__ ((visibility ("default")))
bool vst3_module_exit(void) {
  SAT_TRACE;
  //if (--counter == 0) {
  //  moduleHandle = nullptr;
  //  return true;
  //}
  return true;
}

//----------------------------------------------------------------------
#endif