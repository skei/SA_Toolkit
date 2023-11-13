#ifndef sat_vst2_utils_included
#define sat_vst2_utils_included
//----------------------------------------------------------------------

#include "plugin/vst2/sat_vst2.h"

uint32_t sat_vst2_create_unique_id(const clap_plugin_descriptor_t* descriptor) {
  char buffer [1024];
  buffer[0] = 0;
  //strcat(buffer,"MIP_MAGIC_M_PL");
  strcat(buffer,descriptor->name);
  strcat(buffer,descriptor->vendor);
  strcat(buffer,descriptor->version); // we probably don't want version in the id..
  return SAT_HashString(buffer);
}

//----------------------------------------------------------------------
#endif
