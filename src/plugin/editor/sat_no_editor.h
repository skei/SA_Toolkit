#ifndef sat_no_editor_included
#define sat_no_editor_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/sat_plugin_base.h"

//----------

// struct clap_gui_resize_hints_t;
// struct clap_window_t;
// class SAT_EditorListener;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_NoEditor {

//------------------------------
public:
//------------------------------

  SAT_NoEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, double AScale=1.0, bool AProportional=false) {
  }

  //----------

  virtual ~SAT_NoEditor() {
  }

//------------------------------
public:
//------------------------------

  /*

  bool isApiSupported(const char *api, bool is_floating) override {
    return false;
  }

  bool getPreferredApi(const char **api, bool *is_floating) override {
    return false;
  }

  bool create(const char *api, bool is_floating) override {
    return false;
  }

  void destroy() override {
  }

  bool setScale(double scale) override {
    return false;
  }

  bool getSize(uint32_t *width, uint32_t *height) override {
    return false;
  }

  bool canResize() override {
    return false;
  }

  bool getResizeHints(clap_gui_resize_hints_t *hints) override {
    return false;
  }

  bool adjustSize(uint32_t *width, uint32_t *height) override {
    return false;
  }

  bool setSize(uint32_t width, uint32_t height) override {
    return false;
  }

  bool setParent(const clap_window_t *window) override {
    return false;
  }

  bool setTransient(const clap_window_t *window) override {
    return false;
  }

  void suggestTitle(const char *title) override {
  }

  bool show() override {
    return false;
  }

  bool hide() override {
    return false;
  }

  */

};

//----------------------------------------------------------------------
#endif
