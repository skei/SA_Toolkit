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

  SAT_Editor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight) {
    MWidth = AWidth;
    MHeight = AHeight;
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
    SAT_Print("scale %f\n",scale);
    MScale = scale;
    //if (MWindow) MWindow->setScale(scale);
    return true;
  }

  //----------

  bool get_size(uint32_t *width, uint32_t *height) {
    SAT_Print("-> %i,%i\n",MWidth,MHeight);
    *width = MWidth;
    *height = MHeight;
    return true;
  }

  //----------

  bool can_resize() {
    //SAT_Print("\n");
    return true;
  }

  //----------

  bool get_resize_hints(clap_gui_resize_hints_t *hints) {
    SAT_Print("\n");
    hints->can_resize_horizontally  = true;
    hints->can_resize_vertically    = true;
    hints->preserve_aspect_ratio    = false;
    hints->aspect_ratio_width       = MWidth;
    hints->aspect_ratio_height      = MHeight;
    return true;
  }

  //----------

  /*
    *width/*height contains 'suggested' (resized) size
    return wanted size via same ptrs
    bitwig seems to call this repeatedly until the
    suggested, and returned sizes are equal
    (be careful with rounding for scaling)
  */

  bool adjust_size(uint32_t *width, uint32_t *height) {
    SAT_Print("(%i,%i) -> %i,%i\n",*width,*height,MWidth,MHeight);
    MWidth = *width;
    MHeight = *height;
    //*width = MWidth;
    //*height = MHeight;
    return true;
  }

  //----------

  bool set_size(uint32_t width, uint32_t height) {
    SAT_Print("width %i height %i\n",width,height);
    MWidth = width;
    MHeight = height;
    if (MWindow) {
      MWindow->setSize(width,height);
      MWindow->on_window_resize(width,height);
      //MWindow->paint();
      //MWindow->invalidate(0,0,width,height);
    }
    return true;
  }

  //----------

  bool set_parent(const clap_window_t *window) {
    SAT_Print("\n");
    MParent = window;
    if (!MWindow) {
      MWindow = new SAT_Window(MWidth,MHeight,window->x11);
    }
    else {
      MWindow->reparent(window->x11);
    }
    return true;
  }

  //----------

  bool set_transient(const clap_window_t *window) {
    SAT_Print("\n");
    return true;
  }

  //----------

  void suggest_title(const char *title) {
    SAT_Print("title %s\n",title);
    MTitle = title;
    if (MWindow) MWindow->setTitle(title);
  }

  //----------

  bool show() {
    SAT_Print("\n");
    //if (!MWindow) MWindow = new SAT_Window(MWidth,MHeight,MParent);
    if (MWindow && !MIsOpen) {
      MWindow->show();
      //MWindow->paint();
      //MWindow->invalidate(0,0,MWidth,MHeight);
      MIsOpen = true;
    }
    return true;
  }

  //----------

  bool hide() {
    SAT_Print("\n");
    if (MWindow && MIsOpen) {
      MWindow->hide();
      MIsOpen = false;
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
