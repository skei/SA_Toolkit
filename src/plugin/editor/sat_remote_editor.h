#ifndef sat_remote_editor_included
#define sat_remote_editor_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/editor/sat_editor_listener.h"
#include "plugin/editor/sat_base_editor.h"
#include "plugin/sat_parameter.h"


class SAT_RemoteEditor
: public SAT_BaseEditor {

//------------------------------
private:
//------------------------------

  SAT_EditorListener* MListener       = nullptr;
  uint32_t            MWidth          = 512;
  uint32_t            MHeight         = 512;
  double              MScale          = 1.0;
  const char*         MTitle          = "";
  bool                MProportional   = false;

//------------------------------
public:
//------------------------------

  SAT_RemoteEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, double AScale=1.0, bool AProportional=false)
  : SAT_BaseEditor(AListener,AWidth,AHeight,AScale,AProportional) {
    MListener       = AListener;
    MWidth          = AWidth;
    MHeight         = AHeight;
    MScale          = AScale;
    MProportional   = AProportional;
  }

  //----------

  virtual ~SAT_RemoteEditor() {
  }

//------------------------------
public:
//------------------------------

  // bool connect(SAT_Widget* AWidget, SAT_Parameter* AParameter, uint32_t AIndex=0) override {
  //   return false;
  // }

  //----------

  void setParameterValue(SAT_Parameter* AParameter) override {
  }

  //----------

  void updateParameterFromHost(SAT_Parameter* AParameter, double AValue) override {
  }

  //----------

  void updateModulationFromHost(SAT_Parameter* AParameter, double AValue) override {
  }

//------------------------------
public: // clap.gui
//------------------------------

  bool isApiSupported(const char *api, bool is_floating) override {
    return false;
  }

  //----------

  bool getPreferredApi(const char **api, bool *is_floating) override {
    return false;
  }

  //----------

  bool create(const char *api, bool is_floating) override {
    return false;
  }
  
  //----------

  void destroy() override {
  }

  //----------

  bool setScale(double scale) override {
    return false;
  }

  //----------

  bool getSize(uint32_t *width, uint32_t *height) override {
    return false;
  }

  //----------

  bool canResize() override {
    return false;
  }

  //----------

  bool getResizeHints(clap_gui_resize_hints_t *hints) override {
    return false;
  }

  //----------

  bool adjustSize(uint32_t *width, uint32_t *height) override {
    return false;
  }

  //----------

  bool setSize(uint32_t width, uint32_t height) override {
    return false;
  }

  //----------

  bool setParent(const clap_window_t *window) override {
    return false;
  }

  //----------

  bool setTransient(const clap_window_t *window) override {
    return false;
  }

  //----------

  void suggestTitle(const char *title) override {
  }

  //----------

  bool show() override {
    return false;
  }

  //----------

  bool hide() override {
    return false;
  }
  
};

//----------------------------------------------------------------------
#endif
