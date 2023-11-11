#ifndef sat_editor_included
#define sat_editor_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/sat_editor_listener.h"
#include "gui/base/sat_window_listener.h"
#include "gui/sat_window.h"

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

  SAT_EditorListener*   MListener   = nullptr;
//sat_atomic_bool_t     MIsClosing  = false;

  double                MScale      = 1.0;
  uint32_t              MWidth      = 640;
  uint32_t              MHeight     = 480;
  const char*           MTitle      = "SAT_Editor";
  const clap_window_t*  MParent     = nullptr;
  const clap_window_t*  MTransient  = nullptr;
  SAT_Window*           MWindow     = nullptr;
  bool                  MIsOpen     = false;

//------------------------------
public:
//------------------------------

  //SAT_Editor(SAT_EditorListener* AListener) {
  SAT_Editor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0) {
    //if (AListener) MListener = AListener;
    MListener = AListener;

    #if defined (SAT_GUI_WAYLAND)
      MWindow = new SAT_Window(AWidth,AHeight,this,AParent);
    #elif defined (SAT_GUI_WIN32)
      MWindow = new SAT_Window(AWidth,AHeight,this,AParent);
    #elif defined (SAT_GUI_X11)
      MWindow = new SAT_Window(AWidth,AHeight,this,AParent);
    #endif

    //MWindow->setListener(this);

  }

  //----------

  virtual ~SAT_Editor() {
    if (MWindow /*&& MIsOpen*/) {
      MWindow->close();
      delete MWindow;
    }
  }

//------------------------------
public:
//------------------------------

  virtual SAT_Window* getWindow() {
    return MWindow;
  }

  virtual bool isOpen() {
    return MIsOpen;
  }

//------------------------------
public:
//------------------------------

  // handled in SAT_Plugin

  //virtual bool is_api_supported(const char *api, bool is_floating) { return false; }
  //virtual bool get_preferred_api(const char **api, bool *is_floating) { return false; }
  //virtual bool create(const char *api, bool is_floating) { return false; }
  //virtual void destroy() {}

  //----------

  virtual bool set_scale(double scale) {
    //SAT_Print("scale %f\n",scale);
    MScale = scale;
    return true;
  }

  //----------

  virtual bool get_size(uint32_t *width, uint32_t *height) {
    //SAT_Print("\n");
    *width = MWidth;
    *height = MHeight;
    return true;
  }

  //----------

  virtual bool can_resize() {
    //SAT_Print("\n");
    return true;
  }

  //----------

  virtual bool get_resize_hints(clap_gui_resize_hints_t *hints) {
    //SAT_Print("\n");
    hints->can_resize_horizontally  = true;
    hints->can_resize_vertically    = true;
    hints->preserve_aspect_ratio    = false;
    hints->aspect_ratio_width       = MWidth;
    hints->aspect_ratio_height      = MHeight;
    return true;
  }

  //----------

  virtual bool adjust_size(uint32_t *width, uint32_t *height) {
    //SAT_Print("*width %i *height %i\n",*width,*height);
    return true;
  }

  //----------

  /*
    we don't receive EXPOSE/WM_PAINT events if window is embedded
    (double check if this is actually/still true) -> yes..
    calling window->setSize will call xcb_configure_window (x11),
    which will result in an NOTIFY event (???)

  */

  virtual bool set_size(uint32_t width, uint32_t height) {
    //SAT_Print("width %i height %i\n",width,height);
    MWidth = width;
    MHeight = height;
    if (MWindow) {
      MWindow->setSize(width,height);
      //if (MIsOpen) { MWindow->on_window_resize(width,height); }
    }
    return true;
  }

  //----------

  /*
    todo: delay window creation until show()?
  */

  virtual bool set_parent(const clap_window_t *window) {
    //SAT_Print("\n");
    MParent = window;
    #if defined (SAT_GUI_WAYLAND)
      MWindow->setParent((intptr_t)MParent->ptr);
      return true;
    #elif defined (SAT_GUI_WIN32)
      MWindow->setParent(MParent->win32);
      return true;
    #elif defined (SAT_GUI_X11)
      SAT_PRINT;
      MWindow->setParent(MParent->x11);
      return true;
    #else
      return false;
    #endif
  }

  //----------

  virtual bool set_transient(const clap_window_t *window) {
    //SAT_Print("\n");
    MTransient = window;
    return true;
  }

  //----------

  virtual void suggest_title(const char *title) {
    //SAT_Print("title %s\n",title);
    MTitle = title;
    if (MWindow) MWindow->setTitle(title);
  }

  //----------

  virtual bool show() {
    //SAT_Print("\n");
     if (MWindow && !MIsOpen) {
      // MWindow->markRootWidgetDirty();
      MWindow->open();
      MIsOpen = true;
      return true;
     }
     return false;
  }

  //----------

  virtual bool hide() {
    //SAT_Print("\n");
    if (MWindow && MIsOpen) {
      MWindow->close();
      MIsOpen = false;
      return true;
    }
    return false;
  }

//------------------------------
public: // window listener
//------------------------------

  void on_windowListener_update(SAT_Widget* AWidget) override {
    SAT_Parameter* parameter= (SAT_Parameter*)AWidget->getParameter();
    if (parameter) {
      uint32_t index = parameter->getIndex();
      sat_param_t value = parameter->getValue();
      if (MListener) MListener->on_editorListener_update(index,value);
      SAT_Print("index %i value %f\n",index,value);
    }
  }

  //void on_windowListener_resize(SAT_BaseWindow* AWindow, int32_t AWidth, int32_t AHeight) override {
  //}

  void on_windowListener_timer(SAT_Timer* ATimer, double AElapsed) override {
  }

};

//----------------------------------------------------------------------
#endif

