#ifndef sat_plugin_queue_item_included
#define sat_plugin_queue_item_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/sat_note.h"

// 128 bit
struct SAT_PluginQueueItem {
  union {
    uint32_t  param_id;
    int32_t   note_id;
  };
  uint32_t    dummy;
  union {
    double    value;
    SAT_Note  note;
  };
};

//----------------------------------------------------------------------
#endif



// param value
