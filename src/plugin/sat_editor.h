#ifndef sat_editor_included
#define sat_editor_included
//----------------------------------------------------------------------

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

  uint32_t              MWidth            = 0;
  uint32_t              MHeight           = 0;
  bool                  MProportional     = false;
  double                MScale            = 1.0;
  const clap_window_t*  MParent           = nullptr;
  const char*           MTitle            = "";
  bool                  MIsOpen           = false;
  SAT_EditorListener*   MListener         = nullptr;
  SAT_Window*           MWindow           = nullptr;
  bool                  MPreparedWidgets  = false;

//------------------------------
public:
//------------------------------

  SAT_Editor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight) {
    MListener = AListener;
    MWidth    = AWidth;
    MHeight   = AHeight;
  }

  //----------

  /*
    when our window is embedded, we don't get on_window_close events
  */

  virtual ~SAT_Editor() {
    if (MWindow) {
      MWindow->on_window_close();
      delete MWindow;
    }
  }

//------------------------------
public:
//------------------------------

  SAT_Window* getWindow() {
    return MWindow;
  }

  //----------

  // called from MIP_Plugin.updateEditorParameterValues()
  // - before editor is opened
  // - (loading state)

  //virtual
  void updateEditorParameterValue(uint32_t AIndex, sat_param_t AValue, bool ARedraw) {
    SAT_Print("%i = %.3f %s\n",AIndex,AValue,ARedraw?" (redraw)":"");
  }

  //----------

  // called from SAT_Editor.set_parent()

  virtual SAT_Window* createWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent) {
    SAT_Window* window = new SAT_Window(AWidth,AHeight,AParent,this);
    return window;
  }

//------------------------------
public: // window listener
//------------------------------

  /*
    this is called from whatever thread the timer is called from
    push update into gui thread somwhow..
  */

  /*
    push dirty widgets
    invalidate combined rect

    whenever we redraw (on_window_paint)
    (as result of invalidate?)

      redraw (to buffer) dirty widgets
      copy update rectangle from buffer
      swapbuffers

    be sure to clip when drawing
    (so we're sure not to touch wrong pixels in buffer)

    push root widget as initial dirty rect,
    (to be sure we have a filled buffer)

  */

  void do_window_listener_timer(SAT_Window* ASender) override { // final {
    //SAT_PRINT;
  }

//------------------------------
public: // clap
//------------------------------

  bool create() {
    return true;
  }

  //----------

  void destroy() {
    //if (MWindow) MWindow->on_window_close();
  }

  //----------

  bool set_scale(double scale) {
    //SAT_Print("scale %f\n",scale);
    MScale = scale;
    return true;
  }

  //----------

  bool get_size(uint32_t *width, uint32_t *height) {
    //SAT_Print("-> %i,%i\n",MWidth,MHeight);
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
    //SAT_Print("\n");
    hints->can_resize_horizontally  = true;
    hints->can_resize_vertically    = true;
    hints->preserve_aspect_ratio    = false;
    hints->aspect_ratio_width       = MWidth;
    hints->aspect_ratio_height      = MHeight;
    return true;
  }

  //----------

  /*
    ?? *width/*height contains 'suggested' (resized) size
    return wanted/new size
    bitwig seems to call this repeatedly until the suggested, and returned
    sizes are equal, so be careful with rounding/scaling..
  */

  bool adjust_size(uint32_t *width, uint32_t *height) {
    //SAT_Print("(%i,%i) -> %i,%i\n",*width,*height,MWidth,MHeight);
    return true;
  }

  //----------

  /*
    when our window is embedded, we don't get any on_window_resize
    event, so we simulate one from here..
  */

  bool set_size(uint32_t width, uint32_t height) {
    //SAT_Print("width %i height %i\n",width,height);
    MWidth = width;
    MHeight = height;
    if (MWindow) {
      MWindow->setSize(width,height);
      MWindow->on_window_resize(width,height);
    }
    return true;
  }

  //----------

  bool set_parent(const clap_window_t *window) {
    //SAT_Print("\n");
    MParent = window;
    if (!MWindow) {
      MWindow = createWindow(MWidth,MHeight,window->x11);
    }
    else {
      MWindow->reparent(window->x11);
    }
    return true;
  }

  //----------

  bool set_transient(const clap_window_t *window) {
    //SAT_Print("\n");
    return true;
  }

  //----------

  void suggest_title(const char *title) {
    //SAT_Print("title %s\n",title);
    MTitle = title;
    if (MWindow) MWindow->setTitle(title);
  }

  //----------

  /*
    when our window is embedded, we don't get on_window_open events
  */

  bool show() {
    //SAT_Print("\n");
    if (MWindow && !MIsOpen) {
      if (!MPreparedWidgets) {
        MWindow->on_window_open(); // -> prepareWidgets();
        MPreparedWidgets = true;
      }
      MWindow->show();
      MIsOpen = true;
    }
    return true;
  }

  //----------

  bool hide() {
    //SAT_Print("\n");
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
