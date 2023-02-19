#ifndef sat_editor_listener_included
#define sat_editor_listener_included
//----------------------------------------------------------------------

#include "plugin/sat_parameter.h"

//----------

class SAT_EditorListener {
public:
  virtual void            do_editor_listener_timer() {}
  virtual void            do_editor_listener_parameter_update(uint32_t AIndex, sat_param_t AValue) {}
  //virtual SAT_Parameter*  do_editor_listener_get_parameter(uint32_t AIndex) { return nullptr; }
  //virtual sat_param_t     do_editor_listener_get_parameter_value(uint32_t AIndex) { return 0.0; }
  //virtual sat_param_t     do_editor_listener_get_modulation_value(uint32_t AIndex) { return 0.0; }
};

//----------------------------------------------------------------------
#endif
