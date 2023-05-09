#ifndef sat_vst2_included
#define sat_vst2_included
//----------------------------------------------------------------------

#include "base/sat.h"

#define SAT_VST2_NO_SHELL

//#ifdef SAT_PLUGIN_VST2_VESTIGE
//  typedef int32_t   VstInt32;
//  typedef intptr_t  VstIntPtr;
//  struct  ERect { int16_t top, left, bottom, right; };
//  #include "extern/vst2/vestige.h"
//#else

  #ifdef SAT_LINUX
    #define __cdecl
  #endif

  #include "extern/vst2/vst2sdk/aeffect.h"
  #include "extern/vst2/vst2sdk/aeffectx.h"

//#endif

//----------------------------------------------------------------------
#endif
