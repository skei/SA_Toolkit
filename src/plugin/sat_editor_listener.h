#ifndef sat_editor_listener_included
#define sat_editor_listener_included
//----------------------------------------------------------------------

//editor -> plugin

#include "sat.h"

class SAT_Parameter;
class SAT_Timer;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_EditorListener {
public:
  virtual void on_editorListener_timer(SAT_Timer* ATimer, double AElapsed) {}
  virtual void on_editorListener_update(SAT_Parameter* AParameter, sat_param_t AValue) {}
};

//----------------------------------------------------------------------
#endif
