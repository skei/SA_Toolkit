#ifndef sat_editor_listener_included
#define sat_editor_listener_included
//----------------------------------------------------------------------

// listener = usually SAT_Plugin

#include "plugin/sat_parameter.h"
#include "gui/sat_window.h"

//----------

class SAT_EditorListener {
public:
  virtual void            do_editorListener_timer(SAT_Window* ASender, double AElapsed)           {}
  virtual void            do_editorListener_parameter_update(uint32_t AIndex, sat_param_t AValue) {}                      // value, clap-space or 0..1 space?
//virtual SAT_Parameter*  do_editorListener_get_parameter(uint32_t AIndex)                        { return nullptr; }
//virtual sat_param_t     do_editorListener_get_parameter_value(uint32_t AIndex)                  { return 0.0; }
//virtual sat_param_t     do_editorListener_get_modulation_value(uint32_t AIndex)                 { return 0.0; }
};

//----------------------------------------------------------------------
#endif
