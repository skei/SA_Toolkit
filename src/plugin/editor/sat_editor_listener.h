#ifndef sat_editor_listener_included
#define sat_editor_listener_included
//----------------------------------------------------------------------
// aka SAT_Plugin

#include "sat.h"

class SAT_Timer;
class SAT_Window;
class SAT_WindowListener;

//----------

class SAT_EditorListener {
public:
  virtual void on_EditorListener_update(uint32_t AId, double AValue) {}
  virtual void on_EditorListener_timer(SAT_Timer* ATimer, double ADelta) {}

  #ifdef SAT_EDITOR_EMBEDDED
  virtual SAT_Window* on_EditorListener_createWindow(SAT_WindowListener* AListener, uint32_t AWidth, uint32_t AHeight) { return nullptr; }
  virtual void        on_EditorListener_deleteWindow(SAT_Window* AWindow) {}
  #endif

};

//----------------------------------------------------------------------
#endif