#ifndef sat_vst3_included
#define sat_vst3_included
//----------------------------------------------------------------------

// see also travesty:
// https://github.com/DISTRHO/DPF/tree/develop/distrho/src/travesty

// -I../include/extern/vst3

#include "sat.h"

//----------------------------------------------------------------------

#define SAT_PLUGIN_VST3_PARAM_AFTERTOUCH  0x10000 // kAfterTouch (128)
#define SAT_PLUGIN_VST3_PARAM_PITCHBEND   0x20000 // kPitchBend (129)
#define SAT_PLUGIN_VST3_PARAM_BRIGHTNESS  0x30000 // kCtrlFilterResonance (74)

//----------------------------------------------------------------------

// no effect?

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wunknown-pragmas"
//#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
//#pragma GCC diagnostic ignored "-Wunused-parameter"
//#pragma GCC diagnostic ignored "-Wsign-conversion"
//#pragma GCC diagnostic ignored "-Wold-style-cast"
//#pragma GCC diagnostic ignored "-Wextra-semi"
//#pragma GCC diagnostic ignored "-Wignored-qualifiers"
//#pragma GCC diagnostic ignored "-Wundef"

  //------------------------------

  //#include "pluginterfaces/base/conststringtable.h"
  //#include "pluginterfaces/base/ustring.h"
  //#include "pluginterfaces/base/conststringtable.cpp"
  #include "pluginterfaces/base/coreiids.cpp"
  //#include "pluginterfaces/base/funknown.cpp"
  #include "pluginterfaces/base/ustring.cpp"
  #include "pluginterfaces/base/ipluginbase.h"
  #include "pluginterfaces/gui/iplugview.h"
  #include "pluginterfaces/vst/ivstaudioprocessor.h"
  #include "pluginterfaces/vst/ivstcomponent.h"

  //------------------------------

  // https://github.com/phil-monroe/midiplug/blob/master/SDKs/vst3/public.sdk/source/vst/vstsinglecomponenteffect.h
  // work around for the name clash of IComponent::setState and IEditController::setState
  #define setState setEditorState
  #define getState getEditorState
  #include "pluginterfaces/vst/ivsteditcontroller.h"
  #undef setState
  #undef getState

  //------------------------------

  #include "pluginterfaces/vst/ivstevents.h"
  #include "pluginterfaces/vst/ivsthostapplication.h"
  #include "pluginterfaces/vst/ivstmessage.h"
  #include "pluginterfaces/vst/ivstmidicontrollers.h"
  #include "pluginterfaces/vst/ivstnoteexpression.h"
  #include "pluginterfaces/vst/ivstparameterchanges.h"
  #include "pluginterfaces/vst/ivstunits.h"

  //------------------------------

//#pragma GCC diagnostic pop

//------------------------------------------------------------

using namespace Steinberg;
using namespace Vst;

#ifdef SAT_LINUX
  using namespace Steinberg::Linux;
#endif

//#ifdef SAT_WIN32
//  using namespace Steinberg::Win64;
//#endif


//----------------------------------------------------------------------
#endif