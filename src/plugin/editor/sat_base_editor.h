#ifndef sat_base_editor_included
#define sat_base_editor_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/editor/sat_editor_listener.h"

class SAT_BaseEditor {

//------------------------------
public:
//------------------------------

  SAT_BaseEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight/*, double AScale=1.0, bool AProportional=false*/) {
  }

  SAT_BaseEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, double AScale = 1.0, bool AProportional = false) {
  }

  //----------

  virtual ~SAT_BaseEditor() {
  }

//------------------------------
public:
//------------------------------

  // virtual bool connect(SAT_Widget* AWidget, SAT_Parameter* AParameter, uint32_t AIndex=0) { return false; }
  virtual void setParameterValue(SAT_Parameter* AParameter) {}
  virtual void updateParameterFromHost(SAT_Parameter* AParameter, double AValue) {}
  virtual void updateModulationFromHost(SAT_Parameter* AParameter, double AValue) {}

//------------------------------
public:
//------------------------------

  virtual bool isApiSupported(const char *api, bool is_floating) { return false; }
  virtual bool getPreferredApi(const char **api, bool *is_floating) { return false; }
  virtual bool create(const char *api, bool is_floating) { return false; }
  virtual void destroy() {}
  virtual bool setScale(double scale) { return false; }
  virtual bool getSize(uint32_t *width, uint32_t *height) { return false; }
  virtual bool canResize() { return false; }
  virtual bool getResizeHints(clap_gui_resize_hints_t *hints) { return false; }
  virtual bool adjustSize(uint32_t *width, uint32_t *height) { return false; }
  virtual bool setSize(uint32_t width, uint32_t height) { return false; }
  virtual bool setParent(const clap_window_t *window) { return false; }
  virtual bool setTransient(const clap_window_t *window) { return false; }
  virtual void suggestTitle(const char *title) {}
  virtual bool show() { return false; }
  virtual bool hide() { return false; }


};

//----------------------------------------------------------------------
#endif
