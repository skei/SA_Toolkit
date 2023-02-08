#ifndef sat_editor_included
#define sat_editor_included
//----------------------------------------------------------------------

/*
  * delay window creation until first show()
  * delete window in destructor (if created)
*/

#include "plugin/sat_editor_listener.h"
#include "gui/sat_window.h"
#include "gui/sat_window_listener.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Editor
: public SAT_WindowListener {

//------------------------------
private:
//------------------------------

  uint32_t              MWidth        = 0;
  uint32_t              MHeight       = 0;
  bool                  MProportional = false;
  double                MScale        = 1.0;
  const clap_window_t*  MParent       = nullptr;
  const char*           MTitle        = "";
  bool                  MIsOpen       = false;
  SAT_EditorListener*   MListener     = nullptr;
  SAT_Window*           MWindow       = nullptr;

  //uint32_t MPendingWidth
  //uint32_t MPendingHeight
  //uint32_t MPendingScale

//------------------------------
public:
//------------------------------

  SAT_Editor(SAT_EditorListener* AListener) {
  }

  //----------

  virtual ~SAT_Editor() {
    if (MWindow) {
      delete MWindow;
    }
  }

//------------------------------
public: // clap
//------------------------------

  bool create() {
    return true;
  }

  //----------

  void destroy() {
  }

  //----------

  bool set_scale(double scale) {
    MScale = scale;
    return true;
  }

  //----------

  bool get_size(uint32_t *width, uint32_t *height) {
    *width = MWidth;
    *height = MHeight;
    return true;
  }

  //----------

  bool can_resize() {
    return true;
  }

  //----------

  bool get_resize_hints(clap_gui_resize_hints_t *hints) {
    hints->can_resize_horizontally  = true;
    hints->can_resize_vertically    = true;
    hints->preserve_aspect_ratio    = false;
    hints->aspect_ratio_width       = MWidth;
    hints->aspect_ratio_height      = MHeight;
    return true;
  }

  //----------

  bool adjust_size(uint32_t *width, uint32_t *height) {
    return true;
  }

  //----------

  bool set_size(uint32_t width, uint32_t height) {
    MWidth = width;
    MHeight = height;
    return true;
  }

  //----------

  bool set_parent(const clap_window_t *window) {
    MParent = window;
    return true;
  }

  //----------

  bool set_transient(const clap_window_t *window) {
    return true;
  }

  //----------

  void suggest_title(const char *title) {
    MTitle = title;
  }

  //----------

  bool show() {
    if (!MWindow) {
      MWindow = new SAT_Window(MWidth,MHeight,MParent->x11);
      MWindow->show();
    }
    if (!MIsOpen) {
      MIsOpen = true;
      //...
    }
    return true;
  }

  //----------

  bool hide() {
    if (MIsOpen) {
      MWindow->hide();
      MIsOpen = false;
      //..
    }
    return true;
  }

//------------------------------
private:
//------------------------------

  //void openWindow(); {}
  //void closeWindow(); {}
  //void startTimer() {}
  //void stopTimer() {}

};

//----------------------------------------------------------------------
#endif
