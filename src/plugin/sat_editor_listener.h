#ifndef sat_editor_listener_included
#define sat_editor_listener_included
//----------------------------------------------------------------------

class SAT_EditorListener {
  virtual void on_editor_listener_parameter_update(uint32_t AIndex, sat_param_t AValue) {}
};

//----------------------------------------------------------------------
#endif
