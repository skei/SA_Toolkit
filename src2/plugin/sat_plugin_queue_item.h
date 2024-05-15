#ifndef sat_plugin_queue_item_included
#define sat_plugin_queue_item_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/sat_note.h"

// 128 bit
struct SAT_PluginQueueItem {
  union {
    uint32_t    index;
    uint32_t    noteid;
  };
  uint32_t dummy;
  union {
    double    value;
    SAT_Note  note; // 64bit
  };
};

//----------------------------------------------------------------------
#endif



// param value
