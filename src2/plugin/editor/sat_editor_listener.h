#ifndef sat_editor_listener_included
#define sat_editor_listener_included
//----------------------------------------------------------------------
// aka plugin
// (from editor to plugin)

#include "sat.h"
#include "base/system/sat_timer.h"

class SAT_Window;

//----------------------------------------------------------------------

class SAT_EditorListener {
public:
  virtual void        on_editorListener_update(uint32_t AIndex, sat_param_t AValue) {}
  virtual void        on_editorListener_timer(SAT_Timer* ATimer) {}

  #ifdef SAT_EDITOR_EMBEDDED
  virtual SAT_Window* on_editorListener_createWindow(uint32_t AWidth, uint32_t AHeight) { return nullptr; }
  virtual void        on_editorListener_deleteWindow(SAT_Window* AWindow) {}
  #endif
};

//----------------------------------------------------------------------
#endif


