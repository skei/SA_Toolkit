#ifndef sat_editor_listener_included
#define sat_editor_listener_included
//----------------------------------------------------------------------
// aka plugin
// (from editor to plugin)

#include "sat.h"
#include "base/system/sat_timer.h"

class SAT_EditorListener {
public:
  virtual void on_editorListener_update(uint32_t AIndex, sat_param_t AValue) {}
  virtual void on_editorListener_timer(SAT_Timer* ATimer) {}
};

//----------------------------------------------------------------------
#endif


