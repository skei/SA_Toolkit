#ifndef sat_lv2_utils_included
#define sat_lv2_utils_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/lv2/sat_lv2.h"

//----------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------

void SAT_Lv2PrintFeatures(const LV2_Feature* const* features) {
  uint32_t i = 0;
  while ( features[i] ) {
    const char* uri = features[i]->URI;
    SAT_DPrint("	 %i: %s\n",i,uri);
    i++;
  }
}

//----------

void* SAT_Lv2FindFeature(const char* name, const LV2_Feature* const* features) {
  for (uint32_t i=0; features[i]; ++i) {
    if (strcmp(features[i]->URI, name) == 0) {
      void* data = features[i]->data;
      SAT_Print("found it! features[%i] == %s. data %p\n",i,features[i]->URI,name,data);
      return data;
    }
  }
  return nullptr;
}

//----------

LV2_URID SAT_Lv2MapUrid(const char* name, LV2_URID_Map* urid_map) {
  return urid_map->map(urid_map->handle, name);
}

//----------------------------------------------------------------------
#endif


