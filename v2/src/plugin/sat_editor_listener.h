#ifndef sat_editor_listener_included
#define sat_editor_listener_included
//----------------------------------------------------------------------

//editor -> plugin

#include "sat.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_EditorListener {
//protected:
public:
  virtual void on_editorListener_timer() {}
  virtual void on_editorListener_update(uint32_t AIndex, sat_param_t AValue) {}
};

//----------------------------------------------------------------------
#endif
